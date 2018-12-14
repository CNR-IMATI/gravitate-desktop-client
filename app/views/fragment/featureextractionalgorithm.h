/****************************************************************************
* GRAVITATE Desktop Client                                                  *
*                                                                           *
* Consiglio Nazionale delle Ricerche                                        *
* Istituto di Matematica Applicata e Tecnologie Informatiche                *
* Sezione di Genova                                                         *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Authors: Andrea Repetto                                                   *
* Copyright(C) 2018: IMATI-GE / CNR                                         *
* All rights reserved.                                                      *
*                                                                           *
* This program is dual-licensed as follows:                                 *
*                                                                           *
* (1) You may use GRAVITATE Desktop Client as free software; you can        *
* redistribute it and/or modify it under the terms of the GNU General       *
* Public License as published by the Free Software Foundation; either       *
* version 3 of the License, or (at your option) any later  version.         *
* In this case the program is distributed in the hope that it will be       *
* useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
* (2) You may use GRAVITATE Desktop Client as part of a commercial          *
* software. In this case a proper agreement must be reached with the        *
* Authors and with IMATI-GE/CNR based on a proper licensing contract.       *
*                                                                           *
****************************************************************************/
#ifndef FEATUREEXTRACTIONALGORITHM_H
#define FEATUREEXTRACTIONALGORITHM_H

#include "jobmanagerbase.h"
#include <QObject>
#include <QUrl>
#include <houghfeature.h>

struct FeatureExtractionParams
{
public:
    enum class CurveType
    {
        D_CONVEXITIES,
        CITRUS,
        CIRCUMFERENCE,
        ELLIPSE,
        GEOMETRIC_PETAL,
        SPIRAL
    };

    /*!
     * \brief curveId represents the curve type
     *
     * 'CC' is the D-convexities curve
     * 'C' is the citrus curve
     * 'CF' is the cirsumference
     * 'E' is the ellipse
     * 'GP' is the geometric petal curve
     * 'S' is the spiral curve
     */
    QString curveId; // TODO: refactor to enum
    QUrl modelUrl;
    QUrl facetUrl;

    /*!
     * \brief parD is the number of convexities of the D-convexities curve
     * (default value is 8)  or the degree of 'eccentricity' of the geometric petal
     */
    int parD;

    /*!
     * \brief parN is the positive integer number (less or equal to 60) used to
     * estimate the threshold for DBSCAN (default=50)
     */
    int parN;

    /*!
     * \brief minPts is the minimum number of points required from DBSCAN to
     * form a dense region (default=10)
     */
    int minPts;

    /*!
     * \brief eps is the maximum admissible distance of the selected points
     * from the recognized curve (default=0.85)
     */
    double eps;

    /*!
     * \brief useFacet true if the algorithm should make use of facets, false
     * otherwise
     */
    bool useFacet;

    /*!
     * \brief erosion is the number of layers to be deleted from the border of
     * the facet (default=3)
     */
    int erosion;
};

/*!
 * \brief Returns the parameter string for the curve type
 *
 * \param type The curve type
 * \return
 *
 *
 * Summary of the curve types:
 * - 'CC' is the D-convexities curve
 * - 'C' is the citrus curve
 * - 'CF' is the cirsumference
 * - 'E' is the ellipse
 * - 'GP' is the geometric petal curve
 * - 'S' is the spiral curve
 */
const char *paramString(FeatureExtractionParams::CurveType type);

/*!
 * \brief Returns the label string corresponding to a curve type
 * \param type The curve type
 * \return The curve type descriptive string
 */
const char *labelString(FeatureExtractionParams::CurveType type);

//! Serializes the input parameters as a space-separated list
/*!
 * \brief formatParams
 * \param params
 * \return
 */
QByteArray formatParams(const FeatureExtractionParams &params);

class FeatureExtractionAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit FeatureExtractionAlgorithm(JobManagerBase *jobManager, QObject *parent = 0);

    bool run(const FeatureExtractionParams &params);

    QByteArray lastFeatureSetData();

signals:
    void started(const JobId &jobId);
    void finished(bool success, const QList<HoughFeature> &features);
    void canceled(const JobId &jobId);

public slots:
    void cancel();

private slots:
    void onJobStarted(const JobId &jobId);
    void onJobFailed(const JobId &jobId);
    void onJobFinished(const JobId &jobId, const QByteArray &data);

private:
    JobId m_currentJobId;
    JobManagerBase *m_jobManager;
    QByteArray m_lastFeatureSetData;
};

#endif // FEATUREEXTRACTIONALGORITHM_H

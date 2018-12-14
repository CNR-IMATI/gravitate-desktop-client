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
#ifndef ARTEFACT3DMETADATA_H
#define ARTEFACT3DMETADATA_H

#include <QMetaType>
#include <QString>
#include <QList>
#include <QUrl>
#include <QStringList>
#include <QMap>
#include <QPair>
#include "enums.h"
#include "unitofmeasure.h"
#include "artefactid.h"

/**
 * @brief This class collects all the metadata which concerns the 3D model
 * representation of a Cultural Heritage Artefact
 */
class Artefact3DMetadata
{
public:

    ArtefactId id;         // The Fragment ID, it is usually a URI

    double area = -1.0; // Area of the whole fragment
    double volume = -1.0; // Volume of the fragment
    double avgthickness = 0.0;

    qudt::Unit unit = qudt::UNKNOWN;

    // Linear unit of measure
    QString unitOfMeasure = "??";

    // minimal bounding box URL - not dependent on resolution
    QUrl mbbUrl;

    void setUrl(ModelResolution res, MultiResProperty prop, const QUrl &url);

    QUrl getUrl(ModelResolution res, MultiResProperty prop, bool useFallback = false) const;

    bool hasUrl(ModelResolution res, MultiResProperty prop) const;

    QList<ModelResolution> availableResolutions(MultiResProperty property) const;

    bool isValid();

    ModelResolution primaryResolution();

    // TODO: extract class for unit-of-measure conversion
    static qudt::Unit fromQudt(const QUrl &url);

    // TODO: extract class for unit-of-measure conversion
    static QString unitString(qudt::Unit measure);

private:

    QMap<QPair<ModelResolution, MultiResProperty>, QUrl> m_multiResMap;

};


Q_DECLARE_METATYPE(Artefact3DMetadata)



#endif // ARTEFACT3DMETADATA_H

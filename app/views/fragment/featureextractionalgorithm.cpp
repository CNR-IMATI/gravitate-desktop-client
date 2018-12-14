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
#include "featureextractionalgorithm.h"
#include <QDebug>
#include <QJsonDocument>
#include <QTimer>
#include <iostream>
#include <sstream>
#include <QList>
#include <QVector>

namespace {
    const AlgorithmId ALGO_ID = AlgorithmId::fromString("featuresExtraction");
}

FeatureExtractionAlgorithm::FeatureExtractionAlgorithm(JobManagerBase *jobManager, QObject *parent)
    : QObject(parent), m_jobManager(jobManager)
{
    if(jobManager == nullptr)
    {
        qCritical() << "JobManager not initialized properly";
        return;
    }

    connect(m_jobManager, &JobManagerBase::jobFinished,
            this, &FeatureExtractionAlgorithm::onJobFinished);

    connect(m_jobManager, &JobManagerBase::jobFailed,
            this, &FeatureExtractionAlgorithm::onJobFailed);

//    connect(m_jobManager, &JobManagerBase::jobStarted,
//            this, &FeatureExtractionAlgorithm::onJobStarted);
}

bool FeatureExtractionAlgorithm::run(const FeatureExtractionParams &params)
{
    if(m_jobManager == nullptr)
    {
        qCritical() << "JobManager not initialized properly";
        return false;
    }

    qInfo() << "Starting Feature Extraction algorithm";

    QByteArray payload = formatParams(params);

    auto jobId = m_jobManager->startJob(ALGO_ID, payload);
    if(jobId.isValid())
    {
        m_currentJobId = jobId;
        emit started(jobId);
    }

    return jobId.isValid();
}

QByteArray FeatureExtractionAlgorithm::lastFeatureSetData()
{
    return m_lastFeatureSetData;
}

void FeatureExtractionAlgorithm::cancel()
{
    m_jobManager->cancelJob(m_currentJobId);
    emit canceled(m_currentJobId);


    m_currentJobId = JobId();
}

void FeatureExtractionAlgorithm::onJobStarted(const JobId &jobId)
{
    m_currentJobId = jobId;
    emit started(jobId);
}

void FeatureExtractionAlgorithm::onJobFailed(const JobId &jobId)
{
    emit finished(false, QList<HoughFeature>());
}

void FeatureExtractionAlgorithm::onJobFinished(const JobId &jobId, const QByteArray &data)
{
    if(!m_currentJobId.isValid())
    {
        qDebug() << "No jobs running at the moment";
        emit finished(false, QList<HoughFeature>());
        return;
    }

    if(jobId != m_currentJobId)
    {
        qDebug() << "Not the job you are looking for...";
        emit finished(false, QList<HoughFeature>());
        return;
    }

    if(data.isEmpty())
    {
        qCritical() << "The job appears successful, but the output data is empty";
        emit finished(false, QList<HoughFeature>());
        return;
    }

    m_lastFeatureSetData = data;

    HoughFeatureReader reader;

    std::istringstream in(data.toStdString());

    auto features = reader.parse(in);

    auto fVector = QVector<HoughFeature>::fromStdVector(features);

    QList<HoughFeature> outFeatures = QList<HoughFeature>::fromVector(fVector);

    emit finished(true, outFeatures);
}

QByteArray formatParams(const FeatureExtractionParams &params)
{
    QStringList paramsList;
    paramsList.append(params.modelUrl.toString());
    QString facetUrl = params.useFacet ? params.facetUrl.toString() : "EMPTY";
    paramsList.append(facetUrl);
    paramsList.append(params.curveId);
    paramsList.append(QString::number(params.parD));
    paramsList.append(QString::number(params.parN));
    paramsList.append(QString::number(params.minPts));
    paramsList.append(QString::number(params.eps));
    QString useFacet = params.useFacet ? "true" : "false";
    paramsList.append(useFacet);
    paramsList.append(QString::number(params.erosion));

    return paramsList.join(" ").toUtf8();
}

const char *paramString(FeatureExtractionParams::CurveType type)
{
    switch (type) {
    case FeatureExtractionParams::CurveType::CIRCUMFERENCE: return "CF";
    case FeatureExtractionParams::CurveType::CITRUS:        return "C";
    case FeatureExtractionParams::CurveType::D_CONVEXITIES: return "CC";
    case FeatureExtractionParams::CurveType::ELLIPSE:       return "E";
    case FeatureExtractionParams::CurveType::GEOMETRIC_PETAL: return "GP";
    case FeatureExtractionParams::CurveType::SPIRAL:        return "S";
    }
    return "ND";
}

const char *labelString(FeatureExtractionParams::CurveType type)
{
    switch (type) {
    case FeatureExtractionParams::CurveType::CIRCUMFERENCE: return "Circumference";
    case FeatureExtractionParams::CurveType::CITRUS:        return "Citrus";
    case FeatureExtractionParams::CurveType::D_CONVEXITIES: return "D-Convexities";
    case FeatureExtractionParams::CurveType::ELLIPSE:       return "Ellipse";
    case FeatureExtractionParams::CurveType::GEOMETRIC_PETAL: return "Geometric Petal";
    case FeatureExtractionParams::CurveType::SPIRAL:        return "Spiral";
    }
    return "Unknown curve type";
}

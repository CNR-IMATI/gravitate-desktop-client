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
#include "gravitatejobmanager.h"
#include "gravitatebackendconf.h"
#include <QJsonDocument>
#include <QEventLoop>
#include <qxtsignalwaiter.h>

namespace {
    const QString QUERY_ALGORITHM_ID = "algorithmId";
    const QString QUERY_PARAMETERS = "parameters";
}

GravitateJobManager::GravitateJobManager(BackendProvider *backend, QObject *parent)
    : JobManagerBase (parent),
      m_backend(backend)
{
    m_watcher = new GravitateJobWatcher(m_backend);

    connect(m_watcher, &GravitateJobWatcher::resultReady,
            this, &GravitateJobManager::jobFinished);

    connect(m_watcher, &GravitateJobWatcher::failed,
            this, &GravitateJobManager::jobFailed);
}

JobId GravitateJobManager::startJob(const AlgorithmId &algoId, const QByteArray &payload)
{
    QUrlQuery query;
    query.addQueryItem(QUERY_ALGORITHM_ID, algoId);
    query.addQueryItem(QUERY_PARAMETERS, payload);

    auto reply = m_backend->apiPost(GravitateBackendConf::SchedulerJobsEndpoint, query);

    JobId jobId;

    connect(reply, &QNetworkReply::finished,
            reply, [this, reply, &jobId, algoId] {
        if(reply->error() != QNetworkReply::NoError)
        {
            qCritical() << "Could not start new job for algorithm with ID" << algoId;
            qCritical() << reply->errorString();
            return;
        }

        auto replyContent = reply->readAll();


        bool ok = parseJobId(replyContent, jobId);

        if(!ok)
        {
            qCritical() << "Error parsing the job ID";
        }

        reply->deleteLater();
    });

    bool ok = QxtSignalWaiter::wait(reply, SIGNAL(finished()), 5000);

    if(!ok)
    {
        qCritical() << "Job" << jobId << ": no reply from server";
        return JobId();
    }

    if(jobId.isValid())
    {
        addToActiveJobs(jobId);
    }

    return jobId;
}

void GravitateJobManager::cancelJob(const JobId &jobId)
{
    m_watcher->stop();
}

bool parseJobId(const QByteArray &data, JobId &jobId)
{
    auto doc = QJsonDocument::fromJson(data);
    auto dict = doc.toVariant().toMap();

    if(dict.isEmpty())
    {
        jobId = QString();
        return false;
    }

    jobId = dict.value("id").toString();
    return true;
}

void GravitateJobManager::addToActiveJobs(const JobId &jobId)
{
    qDebug() << "Job" << jobId << "added to the watchlist";
    emit jobStarted(jobId);

    m_watcher->watchJob(jobId);
}


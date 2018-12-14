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
#include "gravitatejobwatcher.h"
#include "gravitatebackendconf.h"
#include "qxtsignalwaiter.h"

namespace {



    const int pollingInterval = 2000;
}

GravitateJobWatcher::GravitateJobWatcher(BackendProvider *backend, QObject *parent)
    : QObject(parent), m_backend(backend)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(pollingInterval);
}

void GravitateJobWatcher::watchJob(const JobId &jobId)
{
    m_currentJobId = jobId;

    m_timer->disconnect();
    connect(m_timer, &QTimer::timeout,
            m_timer, [this,jobId] {

        QByteArray result;

        auto state = getJobState(jobId);

        if(state == JobState::RUNNING)
        {
            return;
        }

        // Then the job has finished either with success or failure
        if(state == JobState::FAILURE)
        {
            emit failed(jobId);
        }
        else if(state == JobState::SUCCESS)
        {
            getJobResults(jobId, result);
            emit resultReady(jobId, result);
        }

        stop();

        qDebug() << "Job" << jobId << "finished:" << stateToString(state);
    });
    m_timer->start();
}

void GravitateJobWatcher::stop()
{
    m_currentJobId = JobId();
    m_timer->stop();
}

JobId GravitateJobWatcher::currentJobId()
{
    return m_currentJobId;
}

bool GravitateJobWatcher::getJobResults(const JobId &jobId, QByteArray &data)
{
    if(m_backend == nullptr)
    {
        qCritical() << "Backend provider not initialized properly";
        return false;
    }

    auto route = GravitateBackendConf::JobsResults(jobId);

    auto reply = m_backend->apiGet(route);

    bool ok = QxtSignalWaiter::wait(reply, SIGNAL(finished()), 5000);

    if(!ok)
    {
        qDebug() << "Timeout error; no reply from server";
        return false;
    }

    auto attr = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(attr.toInt() == 404)
    {
        return false;
    }

    if(attr.toInt() != 200)
    {
        qCritical() << "An error has occurred while retrieving the job results:"
                    << reply->errorString();
        return true;
    }

    // Then we have a valid and "successful" reply
    data = reply->readAll();
    return true;
}

GravitateJobWatcher::JobState GravitateJobWatcher::getJobState(const JobId &jobId)
{
    // NOTE: some code is duplicated between getJobResults and getJobState
    // because they execute two distinct API calls, (/result and /logs) so
    // their internal logic could be changed independently

    if(m_backend == nullptr)
    {
        qCritical() << "Backend provider not initialized properly";
        return JobState::FAILURE;
    }

    auto route = GravitateBackendConf::JobLog(jobId);

    auto reply = m_backend->apiGet(route);

    bool ok = QxtSignalWaiter::wait(reply, SIGNAL(finished()), 5000);

    if(!ok)
    {
        qCritical() << "Timeout error while checking the execution state";
        return JobState::FAILURE;
    }

    auto attr = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(reply->error() != QNetworkReply::NoError)
    {
        qCritical() << "An error has occurred when retrieving the job status:"
                    << reply->errorString();
        return JobState::FAILURE;
    }

    auto logContent = reply->readAll();

    if(isLogNoOutputFound(logContent))
    {
        return JobState::RUNNING;
    }

    return isLogSuccessful(logContent) ? JobState::SUCCESS : JobState::FAILURE;
}

bool GravitateJobWatcher::isLogNoOutputFound(const QString &log)
{
    return log.contains("No output found for the job");
}

bool GravitateJobWatcher::isLogSuccessful(const QString &log)
{
    return ! (log.isEmpty() && log.contains("Error in"));
}

QString GravitateJobWatcher::stateToString(GravitateJobWatcher::JobState state)
{
    switch (state) {
        case JobState::SUCCESS: return "SUCCESS";
        case JobState::FAILURE: return "FAILURE";
        case JobState::RUNNING: return "RUNNING";
    }
    return QString();
}


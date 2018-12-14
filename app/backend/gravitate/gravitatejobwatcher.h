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
#ifndef GRAVITATEJOBWATCHER_H
#define GRAVITATEJOBWATCHER_H

#include <QObject>
#include "backendprovider.h"
#include "jobmanagerbase.h"
#include <QTimer>

class GravitateJobWatcher : public QObject
{
    enum class JobState
    {
        RUNNING,
        SUCCESS,
        FAILURE
    };

    Q_OBJECT
public:
    explicit GravitateJobWatcher(BackendProvider *backend, QObject *parent = nullptr);

    JobId currentJobId();

public slots:
    void watchJob(const JobId &jobId);
    void stop();

signals:
    void resultReady(const JobId &jobId, const QByteArray &result);
    void failed(const JobId &jobId);

private:
    JobId m_currentJobId;
    BackendProvider *m_backend;
    QTimer *m_timer;
    bool getJobResults(const JobId &jobId, QByteArray &data);
    JobState getJobState(const JobId &jobId);

    bool isLogNoOutputFound(const QString &log);
    bool isLogSuccessful(const QString &log);
    QString stateToString(JobState state);
};

#endif // GRAVITATEJOBWATCHER_H

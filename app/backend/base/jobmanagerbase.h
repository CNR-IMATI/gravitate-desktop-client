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
#ifndef JOBMANAGERBASE_H
#define JOBMANAGERBASE_H

#include <QObject>

struct JobId
{
    QString value;

    friend bool operator==(const JobId &first, const JobId &second);
    friend bool operator!=(const JobId &first, const JobId &second);

    explicit JobId()
    {
        value = QString();
    }

    static JobId fromString(const QString &id)
    {
        JobId outId;
        outId.value = id;
        return outId;
    }

    bool isValid()
    {
        return !value.isEmpty();
    }

    operator QString() const {
        return value;
    }

    JobId &operator=(const QString& rhs)
    {
        value = rhs;
        return *this;
    }
};

inline bool operator==(const JobId &first, const JobId &second)
{
    return first.value == second.value;
}

inline bool operator!=(const JobId &first, const JobId &second)
{
    return first.value != second.value;
}

Q_DECLARE_METATYPE(JobId)

struct AlgorithmId
{
    QString value;

    static AlgorithmId fromString(const QString &id)
    {
        AlgorithmId outId;
        outId.value = id;
        return outId;
    }

    bool isValid()
    {
        return !value.isEmpty();
    }

    bool operator==(const AlgorithmId &other) const
    {
        return value == other.value;
    }

    operator QString() const {
        return value;
    }

    AlgorithmId operator=(const QString &rhs)
    {
        value = rhs;
        return *this;
    }
};

Q_DECLARE_METATYPE(AlgorithmId)


class JobManagerBase : public QObject
{
    Q_OBJECT

public:
    explicit JobManagerBase(QObject * parent = nullptr)
        : QObject(parent)
    {
    }

    virtual JobId startJob(const AlgorithmId &algoId, const QByteArray &payload) = 0;

    virtual void cancelJob(const JobId &jobId) = 0;

signals:
    /*!
     * \brief jobStarted is emitted when a new job has been started
     * \param jobId the job id
     */
    void jobStarted(const JobId &jobId);

    /*!
     * \brief jobFailed is emitted the
     * \param jobId the job id
     */
    void jobFailed(const JobId &jobId);

    /*!
     * \brief jobFinished is emitted when a job finishes successfully
     * \param jobId the job id
     * \param data the data produced as output
     */
    void jobFinished(const JobId &jobId, const QByteArray &data);
};

#endif // JOBMANAGERBASE_H

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
#ifndef GRAVITATEBACKENDCONF_H
#define GRAVITATEBACKENDCONF_H

#include <QString>
#include "jobmanagerbase.h"

namespace GravitateBackendConf {

    const QString ClipboardEndpoint = "/clipgroups/clipboard";
    const QString GroupsEndpoint = "/clipgroups/groups";
    const QString ContainersEndpoint = "/clipgroups/container";
    const QString SparqlEndpoint = "/sparql";
    const QString FragmentMetadataEndpoint = "/gravapi/fragmentmetadata";
    const QString SchedulerAlgorithmsEndpoint = "/gravitate-scheduler/algorithms";
    const QString SchedulerJobsEndpoint = "/gravitate-scheduler/jobs";
    const QString ThumbnailEndpoint = "/rest/data/rdf/utils/thumbnails/default";
    const QString MetadataChEndpoint = "/gravapi/fragmentmetadata/ch";

    inline QString JobsResults(const QString &jobId) {
        return QString("%1/%2/result")
                .arg(SchedulerJobsEndpoint)
                .arg(jobId);
    }

    // TODO: change to
    inline QString JobLog(const JobId &jobId)  {
        return QString("%1/%2/logs")
                .arg(SchedulerJobsEndpoint)
                .arg(jobId);
    }
}

#endif // GRAVITATEBACKENDCONF_H

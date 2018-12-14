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
#include "artefactloadhelper.h"
#include "backendfactorybase.h"
#include "metadata3drequest.h"
#include "downloadprogressdialog.h"

namespace ArtefactLoading
{

ArtefactLoadHelper::ArtefactLoadHelper(AbstractDashboard *dashboard, QObject *parent)
    : QObject(parent), m_dashboard(dashboard)
{
}

bool ArtefactLoadHelper::loadArtefactInfo(const ArtefactId &artefactId, Artefact3DMetadata &artefact)
{
    qInfo() << "Loading info for artefact" << artefactId.toString();

    auto req = BackendFactoryBase::instance()->createMetadata3DRequest();

    if(!req->requestMetadataSync(artefactId, artefact))
    {
        emit error(tr("Error loading the 3D metadata for the artefact"));
        return false;
    }
    return true;
}

bool ArtefactLoadHelper::getLocalFile(const QUrl &remoteUrl, QString &localFile)
{
    // Load the 3D model: the resolution is the active one
    if(remoteUrl.isEmpty())
    {
        emit error("File not available for the current resolution");
        return false;
    }

    auto dl = m_dashboard->downloader();

    DownloadProgressDialog dialog(dl);
    auto fileName = remoteUrl.fileName();

    dialog.setLabelText(tr("Downloading '%1'...").arg(fileName));

    qInfo() << "Downloading file:" << remoteUrl.toString();

    localFile = dl->getLocalPath(remoteUrl);

    if(dialog.wasCanceled())
    {
        qInfo() << "Download of the file" << remoteUrl.fileName()
                << "cancelled by the user";
        return false;
    }

    if(localFile.isEmpty())
    {
        emit error(tr("Error downloading file: \"%1\"").arg(localFile));
        return false;
    }
    return true;
}

void initPropertySelector(PropertySelector *selector, PropertyType type)
{
    if(!selector->hasProperty(type))
    {
        selector->addProperty(type);
    }
    selector->setCurrentProperty(type);
}

}

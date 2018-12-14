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
#ifndef GRAVITATETHUMBNAILPROVIDER_H
#define GRAVITATETHUMBNAILPROVIDER_H

#include <QObject>
#include <QUrl>
#include <QPixmap>
#include <QMap>
#include "backendprovider.h"
#include "abstractdownloader.h"
#include "thumbnailprovider.h"

class GravitateThumbnailProvider : public AbstractThumbnailProvider
{
    Q_OBJECT
public:
    explicit GravitateThumbnailProvider(BackendProvider *backend, AbstractDownloader *downloader, QObject *parent = nullptr);

    bool hasThumbnail(const ArtefactId &artefactId) override;

    QPixmap getPixmap(const ArtefactId &artefactId) override;

public slots:
    void requestArtefactThumbnail(const ArtefactId &artefactId) override;

private:
    void requestThumbnailApi(const ArtefactId &artefactId);

    void requestThumbnailSparql(const ArtefactId &artefactId);

    QPixmap createSquarePixmap(const QPixmap &pixmap, int size);

    void handleThumbnailUrl(const ArtefactId &artefactId, const QUrl &thumbUrl);

    bool isImageFile(const QUrl &thumbUrl);

    QMap<ArtefactId, QPixmap> m_loadedPixmaps;
    QList<ArtefactId> m_requestedPixmaps;

    QPixmap m_emptyPixmap;

    BackendProvider *m_backend = nullptr;
    AbstractDownloader *m_downloader = nullptr;

    int m_pixmapSize = 512;
};

#endif // GRAVITATETHUMBNAILPROVIDER_H

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
#include "downloadprogressdialog.h"
#include "abstractdownloader.h"

DownloadProgressDialog::DownloadProgressDialog(AbstractDownloader *downloader, QWidget *parent)
    : QProgressDialog (parent), m_downloader(downloader)
{

//    QProgressDialog progress("Download in progress...","Cancel", 0, 0, m_parentWidget);


    setLabelText(tr("Download in progress") + "...");
    setCancelButtonText(tr("Cancel"));
    setMinimumDuration(100);
    setMinimum(0);
    setValue(0);
    setMaximum(0);
    setWindowModality(Qt::WindowModal);

    connect(downloader, &AbstractDownloader::downloadProgress,
            this, &DownloadProgressDialog::updateProgress);

    connect(this, &QProgressDialog::canceled,
            downloader, &AbstractDownloader::cancel);
}

void DownloadProgressDialog::updateProgress(qint64 bytesRead, qint64 bytesTotal)
{
    if(bytesTotal == -1)
        return;

    int total = int(bytesTotal);
    int read = int(bytesRead);

    if(total >= 0 && read >= 0)
    {
        setMaximum(total);
        setValue(read);
    }
}

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
#ifndef ABSTRACTDASHBOARD_H
#define ABSTRACTDASHBOARD_H

#include "clipboardmodel.h"
#include "helphandler.h"
#include "thumbnailprovider.h"
#include "backendprovider.h"
#include "abstractdownloader.h"
#include "gravitatejobmanager.h"

/**
 * @brief The AbstractDashboard class is an interface of the
 * common functionalities that the main app should provide to views,
 * such as the Clipboard, the Help Handler and others. *
 */
class AbstractDashboard
{
public:
    virtual ClipboardModel * clipboardModel() const = 0;

    virtual HelpHandler *helpHandler() const = 0;

    virtual AbstractThumbnailProvider *thumbnailProvider() const = 0;

    virtual AbstractDownloader *downloader() const = 0;

    virtual GravitateJobManager *jobManager() const = 0;

    virtual ~AbstractDashboard(){}
};


#endif // ABSTRACTDASHBOARD_H

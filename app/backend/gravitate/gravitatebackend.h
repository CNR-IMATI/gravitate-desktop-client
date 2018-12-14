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
#ifndef GRAVITATEBACKEND_H
#define GRAVITATEBACKEND_H

#include "networkbackendprovider.h"
#include <QDebug>
#include <QSharedPointer>
#include "abstractdownloader.h"
#include "gravitatejobmanager.h"

class GravitateBackend
{
public:
    /**
     * @brief backendProvider returns an instance of NetworkBackendProvider already configured
     * with username and password from the user settings
     * @return a pointer to NetworkBackendProvider configured for the Gravitate Backend
     */
    static BackendProvider * const backendProvider();

    static AbstractDownloader * const defaultDownloader();

    static GravitateJobManager * const jobManager();

protected:
    GravitateBackend();

};

#endif // GRAVITATEBACKEND_H

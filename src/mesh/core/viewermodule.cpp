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
#include <QTimer>
#include "viewermodule.h"
#include "meshviewwidget.h"

ViewerModule::ViewerModule(MeshViewWidget *parent)
    : QObject(parent)
{
    QTimer::singleShot(0, this, &ViewerModule::init);
}

MeshViewWidget *ViewerModule::viewerWidget()
{
    return qobject_cast<MeshViewWidget*>(parent());
}

void ViewerModule::setEnabled(bool enabled)
{
    //viewerWidget()->toolbar()->setEnabled(enabled);
}

void ViewerModule::init()
{
    onToolbarInit(viewerWidget()->toolbar());
}

void ViewerModule::onToolbarInit(QToolBar *toolBar)
{

}

void ViewerModule::setErrorMessage(const QString &errorMessage)
{
    m_errorMessage = errorMessage;
}

QString ViewerModule::errorMessage()
{
    return m_errorMessage;
}

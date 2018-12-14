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
#ifndef VIEWERMODULE_H
#define VIEWERMODULE_H

#include <QObject>
#include <QToolBar>

class MeshViewWidget;

class ViewerModule : public QObject
{
    Q_OBJECT
public:

    explicit ViewerModule(MeshViewWidget *parent);

    MeshViewWidget * viewerWidget();

    virtual void setEnabled(bool enabled);

    QString errorMessage();

protected:
    virtual void onToolbarInit(QToolBar *toolBar);

    void setErrorMessage(const QString &errorMessage);

private slots:
    /**
     * @brief init is the initialization function. This slot
     * is called upon object creation with a QTimer, so that
     * virtual functions overriden in derived classes are chosen
     * with dynamic binding.
     */
    void init();

private:
    MeshViewWidget *m_viewWidget;

    QString m_errorMessage;
};


#endif // VIEWERMODULE_H

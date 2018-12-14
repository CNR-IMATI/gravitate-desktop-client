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
#ifndef VIEWCONTAINER_H
#define VIEWCONTAINER_H

#include <QWidget>
#include <QStackedWidget>
#include "abstractview.h"
#include "viewselectionbar.h"

class ViewContainer : public QWidget
{
    Q_OBJECT
public:
    ViewContainer(QWidget *parent);

    QWidget *selectorWidget();
    AbstractView *currentView();

    void addView(const QString &name, AbstractView *view);

    QString name(AbstractView *view);
    int indexOf(AbstractView *view);

signals:
    void currentChanged(int index);

public slots:
    void setCurrentView(int index);

private:
    QStackedWidget *m_stackedWidget;
    ViewSelectionBar *m_selectionBar;
};

#endif // VIEWCONTAINER_H

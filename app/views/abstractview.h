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
#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QMainWindow>
#include <QUrl>
#include <QList>
#include <QString>
#include "toolsetwidget.h"

#include <QStringList>
#include <QInputDialog>
#include "artefact3dmetadata.h"
#include "gresource.h"

#include "clipboardmodel.h"

#include <QAction>
#include "abstractdashboard.h"

class MainWindow;
class BrowseDialog;
class HelpHandler;
class ClipboardModel;

class AbstractView : public QMainWindow
{
    struct Dataset
    {
        QString name;
        QString resourcePath;
    };

    Q_OBJECT

public:
    explicit AbstractView(AbstractDashboard *dashboard, QWidget *parent = 0);
    virtual void initTools();

    virtual ~AbstractView();

    QWidget *getToolWidget();
    ToolsetWidget &toolSet();

public slots:

    virtual void onClipboardContextMenuOpen(QMenu *menu, const GResource &resource);

private slots:
    void onBrowseTriggered();

protected:
    void addDefaultTools();

    ToolsetWidget *m_toolSet;

    QAction *action_Browse;

    QMenu *m_dataMenu;

    AbstractDashboard *m_dashboard;

private:
    void addDatasetToBrowseDialog(const QString &datasetName, const QString &listFile, BrowseDialog *dialog);
};

#endif // ABSTRACTVIEW_H

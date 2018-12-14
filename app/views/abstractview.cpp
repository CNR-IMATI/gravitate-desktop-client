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
#include "abstractview.h"
#include <QString>
#include <QMenu>
#include "gravitatebackend.h"
#include "localfilecache.h"
#include "abstractdashboard.h"
#include "browsedialog.h"
#include "datasetfetchmodel.h"
#include <QStringList>
#include <QTextStream>
#include "helphandler.h"

AbstractView::AbstractView(AbstractDashboard *dashboard, QWidget *parent)
    : QMainWindow(parent),
      m_dashboard(dashboard)
{
    m_toolSet = new ToolsetWidget(this);

    action_Browse = new QAction(tr("Browse"));

    connect(action_Browse, &QAction::triggered,
            this, &AbstractView::onBrowseTriggered);
}

void AbstractView::initTools()
{
}

AbstractView::~AbstractView()
{

}

QWidget *AbstractView::getToolWidget()
{
    return &toolSet();
}

ToolsetWidget &AbstractView::toolSet()
{
    return *m_toolSet;
}

void AbstractView::onClipboardContextMenuOpen(QMenu *menu, const GResource &resource)
{

}

void AbstractView::onBrowseTriggered()
{
    BrowseDialog dialog;
    DatasetFetchModel fetchModel(m_dashboard, this);

    dialog.setDatasetFetchModel(&fetchModel);
    dialog.setClipboardModel(m_dashboard->clipboardModel());

    addDatasetToBrowseDialog("Salamis",   ":/datasets/salamis.txt", &dialog);
    addDatasetToBrowseDialog("Naukratis", ":/datasets/naukratis.txt", &dialog);
    addDatasetToBrowseDialog("Ayia Irini", ":/datasets/ayia_irini.txt", &dialog);
    addDatasetToBrowseDialog("Gath Jars", ":/datasets/gath_jars.txt", &dialog);

    dialog.exec();
}


void AbstractView::addDefaultTools()
{
    m_dataMenu = new QMenu(tr("Data"));

    toolSet().beginMenu(m_dataMenu)
                .addToolPlaceholder(tr("Datasets"))
                .addTool(action_Browse)
            .endMenu();

    auto handler = m_dashboard->helpHandler();

    if(handler == nullptr)
    {
        qCritical() << "HelpHandler not initialized" << Q_FUNC_INFO;
        return;
    }

    auto dataButton = toolSet().getToolButton(m_dataMenu);
    if(dataButton != nullptr)
    {
        dataButton->setWhatsThis(handler->getText("menu-data"));
    }
}

void AbstractView::addDatasetToBrowseDialog(const QString &datasetName, const QString &listFile, BrowseDialog *dialog)
{
    QFile f(listFile);
    if(!f.open(QIODevice::ReadOnly))
    {
        qCritical() << "Error opening file" << listFile;
        return;
    }

    QTextStream stream(&f);

    QStringList artefacts;

    QString line;

    while(stream.readLineInto(&line))
    {
        if(line.size() == 0)
            continue;

        artefacts.append(line);
    }


    dialog->addDataset(datasetName, artefacts);
}

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
#include "groupsviewmain.h"
#include "ui_groupsviewmain.h"
#include <QInputDialog>
#include <QMessageBox>
#include "abstractdashboard.h"
#include "textnoteedit.h"

GroupsViewMain::GroupsViewMain(AbstractDashboard *dashboard, QWidget *parent) :
    AbstractView(dashboard, parent),
    ui(new Ui::GroupsViewMain)
{
    ui->setupUi(this);

    m_proxyModel = new GroupProxyModel(this);

    connect(this, &GroupsViewMain::activeGroupChanged,
            [this] (const GroupId &id, const QString &name) {
        m_currentGroupId = id;
        m_currentGroupName = name;
    });

    connect(this, &GroupsViewMain::activeGroupChanged,
            [this] (const GroupId &id, const QString &name) {
        ui->label->setText(
                    tr("%1 [%2]")
                    .arg(name)
                    .arg(numberOfItemsInGroup(id)));
    });

    setClipboardModel(m_dashboard->clipboardModel());
}

GroupsViewMain::~GroupsViewMain()
{
    delete ui;
}

void GroupsViewMain::setClipboardModel(ClipboardModel *model)
{
    if(model == nullptr)
    {
        qDebug() << "Invalid ClipboardModel";
        return;
    }

    m_clipboardModel = model;

    m_proxyModel->setSourceModel(m_clipboardModel);
}

int GroupsViewMain::numberOfItemsInGroup(const GroupId &groupId)
{
    auto index = m_clipboardModel->findGroupIndex(groupId);
    return m_clipboardModel->rowCount(index);
}

void GroupsViewMain::onClipboardContextMenuOpen(QMenu *menu, const GResource &resource)
{
    if(resource.type == GResource::Type::GROUP)
    {
        auto action = new QAction(tr("Load group"));
        connect(action, &QAction::triggered,
                [this, resource] {
            auto index = m_clipboardModel->findGroupIndex(resource.groupId);
            selectCurrentGroup(index);
        });
        menu->addAction(action);
    }
    else if(resource.type == GResource::Type::ARTEFACT)
    {
        auto addToClipboard = new QAction(tr("Add to Clipboard"));
        connect(addToClipboard, &QAction::triggered,
                [this, resource] {
           m_clipboardModel->addToClipboard(resource.artefactId);
        });
        menu->addAction(addToClipboard);

        if(!m_currentGroupId.isNull())
        {

            QString actionLabel = tr("Add to group \"%1\"").arg(m_currentGroupName);
            auto addToGroup = new QAction(actionLabel);
            connect(addToGroup, &QAction::triggered,
                    [this, resource] {
                m_clipboardModel->addToGroup(resource.artefactId, m_currentGroupId);
            });
            menu->addAction(addToGroup);
        }
    }
}

void GroupsViewMain::initTools()
{
    toolSet().addTool(ui->actionAdd_Note)
            .addTool(ui->actionCreate_Group)
            .addTool(ui->actionAdd_to_Clipboard);

    connect(ui->actionAdd_Note, &QAction::triggered,
            this, &GroupsViewMain::onActionAddNote);
    connect(ui->actionCreate_Group, &QAction::triggered,
            this, &GroupsViewMain::onActionCreateGroup);
    connect(ui->actionAdd_to_Clipboard, &QAction::triggered,
            this, &GroupsViewMain::onActionAddToClipboard);

    auto handler = m_dashboard->helpHandler();

    if(handler == nullptr)
    {
        qDebug() << "HelpHandler not initialized";
        return;
    }

    ui->actionAdd_Note->setWhatsThis(handler->getText("tool-add-note"));
    ui->actionCreate_Group->setWhatsThis(handler->getText("tool-create-group"));
    ui->actionAdd_to_Clipboard->setWhatsThis(handler->getText("tool-add-to-clipboard"));
}

void GroupsViewMain::onActionCreateGroup()
{
    QString name = QInputDialog::getText(this, tr("Enter the name of the group"), tr("Group Name"));
    if(!name.isEmpty())
    {
        m_clipboardModel->createGroup(name);
    }
}

void GroupsViewMain::selectCurrentGroup(const QModelIndex &index)
{
    if(ui->groupContentView->model() == Q_NULLPTR)
    {
        ui->groupContentView->setModel(m_clipboardModel);
    }

    ui->groupContentView->setRootIndex(index);

    auto resource = m_clipboardModel->resource(index);

    m_currentGroupId = resource.groupId;
    m_currentGroupName = resource.label;

    emit activeGroupChanged(m_currentGroupId, m_currentGroupName);
}

void GroupsViewMain::onActionAddToClipboard()
{
    QModelIndex currentSelected = selectedItemInContentView();

    if(!currentSelected.isValid())
    {
        return;
    }

    auto artefactId = m_clipboardModel->artefactId(currentSelected);

    m_clipboardModel->addToClipboard(artefactId);

}

QModelIndex GroupsViewMain::selectedItemInContentView()
{
    if(ui->groupContentView->selectionModel() == nullptr)
    {
        return QModelIndex();
    }

    QModelIndex currentSelected = ui->groupContentView->selectionModel()->currentIndex();

    return currentSelected;
}

void GroupsViewMain::onActionAddNote()
{
    TextNoteEdit editDialog;

    auto groupName = m_currentGroupName.isEmpty()
            ? "Clipboard" : m_currentGroupName;

    editDialog.setGroupName(groupName);

    auto ret = editDialog.exec();

    if(ret != QDialog::Accepted)
    {
        return;
    }
}

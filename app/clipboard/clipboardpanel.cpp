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
#include "clipboardpanel.h"
#include "ui_clipboardpanel.h"

#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QInputDialog>

ClipboardPanel::ClipboardPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClipboardPanel)
{
    ui->setupUi(this);
}

void ClipboardPanel::setClipboardModel(ClipboardModel *model)
{
    m_model = model;
    ui->clipboardView->setModel(model);

    ui->clipboardView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->clipboardView, &QTreeView::customContextMenuRequested,
            this, &ClipboardPanel::onContextMenuRequested);
}

ClipboardPanel::~ClipboardPanel()
{
    delete ui;
}

QModelIndex ClipboardPanel::selectedIndex()
{
    return ui->clipboardView->selectionModel()->currentIndex();
}

void ClipboardPanel::removeItem(const QModelIndex &index)
{
    auto type = m_model->itemType(index);
    if(type == GResource::Type::UNDEFINED)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Undefined item type; operation not allowed"));
        return;
    }


    auto result = createRemoveDialog(index);

    if(result != QMessageBox::Yes)
    {
        return;
    }

    m_model->removeResource(index);
}

int ClipboardPanel::createRemoveDialog(const QModelIndex &index)
{
    QString questionStr;

    auto type = m_model->itemType(index);
    auto label = m_model->label(index);

    if(type == GResource::Type::ARTEFACT)
    {
        QString fromStr;

        // QModelIndex::parent will return the group model index
        auto parent = m_model->parent(index);

        if(parent.isValid())
        {
            auto groupName = m_model->label(parent);
            fromStr = tr("group \"%0\"").arg(groupName);
        }
        else
        {
            // If it is invalid (it has no parent) then it is in the clipboard
            fromStr = tr("the Clipboard");
        }

        // Remove artefact

        questionStr = tr("Are you sure that you want to remove "
                         "item \"%0\" from %1?")
                .arg(label, fromStr);
    }
    else if(type == GResource::Type::GROUP)
    {
        // Remove group
        questionStr = tr("Are you sure that you want to remove group "
                         "\"%0\"?").arg(label);
    }

    return QMessageBox::question(this, tr("GRAVITATE Dashboard"), questionStr);
}

void ClipboardPanel::moveItem(const QModelIndex &index, bool keepOriginal)
{
    auto m = m_model;
    // Open pop-up dialog for choosing a group

    QStringList options;
    QList<GroupId> groupIds;

    auto srcGroupIndex = m->parent(index);

    for(int i = 0; i < m->rowCount(); i++)
    {
        auto childIdx = m->index(i, 0);
        if(m->itemType(childIdx) != GResource::Type::GROUP)
        {
            continue;
        }

        if(srcGroupIndex == childIdx)
        {
            continue;
        }

        auto groupName = m->label(childIdx);
        options.append(groupName);
        auto groupId = GroupId::fromUri(m->containerId(childIdx));
        groupIds.append(groupId);
    }


    QInputDialog dialog;
    dialog.setComboBoxItems(options);

    bool ok;

    auto result = QInputDialog::getItem(this,
                          tr("Select a group"),
                          tr("Group:"),
                          options, 0, false, &ok);

    if(!ok)
    {
        return;
    }

    auto groupIndex = options.indexOf(result);
    auto selectedGroupId = groupIds[groupIndex];

    auto artefactId = m->artefactId(index);

    m->addToGroup(artefactId, selectedGroupId);

    if(!keepOriginal)
    {
        // Delete original item
        m->removeResource(index);
    }
}

void ClipboardPanel::onContextMenuRequested(const QPoint &point)
{
    auto index = ui->clipboardView->indexAt(point);
    auto type = m_model->itemType(index);

    if(!index.isValid())
    {
        return;
    }

    auto res = m_model->resource(index);
    auto pContextMenu = new QMenu(this);

    emit contextMenuOpen(pContextMenu, res);

    // AR: this separator divides view-specific actions from generic ones
    if(!pContextMenu->isEmpty())
    {
        pContextMenu->addSeparator();
    }

    // Remove (from clipboard/group)
    auto pRemoveAction = new QAction(tr("Remove"));
    connect(pRemoveAction, &QAction::triggered,
            [this, index] {
        removeItem(index);
    });

    pContextMenu->addAction(pRemoveAction);

    if(type == GResource::Type::ARTEFACT)
    {
        // Copy to group
        auto pCopyToGroup = new QAction(tr("Copy to group..."));
        connect(pCopyToGroup, &QAction::triggered,
                [this, index] {
            moveItem(index, true);
        });
        pContextMenu->addAction(pCopyToGroup);


        // Move to group
        auto pMoveToGroup = new QAction(tr("Move to group..."));
        connect(pMoveToGroup, &QAction::triggered,
                [this, index] {
            moveItem(index, false);
        });
        pContextMenu->addAction(pMoveToGroup);
    }
    auto globalPos = mapToGlobal(point);

    pContextMenu->exec( globalPos );

    delete pContextMenu;
    pContextMenu = Q_NULLPTR;

}

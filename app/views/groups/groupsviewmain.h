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
#ifndef GROUPSVIEWMAIN_H
#define GROUPSVIEWMAIN_H

#include <QWidget>
#include "abstractview.h"
#include "abstractdashboard.h"
#include "clipboardmodel.h"
#include "groupproxymodel.h"

class MainWindow;

namespace Ui {
class GroupsViewMain;
}

class GroupsViewMain : public AbstractView
{
    Q_OBJECT

public:
    explicit GroupsViewMain(AbstractDashboard *dashboard, QWidget *parent = 0);
    ~GroupsViewMain();

    void setClipboardModel(ClipboardModel *model);

    int numberOfItemsInGroup(const GroupId &groupId);

signals:
    void activeGroupChanged(const GroupId &id, const QString &name);

public slots:

    void onClipboardContextMenuOpen(QMenu *menu, const GResource &resource) override;

private:
    Ui::GroupsViewMain *ui;

    ClipboardModel *m_clipboardModel;
    GroupProxyModel *m_proxyModel;

    GroupId m_currentGroupId;
    QString m_currentGroupName;

    void selectCurrentGroup(const QModelIndex &index);

public:
    void initTools() override;
    QModelIndex selectedItemInContentView();

private slots:
    void onActionAddNote();
    void onActionCreateGroup();
    void onActionAddToClipboard();
};

#endif // GROUPSVIEWMAIN_H

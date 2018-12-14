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
#ifndef CLIPBOARDSYNCCOMMAND_H
#define CLIPBOARDSYNCCOMMAND_H

#include <QList>
#include <QModelIndex>
#include "clipboardmodel.h"
/**
 * @brief The ClipboardSyncCommand class abstracts the synchronization
 * operation of the clipboard, by returning the list of items that have changed
 * and their position in the tree.
 *
 * As the suffix says, this class follows the Command design pattern
 */

class ClipboardSyncCommand
{
public:
    virtual void begin() = 0;
    virtual void end() = 0;
    virtual void execute(const QList<ClipboardItem> &items, const QModelIndex &parent) = 0;
};

class FullSyncCommand : public ClipboardSyncCommand
{
public:
    FullSyncCommand(ClipboardModel *model);
    void begin() override;
    void end() override;
    void execute(const QList<ClipboardItem> &items, const QModelIndex &parent) override;

private:
    ClipboardModel *m_model;
};

class BufferedSyncCommand : public ClipboardSyncCommand
{
    enum Operation
    {
        INSERT,
        REMOVE,
    };

    struct BufferItem
    {
        Operation op;
        int pos;
    };

public:
    BufferedSyncCommand(ClipboardModel *model);

    void begin() override;
    void end() override;
    void execute(const QList<ClipboardItem> &items, const QModelIndex &parent) override;

private:
    ClipboardModel *m_model;
};

#endif // CLIPBOARDSYNCCOMMAND_H

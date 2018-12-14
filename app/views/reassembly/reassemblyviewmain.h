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
#ifndef REASSEMBLYVIEWMAIN_H
#define REASSEMBLYVIEWMAIN_H

#include <QWidget>
#include <QStandardItemModel>
#include "abstractview.h"
#include "abstractdashboard.h"
#include "reassemblyreader.h"
#include "core/mesh.h"

namespace Ui {
class ReassemblyViewMain;
}

class ReassemblyViewMain : public AbstractView
{
    Q_OBJECT

public:
    explicit ReassemblyViewMain(AbstractDashboard *dashboard, QWidget *parent = 0);
    ~ReassemblyViewMain();

    void initTools() override;

public slots:
    void onClipboardContextMenuOpen(QMenu *menu, const GResource &resource) override;

private:
    void initItemModels();

    void initDebugActions();

    void init3dContext();

    Ui::ReassemblyViewMain *ui;

    QStandardItemModel *m_workbenchModel;

    QStandardItemModel *m_candidatePaModel;

    Mesh *m_mesh1;

    Mesh *m_mesh2;

private slots:
    void onDebugLoadFile();

    void addToWorkbench(const ArtefactId &artefactId);

    void load3DModel(const ArtefactId &artefactId, bool append = false);

    void loadModelFromUrl(const QUrl &url, Mesh *mesh);

    void loadCandidatePairs(const QVector<PartialAssemblyPair> &candidatePairs);

    void onCandidatePairSelected(const QModelIndex &index);

    void startReAssemblyOnGroup(const QList<ArtefactId> &artefactList);

    void demoReassembly();
};

Q_DECLARE_METATYPE(PartialAssemblyPair)

#endif // REASSEMBLYVIEWMAIN_H

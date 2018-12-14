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
#include "partonomywidget.h"
#include "ui_partonomywidget.h"

#include <QDebug>
#include <QVariantList>

PartonomyWidget::PartonomyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartonomyWidget)
{
    ui->setupUi(this);

    // Initializes the selectionModel with a null model
    selectionModel = new QItemSelectionModel(nullptr, this);

    // Connects the events
    connect(selectionModel, &QItemSelectionModel::currentChanged,
            this, &PartonomyWidget::itemSelected);

    connect(this, &PartonomyWidget::itemSelected,
            this, [this] (const QModelIndex &index) {
        if(index.isValid())
        {
            Term t = m_thesaurusModel->term(index);
            updateInfoTable(t);
        }
    });

    // Disables List View tab since it has not been implemented
    // yet
    ui->viewTypesTab->setTabEnabled(1, false);
}

PartonomyWidget::~PartonomyWidget()
{
    delete ui;
}

void PartonomyWidget::setModel(ThesaurusModel *model)
{
    ui->partonomyTreeView->setModel(model);
    selectionModel->setModel(model);
    ui->partonomyTreeView->setSelectionModel(selectionModel);
    m_thesaurusModel = model;
}

QModelIndex PartonomyWidget::currentSelectedIndex()
{
    return selectionModel->currentIndex();
}

void PartonomyWidget::updateInfoTable(const Term &term)
{
    QTableWidget *table = ui->infoTable;

    QVariantList rows;
    QString label = QString::fromStdString( term.label );
    QString uri = QString::fromStdString( term.id );
    QString description = QString::fromStdString( term.description );
    rows.append( QStringList() << tr("Name") << label );
    rows.append( QStringList() << tr("Uri") << uri );
    rows.append( QStringList() << tr("Description") << description );

    table->clearContents();
    table->setRowCount(rows.size());
    table->setColumnCount(2); // Simply a key-value pair

    for(int i = 0; i < rows.size(); i++)
    {
        for(int j = 0; j < 2; j++)
        {
            QStringList row = rows[i].toStringList();
            QTableWidgetItem *item = new QTableWidgetItem(row[j]);
            table->setItem(i, j, item);
        }
    }
}

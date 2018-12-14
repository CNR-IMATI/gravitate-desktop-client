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
#include "browsedialog.h"
#include "ui_browsedialog.h"

#include "clipboardmodel.h"
#include "datasetfetchmodel.h"
#include <QString>
#include <QStringList>
#include <QProgressDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include "enums.h"

BrowseDialog::BrowseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrowseDialog),
    m_clipboardModel(Q_NULLPTR),
    m_datasetFetchModel(Q_NULLPTR)
{
    ui->setupUi(this);

    setWindowTitle(tr("Browse datasets"));
    ui->bulkDownloadOptions->setVisible(false);

    connect(ui->startDownloadButton, &QAbstractButton::pressed,
            this, &BrowseDialog::onStartDownload);

    m_addToMenu = createAddToMenu();

    ui->addArtefactsButton->setMenu(m_addToMenu);
}

BrowseDialog::~BrowseDialog()
{
    delete ui;
}

QList<ArtefactId> BrowseDialog::selectedArtefacts()
{
    if(m_datasetFetchModel == nullptr)
    {
        qCritical() << "[ERROR] The DatasetFetchModel has not been initialized";
        return QList<ArtefactId>();
    }
    return m_datasetFetchModel->selectedArtefacts();
}

QList<MultiResProperty> BrowseDialog::selectedProperties()
{
    QList<MultiResProperty> l;

    l.append(MultiResProperty::MESH);

    if(ui->meanCurvCheckBox->isChecked())
        l.append(MultiResProperty::MEAN_CURVATURE);
    if(ui->shapeIndexCheckBox->isChecked())
        l.append(MultiResProperty::SHAPE_INDEX);
    if(ui->sdfCheckBox->isChecked())
        l.append(MultiResProperty::SDF);
    if(ui->labCheckBox->isChecked())
        l.append(MultiResProperty::LAB_COLOR);

    return l;
}

QList<ModelResolution> BrowseDialog::selectedResolutions()
{
    QList<ModelResolution> l;
    if(ui->resOriginalCheckBox->isChecked())
        l.append(ModelResolution::RES_ORIGINAL);
    if(ui->resCleanCheckBox->isChecked())
        l.append(ModelResolution::RES_FULL);
    if(ui->res1MCheckBox->isChecked())
        l.append(ModelResolution::RES_1M);
    if(ui->res100KCheckBox->isChecked())
        l.append(ModelResolution::RES_100K);
    if(ui->res50CheckBox->isChecked())
        l.append(ModelResolution::RES_50K);

    return l;
}

void BrowseDialog::setClipboardModel(ClipboardModel *model)
{
    m_clipboardModel = model;
}

void BrowseDialog::setDatasetFetchModel(DatasetFetchModel *model)
{
    m_datasetFetchModel = model;
    ui->artefactTableView->setModel(m_datasetFetchModel);

    connect(ui->selectAllCheckBox, &QAbstractButton::toggled,
            m_datasetFetchModel, &DatasetFetchModel::setAllSelected);

    connect(m_datasetFetchModel, &QAbstractItemModel::rowsInserted,
            [this] {
        ui->artefactTableView->resizeRowsToContents();
    });

}

void BrowseDialog::addDataset(const QString &name, const QStringList &artefacts)
{
    ui->datasetSelector->addItem(name, artefacts);

    connect(ui->datasetSelector, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &BrowseDialog::onCurrentIndexChanged, Qt::UniqueConnection);

}

void BrowseDialog::onCurrentIndexChanged(const QString &name)
{
    auto data = ui->datasetSelector->currentData();
    QStringList list = data.toStringList();

    QList<ArtefactId> artefacts;
    for(auto s : list)
    {
        artefacts.append(ArtefactId::fromUri(QUrl(s)));
    }

    m_datasetFetchModel->loadArtefactList(artefacts);

    ui->artefactTableView->update();
}

void BrowseDialog::onStartDownload()
{
    auto artefacts = selectedArtefacts();
    auto properties = selectedProperties();
    auto resolutions = selectedResolutions();

    QProgressDialog dialog(this);

    connect(m_datasetFetchModel, &DatasetFetchModel::completedFileProgress,
            [&dialog] (int completed, int total, const QString &fileName){
        dialog.setMaximum(total);
        dialog.setValue(completed);
        dialog.setLabelText(tr("(%1/%2) %3")
                        .arg(completed)
                        .arg(total)
                        .arg(fileName));
    });

    QEventLoop loop;

    connect(m_datasetFetchModel, &DatasetFetchModel::downloadFinished,
            &loop, &QEventLoop::quit);

    connect(m_datasetFetchModel, &DatasetFetchModel::downloadFinished,
            [this] (int completed){
        QMessageBox::information(this,
                                 tr("Download completed"),
                                 tr("%1 files have been downloaded").arg(completed));
    });

    m_datasetFetchModel->fetchData(artefacts, resolutions, properties);

    loop.exec();
}

void BrowseDialog::onAddSelectedToExistingGroup()
{
    auto artefacts = selectedArtefacts();
    auto groups = m_clipboardModel->groups();

    QStringList options;
    for(auto g : groups)
    {
        options << g.label;
    }

    bool ok;
    auto selectedOption = QInputDialog::getItem(this,
                                                tr("Add selected items to group"),
                                                tr("Select a group"),
                                                options,
                                                0,
                                                false,
                                                &ok
                                                );
    // If the user has canceled the action, exit
    if(!ok)
    {
        return;
    }

    auto index = options.indexOf(selectedOption);

    auto selectedGroup = groups.at(index);

    auto artefactId = selectedGroup.artefactId;

    if(!artefactId.uri.isValid())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Invalid selected group URI: ")
                              .arg(selectedGroup.artefactId.toString())
                              );
    }

    for(auto id : artefacts)
    {
        qDebug() << "Added" << id.toString() << "to group" << selectedGroup.label;
        m_clipboardModel->addToGroup(artefactId, selectedGroup.groupId);
    }
}

QMenu *BrowseDialog::createAddToMenu()
{
    auto menu_addTo = new QMenu();
    auto action_addToClipboard = menu_addTo->addAction(tr("Add to clipboard"));
    menu_addTo->addAction(action_addToClipboard);

    connect(action_addToClipboard, &QAction::triggered,
            [this] {
        if(m_clipboardModel == nullptr)
        {
            qDebug() << "[ERROR] The ClipboardModel is not initialized";
            return;
        }

        for(auto id : selectedArtefacts())
        {
            m_clipboardModel->addToClipboard(id);
        }
    });

    auto action_addToGroup = new QAction(tr("Add to existing group"));
    menu_addTo->addAction(action_addToGroup);
    connect(action_addToGroup, &QAction::triggered,
            this, &BrowseDialog::onAddSelectedToExistingGroup);

    auto action_addToNewGroup = menu_addTo->addAction(tr("Add to new group..."));
    menu_addTo->addAction(action_addToNewGroup);
    action_addToNewGroup->setEnabled(false);

    return menu_addTo;
}

void BrowseDialog::showEvent(QShowEvent *event)
{
    onCurrentIndexChanged(ui->datasetSelector->currentText());
}

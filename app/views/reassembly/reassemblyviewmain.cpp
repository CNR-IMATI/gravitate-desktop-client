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
#include "reassemblyviewmain.h"
#include "ui_reassemblyviewmain.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QItemSelectionModel>

#include "gravitatebackend.h"
#include "gravitatesparql.h"
#include "abstractdownloader.h"
#include "downloadprogressdialog.h"
#include "abstractdashboard.h"
#include <QVector>


ReassemblyViewMain::ReassemblyViewMain(AbstractDashboard *dashboard, QWidget *parent) :
    AbstractView(dashboard, parent),
    ui(new Ui::ReassemblyViewMain)
{
    ui->setupUi(this);

    initItemModels();

    initDebugActions();

    init3dContext();
}

ReassemblyViewMain::~ReassemblyViewMain()
{
    delete ui;
}

void ReassemblyViewMain::initItemModels()
{
    m_workbenchModel = new QStandardItemModel(this);

    m_candidatePaModel = new QStandardItemModel(this);
    ui->canditatePaView->setModel(m_candidatePaModel);

    auto candidatePaSelectionModel = ui->canditatePaView->selectionModel();

    connect(candidatePaSelectionModel, &QItemSelectionModel::currentChanged,
            this, &ReassemblyViewMain::onCandidatePairSelected);
}

void ReassemblyViewMain::initDebugActions()
{
    connect(ui->actionLoad_ReAssembly_File, &QAction::triggered,
            this, &ReassemblyViewMain::onDebugLoadFile);

    connect(ui->actionReAssembly_Demo, &QAction::triggered,
            this, &ReassemblyViewMain::demoReassembly);
}

void ReassemblyViewMain::init3dContext()
{
    auto viewportWidget = ui->reassembly3DWidget->meshWidget();

    m_mesh1 = new Mesh(viewportWidget);
    m_mesh2 = new Mesh(viewportWidget);
}

void ReassemblyViewMain::onDebugLoadFile()
{
    auto file = QFileDialog::getOpenFileName(this,
                                 tr("Select a file"),
                                 QString(),
                                 QString("ReAssembly file (*.json)")
                                );
    if(file.isEmpty())
    {
        return;
    }

    ReAssemblyReader reader;
    if(!reader.readJson(file.toStdString()))
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("An error has occurred while reading the "
                                 "ReAssembly file"));
        return;
    }

    auto candidatePairs = QVector<PartialAssemblyPair>::fromStdVector(reader.pairs());

    loadCandidatePairs(candidatePairs);
}

void ReassemblyViewMain::initTools()
{
    addDefaultTools();

    toolSet()
        //.addToolPlaceholder("ReAssembly")
        .addTool(ui->actionReAssembly_Demo)
        .addToolPlaceholder("Adjust\nfragment\nposition\nmanually")
        .beginMenu("Debug")
            .addTool(ui->actionLoad_ReAssembly_File)
        .endMenu();
}


void ReassemblyViewMain::onClipboardContextMenuOpen(QMenu *menu, const GResource &resource)
{
    if(resource.type == GResource::Type::ARTEFACT)
    {
        auto actionAddToWorkbench = new QAction(tr("Add to Workbench"));
        connect(actionAddToWorkbench, &QAction::triggered,
                [this, resource] {
           addToWorkbench(resource.artefactId);
        });
        menu->addAction(actionAddToWorkbench);

        auto actionLoad3DModel = new QAction(tr("Load 3D Model"));
        connect(actionLoad3DModel, &QAction::triggered,
                [this, resource] {
           load3DModel(resource.artefactId);
        });
        menu->addAction(actionLoad3DModel);

    }

    if(resource.type == GResource::Type::GROUP)
    {
        auto startReAssembly = new QAction(tr("Start ReAssembly"));
        menu->addAction(startReAssembly);

        connect(startReAssembly, &QAction::triggered,
                [this, resource] {
            auto clipboard = m_dashboard->clipboardModel();

            auto artefacts = clipboard->getArtefacts(resource.groupId);

            startReAssemblyOnGroup(artefacts);


        });

    }
}

void ReassemblyViewMain::addToWorkbench(const ArtefactId &artefactId)
{
}

void ReassemblyViewMain::load3DModel(const ArtefactId &artefactId, bool append)
{

}

void ReassemblyViewMain::loadModelFromUrl(const QUrl &url, Mesh *mesh)
{
    if(mesh == nullptr)
    {
        qCritical() << "Mesh object has not been initialized";
        return;
    }

    auto dl = m_dashboard->downloader();
    DownloadProgressDialog dialog(dl, this);

    auto file = dl->getLocalPath(url);

    mesh->loadModel(file);

}

void ReassemblyViewMain::loadCandidatePairs(const QVector<PartialAssemblyPair> &candidatePairs)
{
    m_candidatePaModel->clear();

    for(auto pair : candidatePairs)
    {
        QUrl url1(QString::fromStdString(pair.first.fileName));
        QUrl url2(QString::fromStdString(pair.second.fileName));

        auto label = url1.fileName() + " + " + url2.fileName();
        auto item = new QStandardItem(label);

        item->setData( QVariant::fromValue(pair) );

        m_candidatePaModel->appendRow(item);
    }
}

void ReassemblyViewMain::onCandidatePairSelected(const QModelIndex &index)
{
    // Get data from the model
    auto item = m_candidatePaModel->itemFromIndex(index);
    auto pair = item->data().value<PartialAssemblyPair>();

    PartialAssemblyFragment fragments[2] = {pair.first, pair.second};
    Mesh* meshes[2] = { m_mesh1, m_mesh2 };

    for(int i = 0; i < 2; i++)
    {
        auto mesh = meshes[i];
        auto fragment = fragments[i];

        auto url = QUrl(QString::fromStdString(fragments[i].fileName));
        loadModelFromUrl(url, mesh);
        auto transform = fragment.getAffineTransformMatrix();
        mesh->setAffineTransform(transform);
    }

    ui->reassembly3DWidget->resetCamera();


//    // Get URL of the 2 models composing the PA
//    QUrl url1(pair.first.fileName);
//    loadModelFromUrl(url1, m_mesh1);
//    auto mat1 = pair.first.getAffineTransformMatrix();

//    QUrl url2(pair.second.fileName);
//    loadModelFromUrl(url2, m_mesh2);
//    auto mat2 = pair.second.getAffineTransformMatrix();

    // TODO: Apply transformations
}

void ReassemblyViewMain::startReAssemblyOnGroup(const QList<ArtefactId> &artefactList)
{
//    auto backend = m_dashboard->backendProvider();
//    auto queryString = GravitateSparql::metadataQuery(artefactList);
//    QNetworkReply *reply = backend->sparqlQuery(queryString);

//    connect(reply, &QNetworkReply::finished,
//        [=] {
//        //using namespace GravitateRepo;

//        if(reply->error() != QNetworkReply::NoError)
//        {
//            reply->deleteLater();
//            qDebug() << "Error:" << reply->errorString();
//            return;
//        }

//        SparqlResults results;
//        results.readJson(reply->readAll());

//        while(results.next())
//        {
//            auto file = results.value("file").toUrl();
//            auto type = results.value("type").toString();

//            if(type == "http://gravitate-project.eu/id/object/3D_model_file/clean")
//            {
//                qDebug() << file.toString();
//            }
//        }
//    });
}

void ReassemblyViewMain::demoReassembly()
{
    auto tempFile = QTemporaryFile::createNativeFile(":/demo/expectedFragmentsOut_withUrl.json");
    auto stdFile = tempFile->fileName().toStdString();

    ReAssemblyReader reader;
    if(!reader.readJson(stdFile))
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("An error has occurred while reading the "
                                 "ReAssembly file"));
        return;
    }

    auto candidatePairs = QVector<PartialAssemblyPair>::fromStdVector(reader.pairs());

    loadCandidatePairs(candidatePairs);
}

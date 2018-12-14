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
#include "standalonemeshviewer.h"
#include "ui_standalonemeshviewer.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include "scalarfieldreader.h"
#include "core/coordinateaxes.h"
#include "vtkColorSeries.h"
#include "vtkStdString.h"
#include "vtkSmartPointer.h"
#include "enums.h"
#include <QInputDialog>
#include <QStringList>
#include "tweakparamwidget.h"
#include "property/transferfunctionfactory.h"
#include "boundingbox.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkNew.h>
#include <vtkInteractorStyleDrawPolygon.h>
#include <vtkVector.h>
#include <vtkHardwareSelector.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelection.h>
#include "skostreeparsersord.h"
#include <QTemporaryFile>
#include <QColorDialog>
#include <QStandardPaths>

#include <QMessageBox>

#include <vector>

#include "gravitaterepoutils.h"
#include "math_utils.h"
#include "unitofmeasure.h"
#include "annotationhelper.h"

void StandaloneMeshViewer::initSelection()
{
    ui->actionLoad_Selection->setEnabled(false);
    ui->actionSave_Selection->setEnabled(false);

    connect(ui->actionLoad_Selection, &QAction::triggered,
            this, &StandaloneMeshViewer::onSelectionLoad);

    connect(ui->actionSave_Selection, &QAction::triggered,
            this, &StandaloneMeshViewer::onSelectionSave);

    connect(s_modelLoaded, &QState::entered,
            [this] {
        ui->actionLoad_Selection->setEnabled(true);
        ui->actionSave_Selection->setEnabled(true);
    });


}

StandaloneMeshViewer::StandaloneMeshViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StandaloneMeshViewer)
{
    ui->setupUi(this);

    m_mesh = new Mesh(ui->viewerWidget);

    connect(m_mesh, &Mesh::modelLoaded,
            this, &StandaloneMeshViewer::onMeshLoaded);

    connect(m_mesh, &Mesh::propertyLoaded,
            this, &StandaloneMeshViewer::onPropertyLoaded);


    initStateMachine();

    initDebugTab();

    initComboBox();

    CoordinateAxes *axes = new CoordinateAxes(ui->viewerWidget);

    initPropertyLoading();

//    auto interactor = ui->viewerWidget->renderWindowInteractor();

//    m_vertexPickerStyle =
//            vtkSmartPointer<InteractorStyleVertexPicker>::New();
//    interactor->SetInteractorStyle(m_vertexPickerStyle);

    initAnnotation();

    initFaceting();

    initSelection();

    connect(ui->actionLoad_Vocabulary, &QAction::triggered,
            this, &StandaloneMeshViewer::onCustomVocabularyLoad);
}

StandaloneMeshViewer::~StandaloneMeshViewer()
{
    delete ui;
}

bool StandaloneMeshViewer::isTermSelected()
{
    return currentSelectedPartonomyTermIndex().isValid();
}

Term StandaloneMeshViewer::currentSelectedPartonomyTerm()
{
    auto index = currentSelectedPartonomyTermIndex();
    return m_thesaurusModel->term(index);
}

void StandaloneMeshViewer::initDebugTab()
{
    // Initialization of the tweak parameters

    auto group = new QButtonGroup(this);
    group->addButton(ui->windowLevelButton);
    group->addButton(ui->minMaxButton);
    ui->windowLevelButton->toggle();

    ui->tweakParams->addParam("Window",     0.0, 5.0, 1.0);
    ui->tweakParams->addParam("Level", -2.0, 2.0, 0.0);
    ui->tweakParams->addParam("Contrast",  0.0, 1.0, 0.95);

    auto min = 0.0, max = 100.0;

    ui->minMaxParams->addParam("Min",  min, max, min);
    ui->minMaxParams->addParam("Max",  min, max, max);

    connect(ui->applyButton, &QAbstractButton::clicked,
            this, &StandaloneMeshViewer::onPropertyCustomColorApplied);

    connect(m_mesh, &Mesh::propertyLoaded,
            [this] {
        ui->groupBox->setEnabled(true);
    });
}

void StandaloneMeshViewer::initPropertyLoading()
{
    m_coloring = new PropertyColoring(m_mesh, ui->viewerWidget);

    ui->actionLoad_Geometric_Property->setEnabled(false);

    connect(s_modelLoaded, &QState::entered,
            [this] {
       ui->actionLoad_Geometric_Property->setEnabled(true);
    });

    m_wireframeBox = new WireframeBox(ui->viewerWidget);


    // Unit of measure selector
    m_unitOfMeasureComboBox = new QComboBox(this);

    auto units = QStringList() << "m" << "cm" << "mm";

    m_unitOfMeasureComboBox->addItems(units);

    auto unitLabel = new QLabel(tr("Unit of measure "));

    ui->toolBar->addWidget(unitLabel);
    ui->toolBar->addWidget(m_unitOfMeasureComboBox);
}

void StandaloneMeshViewer::initComboBox()
{
    vtkSmartPointer<vtkColorSeries> series
            = vtkSmartPointer<vtkColorSeries>::New();
    int maxSchemeId = series->GetNumberOfColorSchemes();
    for(int i = 0; i < maxSchemeId; i++)
    {
        series->SetColorScheme(i);
        vtkStdString s = series->GetColorSchemeName();
        QString qString = QString::fromStdString(s);
        ui->colorSeriesSelector->addItem(qString, i);
    }
}

void StandaloneMeshViewer::loadPartonomy(const QString &filename)
{
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Could not load file %1").arg(filename)
        );
    }

    SkosTreeParserSord parser;
    QUrl root("http://www.cidoc-crm.org/cidoc-crm/E55_Type");

    auto json = parser.parseToJson(filename, root);

    if(json.size() == 0)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Error in parsing the RDF graph")
                              );
        return;
    }

    m_thesaurusModel = new ThesaurusModel(this);

    bool ok = m_thesaurusModel->readJson(json);
    if(!ok)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("An error has occurred in the vocabulary initialization"));
        return;
    }

    ui->partonomyWidget->setModel(m_thesaurusModel);
}

qudt::Unit StandaloneMeshViewer::currentUnitOfMeasure()
{
    auto unitStr = m_unitOfMeasureComboBox->currentText();

    if(unitStr == "m")
        return qudt::METER;
    if(unitStr == "cm")
        return qudt::CENTIMETER;
    if(unitStr == "mm")
        return qudt::MILLIMETER;

    return qudt::UNKNOWN;
}

QModelIndex StandaloneMeshViewer::currentSelectedPartonomyTermIndex()
{
    return ui->partonomyWidget->currentSelectedIndex();
}

void StandaloneMeshViewer::onAnnotationSave()
{
    // Retrieve URI
    auto selectedIndex = ui->partonomyWidget->currentSelectedIndex();
    if(!selectedIndex.isValid())
    {
        qCritical() << "Please select a valid partonomy term";
        return;
    }

    auto term = m_thesaurusModel->term(selectedIndex);
    QUrl partUri = QUrl(QString::fromStdString(term.id));

    if(!partUri.isValid())
    {
        qCritical() << "Selected part has not a valid URI:" << partUri.toString();
        return;
    }

    qDebug() << "Selected part" << partUri.toString();

    // Retrieve selected part indices
    auto indices = m_selectionTool->selectedVertices();

    if(indices.isEmpty())
    {
        qCritical() << "The selection is empty";
        return;
    }

    qDebug() << "Selection has" << indices.size() << "elements";

    // Gather information and save
    PartAnnotation annotation;

    auto color = QColor(Qt::green);
    auto label = term.label;

    annotation.uri = partUri.toString().toStdString();
    annotation.label = label;
    annotation.color.r = color.red();
    annotation.color.g = color.green();
    annotation.color.b = color.blue();
    annotation.indices = indices.toStdVector();

    QUrl modelUri = QUrl::fromLocalFile(m_currentFile);



//    if(m_annotationModel->indexFile().isEmpty())
//    {
//        QFileInfo info(m_currentFile);

//        QDir appdataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//        QString indexFile = QDir(appdataDir).filePath("annotations/" + info.baseName() + ".json");
//        qDebug() << "Annotation index:" << indexFile;

//        m_annotationModel->create(indexFile);
//    }

    if(!m_annotationModel->saveAnnotation(modelUri, annotation))
    {
        qCritical() << "Could not save annotation";
        return;
    }

    QMessageBox::information(this,
                             tr("Info"),
                             tr("Annotation %1 saved successfully")
                             .arg(partUri.toString())
                             );


    emit annotationAdded(annotation);
}

void StandaloneMeshViewer::initAnnotation()
{
    m_selectionTool = new SelectionTool(m_mesh, ui->viewerWidget);

    m_annotationTool = new AnnotationTool(m_mesh, ui->viewerWidget);

    loadPartonomy(":/rdf/partonomy.ttl");

    connect(m_annotationTool, &AnnotationTool::annotationRequested,
            this, &StandaloneMeshViewer::openAnnotationDialog);

    m_annotationModel = new LocalAnnotationModel(this);

    ui->currentAnnotationsView->setModel(m_annotationModel);

    connect(this, &StandaloneMeshViewer::annotationAdded,
            [this] (const PartAnnotation &pa) {
        auto label = QString::fromStdString(pa.label);
        auto indices = QVector<unsigned int>::fromStdVector(pa.indices);
        auto color = QColor(pa.color.r, pa.color.g, pa.color.b);

        m_annotationTool->addAnnotation(label, indices, color);
    });

    connect(this, &StandaloneMeshViewer::annotationAdded,
            m_selectionTool, &SelectionTool::clearSelection);

    ui->actionLoad_Annotations->setEnabled(false);
//    m_selectionTool->setEnabled(false);

    connect(s_modelLoaded, &QState::entered,
            [this] {
        ui->actionLoad_Annotations->setEnabled(true);
        m_selectionTool->setEnabled(true);
        m_annotationTool->setEnabled(true);
        ui->annotationButtonsLayout->setEnabled(true);

        m_annotationTool->clearAnnotations();
        m_annotationModel->clearAnnotations();
    });

    connect(ui->removeAnnotationButton, &QPushButton::clicked,
            [this] {

        auto reply = QMessageBox::question(this,
                              tr("Delete an annotation"),
                              tr("Are you sure that you want to "
                                "delete the selected annotation?"));
        if(reply != QMessageBox::Yes)
        {
            return;
        }

        auto selectionModel = ui->currentAnnotationsView->selectionModel();
        auto index = selectionModel->currentIndex();

        m_annotationModel->removeAnnotation(index);

        // Remove the corresponding actor in the annotationtool
        auto id = index.row();
        m_annotationTool->removeAnnotation(id);
    });
}

void StandaloneMeshViewer::initFaceting()
{
    m_facetOverlay = new FacetOverlay(m_mesh, ui->viewerWidget);

    ui->actionLoad_Facets->setEnabled(false);

    connect(s_modelLoaded, &QState::entered,
            [this] {
       ui->actionLoad_Facets->setEnabled(true);
    });
}

void StandaloneMeshViewer::initStateMachine()
{
    m_stateMachine = new QStateMachine(this);

    s_initial = new QState();
    s_modelLoaded = new QState();

    s_initial->addTransition(m_mesh, &Mesh::modelLoaded, s_modelLoaded);

    // This state transition is necessary to handle the case of when a model
    // has been already already loaded
    s_modelLoaded->addTransition(m_mesh, &Mesh::modelLoaded, s_modelLoaded);

    m_stateMachine->addState(s_initial);
    m_stateMachine->addState(s_modelLoaded);
    m_stateMachine->setInitialState(s_initial);

    m_stateMachine->start();

}

void StandaloneMeshViewer::on_actionLoad_Mesh_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Mesh File"), "",
                                                tr("PLY files (*.ply);;OBJ files(*.obj)"));
    if(file.isNull())
    {
        return;
    }

    m_currentFile = file;
    m_mesh->loadModel(file);
}

void StandaloneMeshViewer::on_actionLoad_Geometric_Property_triggered()
{
    if(!s_modelLoaded->active())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Please load a 3D model first"));
        return;
    }

    QString file = QFileDialog::getOpenFileName(this, tr("Open Property File"), "",
                                                tr("Property files (*.txt);;All files (*.*)"));


    // if the user has pressed the "Cancel" button,
    // getOpenFileName returns a null string
    if(file.isNull())
    {
        return;
    }

    PropertyType type = GravitateRepoUtils::propertyType(file);
    if(type == PropertyType::UNDEFINED)
    {
        type = GravitateRepoUtils::getSelectPropertyType();
    }

    MeshPropertyReader reader(type);

    if(!reader.readTxt(file))
    {
        qDebug() << tr("Error reading file %1").arg(file);
    }

    QVector<double> data = reader.data();



    if(type == PropertyType::SDF)
    {
        auto sourceUnit = currentUnitOfMeasure();
        auto targetUnit = qudt::CENTIMETER;

        data = convertUnitOfMeasure(data, sourceUnit, targetUnit);
    }

    QString name = GravitateRepoUtils::propertyName(type);

    TransferFunctionFactory factory;
    vtkSmartPointer<vtkScalarsToColors> lut =  factory.createLut(type, m_mesh);

    auto target = GravitateRepoUtils::propertyTarget(type);

    if(target == PropertyTarget::VERTICES)
    {
        m_mesh->loadVertexProperty(name, data);
    }
    else if(target == PropertyTarget::TRIANGLES)
    {
        m_mesh->loadCellProperty(name, data);
    }

    m_coloring->addProperty(type, lut);
    m_coloring->selectProperty(type);

    ui->colorSeriesSelector->setEnabled(true);
}

void StandaloneMeshViewer::onMeshLoaded()
{
    QFileInfo info(m_currentFile);

    setWindowTitle(info.fileName());

    auto infoString = tr("Vertices: %1; Triangles: %2")
            .arg(m_mesh->vertexCount())
            .arg(m_mesh->triangleCount());

    ui->viewerWidget->setInfo(infoString);

//    m_vertexPickerStyle->SetPolyData(m_mesh->polyData());
}

void StandaloneMeshViewer::onPropertyLoaded()
{
    m_propertyLoaded = true;

    ui->applyButton->setEnabled(true);
}

void StandaloneMeshViewer::onPropertyCustomColorApplied()
{
    TransferFunctionFactory factory;

    vtkSmartPointer<vtkScalarsToColors> lut;

    auto seriesName = ui->colorSeriesSelector->currentText();
    auto scheme = vtkSmartPointer<vtkColorSeries>::New();
    auto invert = ui->invertScaleCheckBox->isChecked();
    scheme->SetColorSchemeByName(seriesName.toStdString().c_str());

    if(ui->windowLevelButton->isChecked())
    {
        auto window = ui->tweakParams->value("Window");
        auto level = ui->tweakParams->value("Level");
        auto contrast = ui->tweakParams->value("Contrast");
        lut = factory.makeCtfDiverging(scheme, window, level, contrast, invert);
    }
    else if(ui->minMaxButton->isChecked())
    {
        auto min = ui->minMaxParams->value("Min");
        auto max = ui->minMaxParams->value("Max");
        lut = factory.makeCtf(scheme, min, max, invert);
    }

    m_coloring->applyPropertyColoring(lut);

    // Get the current property target

    //auto customName =  "Custom";
    //    m_coloring->addProperty(lut, customName, PropertyTarget::VERTICES);
    //    m_coloring->selectProperty(customName);
    ui->viewerWidget->update();
}

void StandaloneMeshViewer::on_actionLoad_Annotations_triggered()
{
    if(!s_modelLoaded->active())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Please load a 3D model first"));
        return;
    }

    AnnotationHelper helper(m_selectionTool, m_annotationTool, m_annotationModel, this);

    auto url = QUrl::fromLocalFile(m_currentFile);

    helper.loadAnnotations(url);
}

void StandaloneMeshViewer::on_actionLoad_Facets_triggered()
{
    if(!s_modelLoaded->active())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Please load a 3D model first"));
        return;
    }

    QString file = QFileDialog::getOpenFileName(this, tr("Open Mesh File"), "",
                                                tr("FCT files (*.fct)"));
    if(file.isNull())
    {
        return;
    }

    if(!m_facetOverlay->loadFct(file))
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Error parsing the faceting file"));
        return;
    }
}

void StandaloneMeshViewer::on_actionLoad_Bounding_Box_triggered()
{
    // Load JSON file containing the BB vertices
//    if(!s_modelLoaded->active())
//    {
//        QMessageBox::critical(this,
//                              tr("Error"),
//                              tr("Please load a 3D model first"));
//        return;
//    }

    QString file = QFileDialog::getOpenFileName(this, tr("Select Bounding Box file"), "",
                                                tr("JSON files (*.json)"));

    if(file.isEmpty())
    {
       return;
    }

    BoundingBox bb;

    if(!bb.readJson(file.toStdString()))
    {
        qCritical() << "Error reading file" << file;
        return;
    }

    // FIXME: use qudt data type as "data" for the combo box
    auto unitString = m_unitOfMeasureComboBox->currentText();    

    m_wireframeBox->apply(bb, qudt::UNKNOWN);
}

void StandaloneMeshViewer::onSelectionSave()
{
    auto file = QFileDialog::getSaveFileName(this,
                                 tr("Save selection to file"),
                                 "selection.txt",
                                 tr("Text files (*.txt)")
                                 );
    if(file.isEmpty())
        return;

    m_selectionTool->saveSelection(file);
}

void StandaloneMeshViewer::onSelectionLoad()
{
    auto file = QFileDialog::getOpenFileName(this,
                                 tr("Select file containing a selection"),
                                 "",
                                 tr("Text files (*.txt)")
                                 );
    if(file.isEmpty())
        return;

    m_selectionTool->loadSelection(file);
}

void StandaloneMeshViewer::openAnnotationDialog()
{
    if(!isTermSelected())
    {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Please select a term first"));
        return;
    }

    auto modelUrl = QUrl::fromLocalFile(m_currentFile);
    auto term = currentSelectedPartonomyTerm();

    AnnotationHelper helper(m_selectionTool, m_annotationTool, m_annotationModel, this);
    helper.onAnnotationRequest(modelUrl, term);
}

void StandaloneMeshViewer::onCustomVocabularyLoad()
{
    auto file = QFileDialog::getOpenFileName(this,
                                 tr("Select a vocabulary"),
                                 "",
                                 tr("Turtle (*.ttl)")
                                 );

    if(file.isEmpty())
        return;

    loadPartonomy(file);
}

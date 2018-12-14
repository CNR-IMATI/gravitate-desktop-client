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
#include "fragmentviewmain.h"
#include "ui_fragmentviewmain.h"
#include "partonomywidget.h"
#include "metadata3drequest.h"
#include "gravitatebackend.h"
#include <QDebug>
#include <QFile>
#include <QTemporaryFile>
#include <QProgressDialog>
#include <QEventLoop>
#include <QFileDialog>
#include <QDir>
#include <QPushButton>
#include <QList>
#include <QMessageBox>
#include <QScopedPointer>
#include <QElapsedTimer>

#include <QVariantMap>
#include <QJsonDocument>
#include "annotationdialog.h"
#include <QStandardPaths>
#include "partannotation.h"
#include <QVector>
#include "gravitaterepoutils.h"
#include "skostreeparsersord.h"
#include "gravitatemetadata.h"
#include "helphandler.h"
#include <exception>

#include "demoutils.h"

#include "parameterselectiondialog.h"

#include "boundingbox.h"
#include "facet.h"
#include "scalarfieldreader.h"
#include "property/propertyrendering.h"
#include "annotationhelper.h"
#include "artefactloadhelper.h"
#include "backendfactorybase.h"
#include "dashboardsdfpropertyrendering.h"
#include "metadatachprovider.h"
#include "qxtsignalwaiter.h"

FragmentViewMain::FragmentViewMain(AbstractDashboard *dashboard, QWidget *parent) :
    AbstractView(dashboard, parent),
    ui(new Ui::FragmentViewMain),
    m_annotationModel(nullptr)
{
    ui->setupUi(this);

    initItemWidget();

    setupSignals();

    initMetadataModel();

    initProperties();

    initAnnotation();

    initFeatureRecognition();

    initDemoFeatureRecognition();

    initResolutionSelector();

    initButtonStates();

    // Debug actions
    connect(ui->actionLoad_file_from_disk, &QAction::triggered,
            this, &FragmentViewMain::onDebugLoadFileFromDisk);
}

FragmentViewMain::~FragmentViewMain()
{
    delete ui;
}

void FragmentViewMain::initTools()
{
    addDefaultTools();

    auto loadVocabMenu = new QMenu(tr("Load\nVocabulary"));
    auto geomCharMenu = new QMenu(tr("Geometric\nCharacterization"));

    toolSet().beginMenu(loadVocabMenu)
        .addTool(ui->actionLoad_CHAP)
    .endMenu()
    .beginMenu(geomCharMenu)
        .addTool(ui->actionLoad_Mean_Curvature)
        .addTool(ui->actionLoad_Shape_Index)
        .addTool(ui->actionLoad_Colorimetric)
        .addTool(ui->actionLoad_SDF)
        .addTool(ui->actionLoadMinimal_Bounding_Box)
    .endMenu()
    .addTool(ui->actionAutomatic_Feature_Recognition)
    .addTool(ui->actionFeature_Extraction_Demo)
    .addTool(ui->actionAnnotation_Mode)
    .addTool(ui->actionLoadFacets)
    .beginMenu("Debug")
        .addTool(ui->actionLoad_file_from_disk)
    .endMenu();

    ui->actionAutomatic_Feature_Recognition->setText(tr("Geometric\nFeature\nExtraction"));

    auto handler = m_dashboard->helpHandler();

    if(handler == nullptr)
    {
        qCritical() << "HelpHandler not initialized" << Q_FUNC_INFO;
        return;
    }

    m_resolutionSelector->setWhatsThis(handler->getText("resolution-selector"));
    m_resolutionSelector->setToolTip(tr("Resolution Selector"));
    m_propertyToolbar->initHelp(handler);

    auto loadVocabMenuButton = toolSet().getToolButton(loadVocabMenu);
    auto geomCharMenuButton = toolSet().getToolButton(geomCharMenu);

    if(loadVocabMenuButton != nullptr)
    {
        loadVocabMenuButton->setWhatsThis(
            handler->getText("menu-load-vocabulary")
        );
    }

    if(geomCharMenuButton != nullptr)
    {
        geomCharMenuButton->setWhatsThis(
            handler->getText("menu-geom-char")
        );
    }

    // Help initialization

    ui->actionFeature_Extraction_Demo->setWhatsThis(
        handler->getText("tool-feature-recognition")
    );

    ui->actionAutomatic_Feature_Recognition->setWhatsThis(
        handler->getText("tool-feature-recognition")
    );

    ui->actionAnnotation_Mode->setWhatsThis(
        handler->getText("tool-annotation-mode")
    );

    ui->actionLoadFacets->setWhatsThis(
        handler->getText("tool-load-facets")
    );
}

void FragmentViewMain::initButtonStates()
{
    auto initialState = ui->itemWidget->initialState();
    auto loadedState = ui->itemWidget->modelLoadedState();

    auto setEnabledModelDependentButtons = [this] (bool enabled){

        ui->actionLoad_Mean_Curvature->setEnabled(enabled);
        ui->actionLoad_Shape_Index->setEnabled(enabled);
        ui->actionLoad_SDF->setEnabled(enabled);
        ui->actionLoad_Colorimetric->setEnabled(enabled);
        ui->actionLoadFacets->setEnabled(enabled);
        ui->actionAnnotation_Mode->setEnabled(enabled);
        ui->actionLoadMinimal_Bounding_Box->setEnabled(enabled);
    };

    auto resetToggleButtons = [this] {
        ui->actionLoadMinimal_Bounding_Box->setChecked(false);

        ui->actionLoadFacets->setChecked(false);

        ui->actionAnnotation_Mode->setChecked(false);
        // TODO: add reset function for annotation mode
    };

    connect(initialState, &QState::entered, [=] {
            setEnabledModelDependentButtons(false);
    });

    connect(loadedState, &QState::exited, [=] {
            setEnabledModelDependentButtons(false);
    });

    connect(loadedState, &QState::entered, [=] {
            setEnabledModelDependentButtons(true);
    });

    connect(loadedState, &QState::entered,
            resetToggleButtons);

    connect(ui->itemWidget, &InspectionItemWidget::annotationDeleted,
            this, &FragmentViewMain::onAnnotationDeleteRequested);

}

void FragmentViewMain::load3DModelFile(const QString &filename)
{
    ui->itemWidget->load3DModelFromFile(filename);
}

InspectionItemWidget *FragmentViewMain::itemWidget()
{
    return ui->itemWidget;
}

void FragmentViewMain::onClipboardContextMenuOpen(QMenu *menu, const GResource &resource)
{
    if(resource.type == GResource::Type::ARTEFACT)
    {
        auto action = new QAction(tr("Load Artefact"));
        connect(action, &QAction::triggered,
                [this, resource] {
            loadArtefact(resource.artefactId);
        });

        menu->addAction(action);
    }
}

void FragmentViewMain::loadArtefact(const ArtefactId &artefactId)
{
    ArtefactLoading::ArtefactLoadHelper helper(m_dashboard, this);

    if(!helper.loadArtefactInfo(artefactId, m_artefact))
    {
        return;
    }

    auto modelUrl = m_artefact.getUrl(activeResolution(), MultiResProperty::MESH, true);

    if(!load3DModelFromUrl(modelUrl))
    {
        return;
    }

    m_previousResolution = m_resolutionSelector->currentResolution();
    ui->itemWidget->setArtefact(m_artefact);
    m_metadataModel->requestArtefactMetadata(artefactId);
}

void FragmentViewMain::handleLoadCHAP()
{
    qDebug() << "Loading Cultural Heritage Artefact Partonomy...";
    QString resource = "://rdf/partonomy.ttl";

    QFile f(resource);
    if(!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Could not load file %1").arg(resource)
        );
    }

    SkosTreeParserSord parser;
    QUrl root("http://www.cidoc-crm.org/cidoc-crm/E55_Type");

    auto json = parser.parseToJson(":/rdf/partonomy.ttl", root);

    if(json.size() == 0)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Error in parsing the RDF graph")
//                              .arg(QString::fromStdString(parser.errorMessage()))
                              );
        return;
    }

    m_thesaurusModel = new ThesaurusModel(this);

    QString errMsg;

    if(!m_thesaurusModel->readJson(json, &errMsg))
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("JSON parse error: %1").arg(errMsg));
        return;
    }

    ui->partonomyWidget->setModel(m_thesaurusModel);
}

void FragmentViewMain::setResolutionBlockingSignals(ModelResolution resolution)
{
    bool previous = m_resolutionSelector->blockSignals(true);
    m_resolutionSelector->setResolution(resolution);
    m_resolutionSelector->blockSignals(previous);
}

bool FragmentViewMain::change3DModelResolution(ModelResolution targetResolution)
{
    if(!ui->itemWidget->mesh()->isLoaded())
    {
        // NOTE: Nothing to do, fails silently
        return false;
    }


    auto modelUrl = m_artefact.getUrl(targetResolution, MultiResProperty::MESH, true);

    if(!load3DModelFromUrl(modelUrl))
    {
        setResolutionBlockingSignals(m_previousResolution);
        return false;
    }

    setResolutionBlockingSignals(targetResolution);
    m_previousResolution = m_resolutionSelector->currentResolution();

    return true;
}

bool FragmentViewMain::load3DModelFromUrl(const QUrl modelUrl)
{
    ArtefactLoading::ArtefactLoadHelper helper(m_dashboard, this);
    QString file;
    if(!helper.getLocalFile(modelUrl , file))
    {
        return false;
    }

    ui->itemWidget->load3DModelFromFile(file);

    auto propertySelector = m_propertyToolbar->propertySelector;

    propertySelector->blockSignals(true);

    m_propertyColoring->reset();
    m_wireframeBox->reset();
    m_facetOverlay->reset();

    m_propertyToolbar->resetState();
    m_propertyToolbar->setEnabled(true);

    propertySelector->blockSignals(false);
    return true;
}

void FragmentViewMain::onAnnotationModeEnabled(bool enabled)
{
    if(enabled && m_annotationModel == nullptr)
    {
        loadArtefactAnnotations();
    }

    m_annotationTool->setAnnotationsVisible(enabled);
    m_annotationTool->setEnabled(enabled);

    setAnnotationToolsEnabled(enabled);
    ui->itemWidget->meshViewWidget()->update();
}

void FragmentViewMain::loadArtefactAnnotations()
{
    if(!ui->itemWidget->isModelLoaded())
    {
        QMessageBox::critical(this,
                             tr("Error"),
                             tr("Please load a 3D model first"));
        return;
    }


    m_annotationModel = new LocalAnnotationModel(this);

    ui->itemWidget->setAnnotationModel(m_annotationModel);

    auto currentResolution = m_resolutionSelector->currentResolution();
    auto artefact = ui->itemWidget->artefact();
    auto modelUrl = artefact.getUrl(currentResolution, MultiResProperty::MESH, true);

    AnnotationHelper helper(m_selectionTool, m_annotationTool, m_annotationModel, this);
    helper.loadAnnotations(modelUrl);

    setAnnotationToolsEnabled(true);
}

void FragmentViewMain::onAnnotationRequest()
{
    if(!isTermSelected())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Please select a term first"));
        return;
    }

    auto modelUrl = getCurrentUrlForProperty(MultiResProperty::MESH);
    auto term = getSelectedTerm();

    AnnotationHelper helper(m_selectionTool, m_annotationTool, m_annotationModel, this);
    helper.onAnnotationRequest(modelUrl, term);
}

void FragmentViewMain::loadFacets()
{
    if(!m_facetOverlay->isLoaded())
    {
        loadProperty<std::vector<Facet>, FacetOverlay>(m_facetOverlay);
    }

    if(!m_facetOverlay->isLoaded())
    {
        ui->actionLoadFacets->setChecked(false);
        return;
    }

    m_propertyToolbar->facetCheckBox->setEnabled(true);
    m_propertyToolbar->facetCheckBox->setChecked(true);
}

void FragmentViewMain::onFacetsEnabled(bool enabled)
{
    m_facetOverlay->setVisible(enabled);
}

void FragmentViewMain::loadBoundingBox()
{
    if(!m_wireframeBox->isSet())
    {
        loadProperty<BoundingBox, WireframeBox>(m_wireframeBox);
    }

    m_propertyToolbar->boundingBoxCheckBox->setEnabled(true);
    m_propertyToolbar->boundingBoxCheckBox->setChecked(true);

    m_wireframeBox->setVisible(true);
}

void FragmentViewMain::onMinimalBoundingBoxToggled(bool enabled)
{
    if(!m_wireframeBox->isSet())
        return;

    m_wireframeBox->setVisible(enabled);
}

void FragmentViewMain::onDebugLoadFileFromDisk()
{
    // open file
    QString selectedFilter;

    QString file = QFileDialog::getOpenFileName(this, tr("Select a file"), "",
                                                tr("3D models (*.ply);;"
                                                   "Faceting (*.fct);;"
                                                   "Property (*.txt);;"
                                                   "Bounding Box (*.json);;"
                                                   "Features (*.json)"
                                                ),
                                                &selectedFilter
                                                );
    if(file.isEmpty())
    {
        return;
    }

    auto artefact = ui->itemWidget->artefact();

    QString missingModelErrorMsg = tr("Please load a 3D model first");

    // case: PLY file --> load as a 3d model
    // case: FCT file --> load as facets
    // case: TXT file --> get base name suffix and load as property
    // case: JSON file --> oriented bounding box
    QFileInfo info(file);

    if( info.suffix() == "ply")
    {
        ui->itemWidget->load3DModelFromFile(file);
        return;
    }

    // This guarantees that a 3D model has been loaded first
    if(!ui->itemWidget->isModelLoaded())
    {
        QMessageBox::critical(this, tr("Error"), missingModelErrorMsg);
        return;
    }

    if (info.suffix() == "fct") {
        loadLocalPropertyFile<std::vector<Facet>>(file, m_facetOverlay);
        return;
    }
    else if(info.suffix() == "txt") {
        auto property = GravitateRepoUtils::propertyType(file);
        switch(property)
        {
        case PropertyType::MEAN_CURVATURE:
        {
            MeanCurvaturePropertyRendering ren(m_propertyColoring);
            loadLocalPropertyFile<MeanCurvatureProperty>(file, &ren);
            break;
        }
        case PropertyType::SHAPE_INDEX:
        {
            ShapeIndexPropertyRendering ren(m_propertyColoring);
            loadLocalPropertyFile<ShapeIndexProperty>(file, &ren);
            break;
        }
        case PropertyType::LIGHTNESS:
        {
            LightnessPropertyRendering ren(m_propertyColoring);
            loadLocalPropertyFile<LightnessProperty>(file, &ren);
            break;
        }
        case PropertyType::SDF:
        {
            SDFPropertyRendering ren(m_propertyColoring);
            loadLocalPropertyFile<SDFProperty>(file, &ren);
            break;
        }
        default:
            break;
        }

        return;
    }
    else if(info.suffix() == "json")
    {
        if(selectedFilter.contains("Bounding Box"))
        {
            loadLocalPropertyFile<BoundingBox>(file, m_wireframeBox);
            return;
        }
        else if(selectedFilter.contains("Feature"))
        {
            loadLocalPropertyFile<std::vector<HoughFeature>>(file, m_houghFeatureTool);
            return;
        }
    }

    QMessageBox::critical(this, tr("Error"), tr("Cannot handle file type '%1'").arg(info.suffix()));
    return;

}

void FragmentViewMain::onAnnotationDeleteRequested(const QModelIndex &index)
{
    auto reply = QMessageBox::question(this,
                          tr("Delete an annotation"),
                          tr("Are you sure that you want to "
                            "delete the selected annotation?"));

    if(reply != QMessageBox::Yes)
    {
        return;
    }

    m_annotationModel->removeAnnotation(index);
    auto id = index.row();
    m_annotationTool->removeAnnotation(id);
}

void FragmentViewMain::onActionFeatureRecognition()
{
    if(!ui->itemWidget->isModelLoaded())
    {
        QMessageBox::information(this,
                                 tr("Information"),
                                 tr("Please load a 3D model first"));
        return;
    }

    if(!change3DModelResolution(ModelResolution::RES_100K))
    {
        return;
    }

    auto modelUrl = getCurrentUrlForProperty(MultiResProperty::MESH);
    auto facetUrl = getCurrentUrlForProperty(MultiResProperty::FACETS);

    FeatureExtractionParams params;

    bool accepted = getFeatureExtractionParamsDialog(modelUrl, facetUrl, params);

    if(!accepted)
    {
        return;
    }



    bool ok = m_featureAlgo->run(params);

    if(!ok)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("The feature extraction call has failed. See the console for further information."));
        return;
    }

    QElapsedTimer timer;
    timer.start();

    QProgressDialog dialog(this);

    dialog.setLabelText("Feature extraction in progress");
    dialog.setMinimum(0);
    dialog.setMaximum(0);
    dialog.setValue(0);

    connect(&dialog, &QProgressDialog::canceled,
            m_featureAlgo, &FeatureExtractionAlgorithm::cancel);

    dialog.show();
    connect(m_featureAlgo, &FeatureExtractionAlgorithm::finished,
            m_featureAlgo, [this] (bool success, const QList<HoughFeature> &features)
    {
        if(!success)
        {
            return;
        }


        auto vector = features.toVector().toStdVector();

        loadLocalProperty(vector, m_houghFeatureTool);

    });

    connect(m_featureAlgo, &FeatureExtractionAlgorithm::finished,
            &dialog, &QProgressDialog::close);

    QEventLoop loop;
    connect(m_featureAlgo, &FeatureExtractionAlgorithm::finished,
            &loop, &QEventLoop::quit);
    connect(m_featureAlgo, &FeatureExtractionAlgorithm::canceled,
            &loop, &QEventLoop::quit);

    loop.exec();

    qInfo() << "Elapsed time: " << (timer.elapsed() / 1000) << "seconds";
    // QxtSignalWaiter::wait(m_featureAlgo, SIGNAL(finished(bool, const QList<HoughFeature>)));

}

void FragmentViewMain::setAnnotationToolsEnabled(bool enabled)
{
    m_annotationTool->setEnabled(enabled);
    m_selectionTool->setEnabled(enabled);
}

void FragmentViewMain::setupSignals()
{
    connect(ui->actionLoad_Mean_Curvature, &QAction::triggered,
            [this] {
        MeanCurvaturePropertyRendering ren(m_propertyColoring);
        loadScalarFieldProperty<MeanCurvatureProperty>(&ren);
    });


    connect(ui->actionLoad_Shape_Index, &QAction::triggered,
            [this] {
        ShapeIndexPropertyRendering ren(m_propertyColoring);
        loadScalarFieldProperty<ShapeIndexProperty>(&ren);
    });

    connect(ui->actionLoad_Colorimetric, &QAction::triggered,
            [this] {
        LightnessPropertyRendering ren(m_propertyColoring);
        loadScalarFieldProperty<LightnessProperty>(&ren);
    });

    connect(ui->actionLoad_SDF, &QAction::triggered,
            [this] {
        DashboardSDFPropertyRendering ren(m_propertyColoring, m_artefact.avgthickness);
        loadScalarFieldProperty<SDFProperty>(&ren);
    });

    connect(ui->actionLoad_CHAP, &QAction::triggered,
            this, &FragmentViewMain::handleLoadCHAP);
}

void FragmentViewMain::saveAnnotation(const PartAnnotation &annotation)
{
    if(!ui->itemWidget->isModelLoaded())
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Error: load a 3D model first"));
        return;
    }

    auto currentResolution = m_resolutionSelector->currentResolution();
    auto artefact = ui->itemWidget->artefact();
    auto modelUrl = artefact.getUrl(currentResolution, MultiResProperty::MESH, true);

    if(!m_annotationModel->saveAnnotation(modelUrl, annotation))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not save annotation"));
        return;
    }

    m_annotationTool->addAnnotation(annotation);
}

bool FragmentViewMain::isTermSelected()
{
    auto index = ui->partonomyWidget->currentSelectedIndex();
    return index.isValid();
}

Term FragmentViewMain::getSelectedTerm()
{
    auto index = ui->partonomyWidget->currentSelectedIndex();
    return m_thesaurusModel->term(index);
}

QString FragmentViewMain::indexFileLocation()
{
    if(!ui->itemWidget->isModelLoaded() ||
        ui->itemWidget->currentModelPath().isEmpty())
    {
        return QString();
    }

    QFileInfo info(ui->itemWidget->currentModelPath());
    QString currentModelName = info.baseName();
    QDir appdataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString indexFile = QDir(appdataDir).filePath("annotations/" + currentModelName + ".json");

    return indexFile;
}

ModelResolution FragmentViewMain::selectVersionBySize(ModelResolution res)
{
    auto a = ui->itemWidget->artefact();
    if(a.hasUrl(res, MultiResProperty::MESH))
    {
        return res;
    }
    return ModelResolution::RES_FULL;
}

ModelResolution FragmentViewMain::activeResolution()
{
    return m_resolutionSelector->currentResolution();
}

ModelResolution FragmentViewMain::findTargetResolution(PropertyType type)
{
    if(type == PropertyType::UNDEFINED)
    {
        return activeResolution();
    }
    return GravitateMetadata::findTargetResolution(type);
}

void FragmentViewMain::demoFeatureRecognition(const FeatureExtractionDemoParams &demoParams)
{
    // HACK: run demo of the feature recognition on a sample fragment
    m_resolutionSelector->setResolution(ModelResolution::RES_100K);
    loadArtefact(demoParams.artefactId);

    auto modelUrl = getCurrentUrlForProperty(MultiResProperty::MESH);
    auto facetUrl = getCurrentUrlForProperty(MultiResProperty::FACETS);

    FeatureExtractionParams params;

    bool accepted = getFeatureExtractionParamsDialog(modelUrl, facetUrl, params);

    if(!accepted)
    {
        return;
    }

    QScopedPointer<QTemporaryFile> tempFile(QTemporaryFile::createNativeFile(demoParams.outputFile));

    loadLocalPropertyFile<std::vector<HoughFeature>>(tempFile->fileName(), m_houghFeatureTool);

    for(int i = 0; i < m_houghFeatureTool->featureCount(); i++)
    {
        bool shouldDisplay = demoParams.outputClusters.contains(i);
        m_houghFeatureTool->setFeatureVisible(i, shouldDisplay);
    }
}

void FragmentViewMain::onSaveCurrentFeaturesRequest()
{
    auto data = m_featureAlgo->lastFeatureSetData();

    if(data.isEmpty())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Please run the feature extraction algorithm first"));
        return;
    }

    auto fileName = QFileDialog::getSaveFileName(this,
                                 tr("Save feature recognition output"),
                                 "",
                                 "*.json"
                                 );

    if(fileName.isEmpty())
    {
        return;
    }

    QFile outFile(fileName);
    if(!outFile.open(QIODevice::WriteOnly))
    {
        qCritical() << "Error while opening the file for writing:" << fileName;
        return;
    }

    outFile.write(data);

    outFile.close();
}

bool FragmentViewMain::getFeatureExtractionParamsDialog(const QUrl &modelUrl, const QUrl &facetUrl, FeatureExtractionParams &outParams)
{
    ParameterSelectionDialog dialog;

    dialog.setWindowTitle(tr("Feature Recognition"));

    QString paramCurveType = "curveType";
    dialog.addListParameter(paramCurveType, tr("Curve type"));

    auto curves = {
        FeatureExtractionParams::CurveType::GEOMETRIC_PETAL,
        FeatureExtractionParams::CurveType::CITRUS,
        FeatureExtractionParams::CurveType::CIRCUMFERENCE,
        FeatureExtractionParams::CurveType::D_CONVEXITIES,
        FeatureExtractionParams::CurveType::ELLIPSE,
        FeatureExtractionParams::CurveType::SPIRAL,
    };

    for(auto curve : curves)
    {
        dialog.addListParameterValue(paramCurveType, labelString(curve), paramString(curve));
    }

    dialog.setValue(paramCurveType, labelString(FeatureExtractionParams::CurveType::GEOMETRIC_PETAL));

    QString paramParD = "parD";
    dialog.addIntParameter(paramParD, tr("D"), 4, 20);
    dialog.setValue(paramParD, 10);

    QString paramParN = "parN";
    dialog.addIntParameter(paramParN, tr("N"), 10, 100);
    dialog.setValue(paramParN, 55);

    QString paramMinPts = "minPts";
    dialog.addIntParameter(paramMinPts, tr("Minimum no. Points per cluster"), 1, 100);
    dialog.setValue(paramMinPts, 30);

    QString paramEpsilon = "eps";
    dialog.addDoubleParameter(paramEpsilon, tr("Epsilon"), 0.0, 2.0, 0.1);
    dialog.setValue(paramEpsilon, 0.5);

    QString paramFacet = "facet";
    dialog.addBoolParameter(paramFacet, tr("Use facets (when available)"));
    dialog.setValue(paramFacet, !facetUrl.isEmpty());
    auto facetParamWidget = dialog.getWidget(paramFacet);
    if(facetUrl.isEmpty() && facetParamWidget != nullptr)
    {
        facetParamWidget->setEnabled(false);
    }

    QString paramErosion = "erosion";
    dialog.addIntParameter(paramErosion, tr("Erosion"), 0, 20);
    dialog.setValue(paramErosion, 5);

    auto result = dialog.exec();

    if(result != QDialog::Accepted)
    {
        return false;
    }

    outParams.curveId = dialog.data(paramCurveType).toString();
    outParams.modelUrl = modelUrl;
    outParams.facetUrl = facetUrl;
    outParams.parD = dialog.value(paramParD).toInt();
    outParams.parN = dialog.value(paramParN).toInt();
    outParams.minPts = dialog.value(paramMinPts).toInt();
    outParams.eps = dialog.value(paramEpsilon).toDouble();
    outParams.useFacet = dialog.value(paramFacet).toBool();
    outParams.erosion = dialog.value(paramErosion).toInt();

    return true;
}

void FragmentViewMain::initItemWidget()
{
    ui->itemWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void FragmentViewMain::initProperties()
{
    auto mesh = ui->itemWidget->mesh();
    auto meshWidget = ui->itemWidget->meshViewWidget();

    m_propertyColoring = new PropertyColoring(mesh, meshWidget);
    m_propertyToolbar = new MeshPropertyToolbar(this);
    connect(m_propertyToolbar->propertySelector, &PropertySelector::propertyTypeChanged,
            m_propertyColoring, &PropertyColoring::selectProperty);
    m_propertyToolbar->setEnabled(false);

    // NOTE: annotation checkbox is redundant with annotation mode
    m_propertyToolbar->annotationCheckBox->setEnabled(false);

    addToolBar(Qt::TopToolBarArea, m_propertyToolbar);

    // Faceting
    m_facetOverlay = new FacetOverlay(mesh, meshWidget);

    connect(ui->actionLoadFacets, &QAction::triggered,
            this, &FragmentViewMain::loadFacets);

    connect(m_propertyToolbar->facetCheckBox, &QCheckBox::toggled,
            this, &FragmentViewMain::onFacetsEnabled);

    m_propertyToolbar->facetCheckBox->setEnabled(false);

    // Bounding Box
    m_wireframeBox = new WireframeBox(meshWidget);

    connect(ui->actionLoadMinimal_Bounding_Box, &QAction::triggered,
            this, &FragmentViewMain::loadBoundingBox);

    connect(m_propertyToolbar->boundingBoxCheckBox, &QCheckBox::toggled,
            this, &FragmentViewMain::onMinimalBoundingBoxToggled);

    m_propertyToolbar->boundingBoxCheckBox->setEnabled(false);

}

void FragmentViewMain::initAnnotation()
{
    auto mesh = ui->itemWidget->mesh();
    auto meshWidget = ui->itemWidget->meshViewWidget();

    // Annotation Model
    connect(ui->itemWidget, &InspectionItemWidget::annotationRequested,
            this, &FragmentViewMain::onAnnotationRequest);

    connect(ui->actionAnnotation_Mode, &QAction::toggled,
            this, &FragmentViewMain::onAnnotationModeEnabled);

    connect(ui->itemWidget, &InspectionItemWidget::modelLoaded,
            [this] {
        if(m_annotationModel == nullptr)
            return;

        // TODO: Extract function unsetAnnotationModel
        ui->itemWidget->setAnnotationModel(nullptr);
        m_annotationModel->clearAnnotations();
        m_annotationModel->deleteLater();
        m_annotationModel = nullptr;
    });

    // Initialize Selection Tool
    m_selectionTool = new SelectionTool(mesh, meshWidget);

    connect(ui->itemWidget, &InspectionItemWidget::modelLoaded,
            m_selectionTool, &SelectionTool::clearSelection);

    // Inizialize Annotation Tool
    m_annotationTool = new AnnotationTool(mesh, meshWidget);

    connect(m_annotationTool, &AnnotationTool::annotationRequested,
            this, &FragmentViewMain::onAnnotationRequest);

    ui->itemWidget->setAnnotationsEditable(true);

}

void FragmentViewMain::initMetadataModel()
{
    auto metadataChProvider = BackendFactoryBase::instance()->createMetadataChProvider();

    m_metadataModel = new MetadataModel(metadataChProvider.release(), this);
    ui->itemWidget->setMetadataModel(m_metadataModel);
}

void FragmentViewMain::initFeatureRecognition()
{
    auto mesh = ui->itemWidget->mesh();
    auto meshWidget = ui->itemWidget->meshViewWidget();

    // Initialize UI button
    connect(ui->actionAutomatic_Feature_Recognition, &QAction::triggered,
            this, &FragmentViewMain::onActionFeatureRecognition);

    // Set up algorithm connections
    m_featureAlgo = new FeatureExtractionAlgorithm(m_dashboard->jobManager(), this);

    m_houghFeatureTool = new HoughFeatureTool(mesh, meshWidget);

    auto menu = new QMenu();
    menu->addAction(tr("Save current feature set"), this, &FragmentViewMain::onSaveCurrentFeaturesRequest);

    ui->actionAutomatic_Feature_Recognition->setMenu(menu);
}

void FragmentViewMain::initDemoFeatureRecognition()
{
    FeatureExtractionDemoParams entries[] = {
        {
            "1909.310.2",
            {QUrl("http://collection.britishmuseum.org/id/object/GAA58722")},
            "://demo/1909_310_2_100K_Features.json",
            {0}
        },
        {
            "D.292",
            {QUrl("http://data.cyl/id/object/D.292")},
            ":/demo/D_292_100K_Features.json",
            {0,1}
        },
        {
            "AN1909.837",
            {QUrl("http://www.ashmolean.org/id/object/AN1909.837")},
            "://demo/AN1909_837_100K_Features.json",
            {0,1}
        }
    };

    QMenu *menu = new QMenu();

    for(auto paramSet : entries)
    {
        menu->addAction(paramSet.label, [this, paramSet] { this->demoFeatureRecognition(paramSet); });
    }

    ui->actionFeature_Extraction_Demo->setMenu(menu);

//    connect(ui->actionFeature_Extraction_Demo, &QAction::triggered,
//            this, &FragmentViewMain::demoFeatureRecognition);
}

void FragmentViewMain::initResolutionSelector()
{
    m_resolutionSelector = new ResolutionSelector(this);
    m_resolutionSelector->initOptions(GravitateRepoUtils::AllModelResolutions);

    connect(m_resolutionSelector, &ResolutionSelector::resolutionChanged,
            this, &FragmentViewMain::change3DModelResolution, Qt::UniqueConnection);

    ui->toolBar->addWidget(m_resolutionSelector);

    m_previousResolution = m_resolutionSelector->currentResolution();
}

QUrl FragmentViewMain::getCurrentUrlForProperty(MultiResProperty property)
{
    auto res = activeResolution();

    return m_artefact.getUrl(res, property, res != ModelResolution::RES_ORIGINAL);
}


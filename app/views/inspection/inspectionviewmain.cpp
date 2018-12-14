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
#include "inspectionviewmain.h"
#include "ui_inspectionviewmain.h"

#include <QDebug>
#include "gravitatebackend.h"
#include "metadata3drequest.h"

#include <QTemporaryFile>
#include <QProgressDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>

#include "gravitaterepoconf.h"
#include "gravitaterepoutils.h"
#include "downloadprogressdialog.h"
#include "common/gravitatemetadata.h"
#include "boundingbox.h"
#include "inspectionitemwidget.h"
#include "helphandler.h"


#include "property/wireframebox.h"
#include "annotation/annotationtool.h"
#include "backendfactorybase.h"
#include "dashboardsdfpropertyrendering.h"
#include "metadatachprovider.h"

#include <memory>

InspectionViewMain::InspectionViewMain(AbstractDashboard *dashboard, QWidget *parent) :
    AbstractView(dashboard, parent),
    ui(new Ui::InspectionViewMain)
{
    ui->setupUi(this);

    initResolutionSelection();

    initPropertySelection();

    initPropertyLoadSignals();
}

InspectionViewMain::~InspectionViewMain()
{
    delete ui;
}

void InspectionViewMain::initPropertySelection()
{
    m_propertyToolbar = new MeshPropertyToolbar(this);
    connect(m_propertyToolbar->propertySelector, &PropertySelector::propertyTypeChanged,
            [this] (PropertyType type) {
        qDebug() << GravitateRepoUtils::propertyName(type);
        for(auto item : m_activeItems)
        {
            item.propertyColoring->selectProperty(type);
        }
    });

    connect(this, &InspectionViewMain::resolutionChanged,
            m_propertyToolbar, &MeshPropertyToolbar::resetState);

    m_propertyToolbar->resetState();

    addToolBar(Qt::ToolBarArea::TopToolBarArea, m_propertyToolbar);
}

int InspectionViewMain::numberOfLoadedItems()
{
    return activeItems().count();
}

void InspectionViewMain::initTools()
{
    m_geomCharMenu = new QMenu(tr("Geometric\nCharacterization"));
    addDefaultTools();
    toolSet().beginMenu(m_geomCharMenu)
        .addTool(ui->actionLoad_Mean_Curvature)
        .addTool(ui->actionLoad_Shape_Index)
        .addTool(ui->actionLoad_Colorimetric)
        .addTool(ui->actionLoad_SDF)
        .addTool(ui->actionMinimal_Bounding_Box)
    .endMenu()
    .addTool(ui->actionLoadFacets)
    .addTool(ui->actionLoad_Annotations);

    auto handler = m_dashboard->helpHandler();

    if(handler == nullptr)
    {
        qCritical() << "HelpHandler not initialized" << Q_FUNC_INFO;
        return;
    }

    // Help Initialization
    m_resolutionSelector->setWhatsThis(handler->getText("resolution-selector"));
    m_resolutionSelector->setToolTip(tr("Resolution Selector"));
    m_propertyToolbar->initHelp(handler);

    ui->actionLoadFacets->setWhatsThis(handler->getText("tool-load-facets"));
    ui->actionLoad_Annotations->setWhatsThis(handler->getText("tool-load-annotations"));

    auto geomTool = toolSet().getToolButton(m_geomCharMenu);
    if(geomTool != nullptr)
    {
        geomTool->setWhatsThis(handler->getText("menu-geom-char"));
    }
}

void InspectionViewMain::addInspectionItem(const InspectionItem &item)
{
    auto widget = item.widget;

    ui->horizontalLayout->addWidget(widget);

    m_activeItems.append(item);

    // Connect signals for closing the widget properly
    connect(item.widget, &InspectionItemWidget::widgetClosed,
            this, [this, widget] {
        int index = ui->horizontalLayout->indexOf(widget);

        widget->disconnect();
        ui->horizontalLayout->removeWidget(widget);
        m_activeItems.removeAt(index);
    });

    // HACK: there should be a more appropriate place for this setting
    item.annotationTool->setReadOnly(true);
}


void InspectionViewMain::onClipboardContextMenuOpen(QMenu *menu, const GResource &resource)
{
    if(resource.type == GResource::Type::ARTEFACT)
    {
        auto action = new QAction(tr("Load Artefact"));
        connect(action, &QAction::triggered,
                [this, resource] {

            loadArtefact(resource.artefactId);

//            ArtefactLoaderInspection loader(this);
//            loader.loadArtefact(resource.id);
        });

        menu->addAction(action);
    }
}

InspectionItem InspectionViewMain::createInspectionItem(InspectionItemWidget *widget, const Artefact3DMetadata &artefact)
{
    auto mesh = widget->mesh();
    auto viewport = widget->meshViewWidget();
    auto metadataChProvider = BackendFactoryBase::instance()->createMetadataChProvider();

    InspectionItem item
    {
        widget,
        artefact,
        new FacetOverlay(mesh, viewport),
        new PropertyColoring(mesh, viewport),
        new WireframeBox(viewport),
        new LocalAnnotationModel(viewport),
        new AnnotationTool(mesh, viewport),
        new MetadataModel(metadataChProvider.release(), this)
    };

    return item;
}

void InspectionViewMain::loadAllFacets()
{
    using namespace std::placeholders;

    // FIXME: checkboxes are always enabled, even in case of failure

    bool allFailed = false; //loadProperty<std::vector<Facet>, FacetOverlay>();

    forEachLoadedItem(std::bind(&InspectionViewMain::loadFacets, this, _1));

    if(!allFailed)
    {
        m_propertyToolbar->facetCheckBox->setEnabled(true);
        m_propertyToolbar->facetCheckBox->setChecked(true);
    }
}

void InspectionViewMain::setFacetsVisible(bool visible)
{
    for(auto item : activeItems())
    {
        item.facetOverlay->setVisible(visible);
    }
}

void InspectionViewMain::loadBoundingBoxes()
{
    using namespace std::placeholders;

    forEachLoadedItem(std::bind(&InspectionViewMain::loadBoundingBox, this, _1));

    bool allFailed = false;

    if(!allFailed)
    {
        m_propertyToolbar->boundingBoxCheckBox->setEnabled(true);
        m_propertyToolbar->boundingBoxCheckBox->setChecked(true);
    }
}

void InspectionViewMain::setBoundingBoxesVisible(bool visible)
{
    for(auto item : activeItems())
    {
        item.wireframeBox->setVisible(visible);
    }
}

void InspectionViewMain::loadAnnotations()
{
    auto res = m_resolutionSelector->currentResolution();

    for(auto item: activeItems())
    {
        auto modelUrl = item.artefact.getUrl(res, MultiResProperty::MESH, true);
        item.annotationModel->loadAnnotations(modelUrl);

        for(auto anno : item.annotationModel->annotations())
        {
            item.annotationTool->addAnnotation(anno);
        }
    }

    m_propertyToolbar->annotationCheckBox->setEnabled(true);
    m_propertyToolbar->annotationCheckBox->setChecked(true);
}

void InspectionViewMain::setAnnotationsVisible(bool visible)
{
    for(auto item : activeItems())
    {
        item.annotationTool->setAnnotationsVisible(visible);
    }
}

void InspectionViewMain::loadArtefact(const ArtefactId &artefactId)
{
    using namespace ArtefactLoading;

    ArtefactLoadHelper helper(m_dashboard, this);

    connect(&helper, &ArtefactLoadHelper::error,
            [this] (const QString &errorMessage) {
        QMessageBox::critical(this, tr("Error"), errorMessage);
    });

    Artefact3DMetadata artefact;

    if(!helper.loadArtefactInfo(artefactId, artefact))
    {
        return;
    }

    // If download has been successful, load it into the itemWidget
    auto itemWidget = new InspectionItemWidget(this);
    itemWidget->setArtefact(artefact);

    InspectionItem item = createInspectionItem(itemWidget, artefact);

    if(!load3DModel(item))
    {
        delete itemWidget;
        return;
    }

    addInspectionItem(item);

    auto prop = m_propertyToolbar->propertySelector->currentProperty();

    if(prop != PropertyType::RGB_COLOR)
    {
        loadScalarFieldProperty(item, prop);
    }

    if(m_propertyToolbar->facetCheckBox->isEnabled())
    {
        loadFacets(item);
    }

    if(m_propertyToolbar->boundingBoxCheckBox->isEnabled())
    {
        loadBoundingBox(item);
    }

    itemWidget->setMetadataModel(item.metadataModel);
    itemWidget->setAnnotationModel(item.annotationModel);
    item.metadataModel->requestArtefactMetadata(artefactId);
}

void InspectionViewMain::initResolutionSelection()
{
    m_currentResolution = ModelResolution::RES_ORIGINAL;

    m_resolutionSelector = new ResolutionSelector(this);
    m_resolutionSelector->initOptions(GravitateRepoUtils::AllModelResolutions);
    m_resolutionSelector->setResolution(m_currentResolution);

    connect(m_resolutionSelector, &ResolutionSelector::resolutionChanged,
            this, &InspectionViewMain::changeResolution);

    connect(this, &InspectionViewMain::resolutionChanged,
            [this] (ModelResolution res){
        bool prev = m_resolutionSelector->blockSignals(true);
        m_resolutionSelector->setResolution(res);
        m_resolutionSelector->blockSignals(prev);
    });

    ui->toolBar->addWidget(m_resolutionSelector);
}

void InspectionViewMain::changeResolution(ModelResolution resolution)
{
    m_currentResolution = resolution;

    for(auto item : m_activeItems)
    {
        load3DModel(item);
    }

    emit resolutionChanged(resolution);
}

QList<InspectionItem> InspectionViewMain::activeItems()
{
    return m_activeItems;
}

ModelResolution InspectionViewMain::activeResolution()
{
    return m_currentResolution;
}

void InspectionViewMain::initPropertyLoadSignals()
{
    using namespace std::placeholders;

    connect(ui->actionLoad_Mean_Curvature, &QAction::triggered,
            [this] {
        forEachLoadedItem(std::bind(&InspectionViewMain::loadMeanCurvature, this, _1));
    });
    connect(ui->actionLoad_Shape_Index, &QAction::triggered,
            [this] {
        forEachLoadedItem(std::bind(&InspectionViewMain::loadShapeIndex, this, _1));
    });
    connect(ui->actionLoad_Colorimetric, &QAction::triggered,
            [this] {
        forEachLoadedItem(std::bind(&InspectionViewMain::loadLightness, this, _1));
    });
    connect(ui->actionLoad_SDF, &QAction::triggered,
            [this] {
        forEachLoadedItem(std::bind(&InspectionViewMain::loadSDF, this, _1));
    });

    connect(ui->actionLoadFacets, &QAction::triggered,
            this, &InspectionViewMain::loadAllFacets);
    connect(ui->actionLoad_Annotations, &QAction::triggered,
            this, &InspectionViewMain::loadAnnotations);
    connect(ui->actionMinimal_Bounding_Box, &QAction::triggered,
            this, &InspectionViewMain::loadBoundingBoxes);

    connect(m_propertyToolbar->facetCheckBox, &QCheckBox::toggled,
            this, &InspectionViewMain::setFacetsVisible);

    connect(m_propertyToolbar->boundingBoxCheckBox, &QCheckBox::toggled,
            this, &InspectionViewMain::setBoundingBoxesVisible);

    connect(m_propertyToolbar->annotationCheckBox, &QCheckBox::toggled,
            this, &InspectionViewMain::setAnnotationsVisible);
}

ModelResolution InspectionViewMain::findTargetResolution(PropertyType type)
{
    if(type == PropertyType::UNDEFINED)
    {
        return activeResolution();
    }
    return GravitateMetadata::findTargetResolution(type);
}

void InspectionViewMain::forEachLoadedItem(std::function<void (const InspectionItem &)> f)
{
    for(auto item : activeItems())
    {
        f(item);
    }
}

bool InspectionViewMain::load3DModel(const InspectionItem &item)
{
    // TODO: should avoid re-loading the same model when the resolution is the same
    auto dl = m_dashboard->downloader();

    ArtefactLoading::ArtefactLoadHelper helper(m_dashboard, this);

    auto remoteUrl = item.artefact.getUrl(m_currentResolution, MultiResProperty::MESH, true);

    QString localFile;
    if(!helper.getLocalFile(remoteUrl, localFile))
    {
        return false;
    }

    item.widget->load3DModelFromFile(localFile);

    return true;
}

void InspectionViewMain::loadMeanCurvature(const InspectionItem &item)
{
    MeanCurvaturePropertyRendering ren(item.propertyColoring);
    loadScalarField<MeanCurvatureProperty>(item, &ren);
}

void InspectionViewMain::loadShapeIndex(const InspectionItem &item)
{
    ShapeIndexPropertyRendering ren(item.propertyColoring);
    loadScalarField<ShapeIndexProperty>(item, &ren);
}

void InspectionViewMain::loadLightness(const InspectionItem &item)
{
    LightnessPropertyRendering ren(item.propertyColoring);
    loadScalarField<LightnessProperty>(item, &ren);
}

void InspectionViewMain::loadSDF(const InspectionItem &item)
{
    DashboardSDFPropertyRendering ren(item.propertyColoring, item.artefact.avgthickness);
    loadScalarField<SDFProperty>(item, &ren);
}

void InspectionViewMain::loadFacets(const InspectionItem &item)
{
    loadProperty<std::vector<Facet>>(item, item.facetOverlay);
    m_propertyToolbar->facetCheckBox->setEnabled(true);
}

void InspectionViewMain::loadBoundingBox(const InspectionItem &item)
{
    loadProperty<BoundingBox>(item, item.wireframeBox);
    m_propertyToolbar->facetCheckBox->setEnabled(true);
}

void InspectionViewMain::loadScalarFieldProperty(const InspectionItem &item, PropertyType type)
{
    // FIXME: Remove switch statement
    switch (type)
    {
    case PropertyType::MEAN_CURVATURE:
        loadMeanCurvature(item);
        break;
    case PropertyType::SHAPE_INDEX:
        loadShapeIndex(item);
        break;
    case PropertyType::LIGHTNESS:
        loadLightness(item);
        break;
    case PropertyType::SDF:
        loadSDF(item);
        break;
    default:
        break;
    }
}

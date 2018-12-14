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
#include "meshviewwidget.h"
#include "ui_meshviewwidget.h"

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkInteractorObserver.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <algorithm>

#include <QActionGroup>

#include "coordinateaxes.h"

void MeshViewWidget::initToolbar()
{
    m_toolBar = new QToolBar();

    ui->mainLayout->insertWidget(0, m_toolBar);

    // Initializing default tools
    m_toolBar->addAction(ui->actionSolid);
    m_toolBar->addAction(ui->actionWireframe);
    m_toolBar->addAction(ui->actionPoints);

    m_lightingButton = new QToolButton();
    m_lightingButton->setDefaultAction(ui->actionChange_Lighting);
    m_lightingButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_toolBar->addWidget(m_lightingButton);
    connect(ui->actionChange_Lighting, &QAction::triggered,
            this, &MeshViewWidget::changeLightingToNextConf);



    QActionGroup *visModeGroup = new QActionGroup(this);
    visModeGroup->addAction(ui->actionSolid);
    visModeGroup->addAction(ui->actionWireframe);
    visModeGroup->addAction(ui->actionPoints);
    visModeGroup->setExclusive(true);

    connect(ui->actionSolid, &QAction::toggled,
            [this](bool checked){
        if(checked) emit visualizationModeChanged(MeshVisualizationMode::SOLID);
    });
    connect(ui->actionWireframe, &QAction::toggled,
            [this](bool checked){
        if(checked) emit visualizationModeChanged(MeshVisualizationMode::WIREFRAME);
    });
    connect(ui->actionPoints, &QAction::toggled,
            [this](bool checked){
        if(checked) emit visualizationModeChanged(MeshVisualizationMode::POINTS);
    });

    ui->actionSolid->trigger();

    m_toolBar->addSeparator();
    m_toolBar->addAction(ui->actionReset_Camera);

    connect(ui->actionReset_Camera, &QAction::triggered,
            this, &MeshViewWidget::resetCameraToDefault);


    clearInfo();
}

void MeshViewWidget::initLightingConfigurations()
{
    m_lightingConfs = {
        { "A", { 45.0, -20.0 }, { 30.0,  20.0 }},
        { "B", { 45.0,  20.0 }, { 30.0, -20.0 }},
        { "C", { 20.0,  60.0 }, { 10.0, -60.0 }},
        { "D", { 20.0, -60.0 }, { 10.0,  60.0 }}
    };

    m_lightKit->SetKeyLightIntensity(0.8);
    m_lightKit->SetKeyToFillRatio(8);

    setLightingConfiguration(0);
}

void MeshViewWidget::setLightingConfiguration(int index)
{
    if(index < 0 || index >= m_lightingConfs.size())
    {
        qCritical() << "Invalid lighting configuration selected:" << index;
        return;
    }

    auto conf = m_lightingConfs.at(index);

    m_lightKit->SetKeyLightAngle(conf.keyLightAngle);
    m_lightKit->SetFillLightAngle(conf.fillLightAngle);

    ui->actionChange_Lighting->setText(conf.label);

    m_currentLightConf = index;

    update();
}

MeshViewWidget::MeshViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshViewWidget)
{
    ui->setupUi(this);
    initVisualizationContext();
    initLightingConfigurations();
    initToolbar();
}

MeshViewWidget::~MeshViewWidget()
{
    delete ui;
}

void MeshViewWidget::saveCameraState()
{
    m_camera->GetPosition(m_defaultPosition);
    m_camera->GetViewUp(m_defaultViewUp);
    m_camera->GetFocalPoint(m_defaultFocalPoint);
}

void MeshViewWidget::loadCameraState()
{
    m_camera->SetPosition(m_defaultPosition);
    m_camera->SetViewUp(m_defaultViewUp);
    m_camera->SetFocalPoint(m_defaultFocalPoint);
}

void MeshViewWidget::clearInfo()
{
    setInfo("");
}

void MeshViewWidget::setInfo(const QString &info)
{
    ui->geometricInfoLabel->setText(info);
}

void MeshViewWidget::changeLightingToNextConf()
{
    int newLightConf = m_currentLightConf + 1;

    if(newLightConf == m_lightingConfs.size())
    {
        newLightConf = 0;
    }

    setLightingConfiguration(newLightConf);
}

void MeshViewWidget::initVisualizationContext()
{
    auto rw = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    vtkNew<vtkGenericOpenGLRenderWindow> window;
    ui->vtkWidget->SetRenderWindow(rw);


    m_camera = vtkSmartPointer<vtkCamera>::New();

    // Layer 1 is reserved for vtkOrientationMarkerWidget, we need three layers
    rw->SetNumberOfLayers(3);

    // Initialize default renderer
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetActiveCamera(m_camera);
    m_renderer->SetLayer(0);
    m_renderer->UseHiddenLineRemovalOn();

    // AR: this renderer is for overlay props, such as vtkDistanceWidget
    m_overlayRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_overlayRenderer->SetActiveCamera(m_camera);
    m_overlayRenderer->SetLayer(2);

    // AR: the insertion order of the renderers should not affect the layers, but it does
    rw->AddRenderer(m_renderer);
    rw->AddRenderer(m_overlayRenderer);

    auto observer = rw->GetInteractor()->GetInteractorStyle();
    // AR: This is fundamental for avoiding clipping plane issues
    observer->SetDefaultRenderer(m_renderer);

    m_cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();

    // font size config for vtkCornerAnnotation
    m_cornerAnnotation->SetLinearFontScaleFactor( 2 );
    m_cornerAnnotation->SetNonlinearFontScaleFactor( 1 );
    m_cornerAnnotation->SetMaximumFontSize(20);

    m_lightKit = vtkSmartPointer<vtkLightKit>::New();
    m_lightKit->AddLightsToRenderer(m_renderer);

    saveCameraState();
}

vtkRenderer *MeshViewWidget::renderer()
{
    return m_renderer;
}

vtkRenderer *MeshViewWidget::overlayRenderer()
{
    return m_overlayRenderer;
}

vtkRenderWindow *MeshViewWidget::renderWindow()
{
    return ui->vtkWidget->GetRenderWindow();
}

vtkRenderWindowInteractor *MeshViewWidget::renderWindowInteractor()
{
    return ui->vtkWidget->GetInteractor();
}

QToolBar *MeshViewWidget::toolbar()
{
    return m_toolBar;
}

QAction *MeshViewWidget::addToolWidget(QWidget *widget)
{
    return m_toolBar->addWidget(widget);

    //ui->customToolsLayout->addWidget(widget);
}

void MeshViewWidget::setTextOverlay(const QString &text)
{
    std::string annotText = text.toStdString();

    m_cornerAnnotation->SetText(vtkCornerAnnotation::LowerLeft,
                                annotText.c_str() );
    update();
}

void MeshViewWidget::resetCameraToDefault()
{
    loadCameraState();
    m_renderer->ResetCamera();
    update();
}

void MeshViewWidget::clear()
{
    m_renderer->RemoveAllViewProps();
    m_overlayRenderer->RemoveAllViewProps();

    // This actor has been removed, we add it again
    // to the renderer
    m_renderer->AddActor(m_cornerAnnotation);

    emit sceneResetToDefaultState();
}



void MeshViewWidget::paintEvent(QPaintEvent *event)
{
    renderWindow()->Render();
    QWidget::paintEvent(event);
}

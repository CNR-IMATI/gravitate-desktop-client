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
#include "inspectionitemwidget.h"
#include "ui_inspectionitemwidget.h"

#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

#include "gravitatebackend.h"
#include "downloadprogressdialog.h"
#include "metadata3drequest.h"
#include "unitofmeasure.h"
#include "boundingbox.h"
#include "property/transferfunctionfactory.h"
#include "gravitaterepoutils.h"

#include "core/measuretool.h"

InspectionItemWidget::InspectionItemWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::InspectionItemWidget)
{
    ui->setupUi(this);
    m_mesh = new Mesh(ui->meshWidget);

    // Init value picker
    m_valuePicker = new ValuePicker(ui->meshWidget);
    connect(m_valuePicker, &ValuePicker::valueUpdated,
            [this] {
        if(!m_valuePicker->isValueSet())
        {
            ui->meshWidget->setTextOverlay("");
            return;
        }

        double value = m_valuePicker->currentValue();
        ui->meshWidget->setTextOverlay(QString::number(value));
    });

    m_axesOverlay = new CoordinateAxes(ui->meshWidget);

    m_measureTool = new MeasureTool(ui->meshWidget);

    connect(this, &InspectionItemWidget::modelLoaded,
            this, &InspectionItemWidget::onModelLoaded);

    connect(ui->deleteAnnotationButton, &QAbstractButton::pressed,
            this, &InspectionItemWidget::onAnnotationDeleteRequest);

    initStateMachine();

    setAnnotationsEditable(false);
}

InspectionItemWidget::~InspectionItemWidget()
{
    delete ui;
}

void InspectionItemWidget::initStateMachine()
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

void InspectionItemWidget::addToolWidget(QWidget *widget)
{
    ui->meshWidget->addToolWidget(widget);
}

void InspectionItemWidget::setArtefact(const Artefact3DMetadata &artefact)
{
    m_artefact = artefact;
    updateArtefactInfoDisplay();
}

const Artefact3DMetadata &InspectionItemWidget::artefact() const
{
    return m_artefact;
}

void InspectionItemWidget::setOverlayText(const QString &text)
{
    ui->meshWidget->setTextOverlay(text);
}

QString InspectionItemWidget::currentModelPath() const
{
    return m_currentModelPath;
}

bool InspectionItemWidget::isModelLoaded() const
{
    return !m_currentModelPath.isEmpty();
}

QState *InspectionItemWidget::initialState()
{
    return s_initial;
}

QState *InspectionItemWidget::modelLoadedState()
{
    return s_modelLoaded;
}

void InspectionItemWidget::setMetadataModel(MetadataModel *model)
{
    // AR: why is setParent needed?
    model->setParent(this);
    ui->metadataTreeView->setModel(model);
}

void InspectionItemWidget::setAnnotationModel(AnnotationModelBase *model)
{
    ui->annotationsListView->setModel(model);
}

void InspectionItemWidget::load3DModelFromFile(const QString &fileName)
{
    ui->meshWidget->clear();
    m_mesh->loadModel(fileName);
    m_currentModelPath = fileName;
    emit modelLoaded();
}

void InspectionItemWidget::updateArtefactInfoDisplay()
{
    auto unitString = Artefact3DMetadata::unitString(m_artefact.unit);

    auto w = ui->meshWidget;

        QString info = tr("Vertices: %1\t"
                          "Triangles: %2\t"
                          "Area: %3 %6^2\t"
                          "Volume: %4 %6^3")
                .arg(m_mesh->vertexCount())
                .arg(m_mesh->triangleCount())
                .arg(artefact().area)
                .arg(artefact().volume)
                .arg(unitString);
            w->setInfo(info);

    auto artefactId = m_artefact.id;
    setWindowTitle(artefactId.toString());
}

void InspectionItemWidget::onModelLoaded()
{
    updateArtefactInfoDisplay();

    m_valuePicker->setActiveMesh(m_mesh);

    auto unitString = Artefact3DMetadata::unitString(m_artefact.unit);

    m_measureTool->setUnitOfMeasureString(unitString);
    m_measureTool->setActiveMesh(m_mesh);
}

void InspectionItemWidget::onAnnotationDeleteRequest()
{
    auto sel = ui->annotationsListView->selectionModel();
    auto index = sel->currentIndex();

    emit annotationDeleted(index);
}

MeshViewWidget *InspectionItemWidget::meshViewWidget() const
{
    return ui->meshWidget;
}

void InspectionItemWidget::setAnnotationsEditable(bool editable)
{
    ui->annotationToolBar->setVisible(editable);
}

Mesh *InspectionItemWidget::mesh() const
{
    return m_mesh;
}

void InspectionItemWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

void InspectionItemWidget::setUnitOfMeasure(qudt::Unit unit)
{

}


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
#ifndef INSPECTIONITEMWIDGET_H
#define INSPECTIONITEMWIDGET_H

#include <QDockWidget>
#include "artefact3dmetadata.h"
#include "core/mesh.h"
#include "abstractview.h"
#include "core/coordinateaxes.h"
#include "core/valuepicker.h"
#include "core/measuretool.h"
#include "metadatamodel.h"
#include "core/meshviewwidget.h"
#include "enums.h"
#include "property/propertycoloring.h"
#include "property/wireframebox.h"
#include "facet/facetoverlay.h"
#include "localannotationmodel.h"
#include <QStateMachine>

namespace Ui {
class InspectionItemWidget;
}

class InspectionItemWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit InspectionItemWidget(QWidget *parent = 0);
    ~InspectionItemWidget();

    void addToolWidget(QWidget *widget);

    void setArtefact(const Artefact3DMetadata &artefact);

    const Artefact3DMetadata &artefact() const;

    void setOverlayText(const QString &text);

    QString currentModelPath() const;

    bool isModelLoaded() const;

    QState *initialState();

    QState *modelLoadedState();

    // New functions

    void setMetadataModel(MetadataModel *model);

    void setAnnotationModel(AnnotationModelBase *model);

    // Changed visibility
    Mesh * mesh() const;

    MeshViewWidget *meshViewWidget() const;

    void setAnnotationsEditable(bool editable);

    void updateArtefactInfoDisplay();

signals:
    void widgetClosed();

    void modelLoaded();

    void annotationRequested();

    void annotationDeleted(const QModelIndex &index);

public slots:

    void load3DModelFromFile(const QString &fileName);

    void setUnitOfMeasure(qudt::Unit unit);

private slots:

    void onModelLoaded();

    void onAnnotationDeleteRequest();

private:
    Ui::InspectionItemWidget *ui;
    Mesh * m_mesh;

    CoordinateAxes *m_axesOverlay;

    ValuePicker *m_valuePicker;

    MeasureTool *m_measureTool;

    Artefact3DMetadata m_artefact;

    QString m_currentModelPath;

    void initStateMachine();

protected:

    // QWidget interface
    QStateMachine *m_stateMachine;

    QState *s_initial;
    QState *s_modelLoaded;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // INSPECTIONITEMWIDGET_H

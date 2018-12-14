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
#ifndef STANDALONEMESHVIEWER_H
#define STANDALONEMESHVIEWER_H

#include <QMainWindow>
#include <QFile>
#include <QStateMachine>
#include <QState>

#include "thesaurusmodel.h"
#include "core/mesh.h"
#include "core/coordinateaxes.h"
#include "enums.h"
#include "property/propertycoloring.h"
#include "tweakparamwidget.h"
#include "vtk/interactorstylevertexpicker.h"
#include "annotation/selectiontool.h"
#include "annotation/annotationtool.h"
#include "localannotationmodel.h"
#include "facet/facetoverlay.h"
#include "property/wireframebox.h"


namespace Ui {
class StandaloneMeshViewer;
}

class StandaloneMeshViewer : public QMainWindow
{
    Q_OBJECT
    friend class StandaloneAnnotationHelper;

public:
    explicit StandaloneMeshViewer(QWidget *parent = 0);
    ~StandaloneMeshViewer();

    bool isTermSelected();

    Term currentSelectedPartonomyTerm();

signals:
    void annotationAdded(const PartAnnotation &part);

private slots:
    void on_actionLoad_Mesh_triggered();

    void on_actionLoad_Geometric_Property_triggered();

    void onMeshLoaded();

    void onPropertyLoaded();

    void onPropertyCustomColorApplied();

    void onAnnotationSave();

    void on_actionLoad_Annotations_triggered();

    void on_actionLoad_Facets_triggered();

    void on_actionLoad_Bounding_Box_triggered();

    void onSelectionSave();

    void onSelectionLoad();

private slots:
    void openAnnotationDialog();

    void onCustomVocabularyLoad();

private:
    Ui::StandaloneMeshViewer *ui;
    QString m_titleBase;

    void initStateMachine();

    void initDebugTab();

    void initPropertyLoading();

    void initComboBox();

    void initAnnotation();

    void initFaceting();

    void loadPartonomy(const QString &filename);

    qudt::Unit currentUnitOfMeasure();

    QModelIndex currentSelectedPartonomyTermIndex();

    QString m_currentFile;

    Mesh *m_mesh;

    PropertyColoring *m_coloring;

    TweakParamWidget *m_tweakPropertyColor;

    // Annotation-related members

    SelectionTool *m_selectionTool;

    AnnotationTool *m_annotationTool;

    ThesaurusModel *m_thesaurusModel;

    LocalAnnotationModel *m_annotationModel;

    // Facet-related members

    FacetOverlay *m_facetOverlay;

    // Scalar field properties

    bool m_propertyLoaded = false;

    vtkSmartPointer<InteractorStyleVertexPicker> m_vertexPickerStyle;

    QComboBox *m_unitOfMeasureComboBox;

    // Minimal bounding box

    WireframeBox *m_wireframeBox;

    // State machine

    QStateMachine *m_stateMachine;

    QState *s_initial;
    QState *s_modelLoaded;
    void initSelection();
};

#endif // STANDALONEMESHVIEWER_H

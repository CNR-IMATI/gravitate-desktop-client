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
#ifndef ANNOTATIONTOOL_H
#define ANNOTATIONTOOL_H

#include <QObject>
#include "core/viewermodule.h"
#include "core/mesh.h"
#include "partannotation.h"

/**
 * @brief The AnnotationTool class takes care of offering a visual
 * representation of the annotations, including actions for adding/removing
 * annotations on the target Mesh object.
 *
 * This class does not take care of saving the annotations in the system.
 *
 */
class AnnotationTool : public ViewerModule
{
    Q_OBJECT

public:
    explicit AnnotationTool(Mesh *mesh, MeshViewWidget *parent = 0);

    void onToolbarInit(QToolBar *toolBar) override;

    void addAnnotation(const PartAnnotation anno);

    void addAnnotation(const QString &label, const QVector<unsigned int> &indices, const QColor &color);

    void removeAnnotation(int index);

    void setEnabled(bool enabled) override;

    void setAnnotationsVisible(bool visible);

    void setReadOnly(bool value);

    bool isReadOnly();

signals:
    void annotationRequested();

public slots:
    void clearAnnotations();

private:

    vtkSmartPointer<vtkActor> create3DActor(const QVector<unsigned int> &indices, const QColor &color);

    QList<vtkSmartPointer<vtkActor>> m_actors;

    Mesh * m_mesh;

    QAction *action_Annotate;

    bool m_readOnly = false;
};

#endif // ANNOTATIONTOOL_H

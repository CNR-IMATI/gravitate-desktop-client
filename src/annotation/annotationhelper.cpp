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
#include <QMessageBox>

#include "annotationhelper.h"
#include "partannotation.h"
#include "annotationdialog.h"

AnnotationHelper::AnnotationHelper(SelectionTool *selectionTool,
                                   AnnotationTool *annotationTool,
                                   AnnotationModelBase *annotationModel,
                                   QWidget *parentWidget)
    : QObject(parentWidget),
      m_selectionTool(selectionTool),
      m_annotationTool(annotationTool),
      m_annotationModel(annotationModel),
      m_parentWidget(parentWidget)
{

}

void AnnotationHelper::onAnnotationRequest(const QUrl &modelUrl, const Term &term)
{
    auto vertices = m_selectionTool->selectedVertices();
    if(vertices.count() == 0)
    {
        QMessageBox::warning(m_parentWidget,
                             tr("Error"),
                             tr("Please select a region of the mesh"));
        return;
    }

    auto tag = QString::fromStdString(term.id);
    auto label = QString::fromStdString(term.label);

    AnnotationDialog dialog;
    dialog.setTag(tag);
    dialog.setLabel(label);



    int ret = dialog.exec();

    if(ret != QDialog::Accepted)
    {
        return;
    }

    auto color = dialog.color();

    // Gather information and save
    PartAnnotation annotation;

    annotation.uri = tag.toStdString();
    annotation.label = label.toStdString();
    annotation.color.r = color.red();
    annotation.color.g = color.green();
    annotation.color.b = color.blue();
    annotation.indices = vertices.toStdVector();

    saveAnnotation(annotation, modelUrl);

    m_selectionTool->clearSelection();
}

bool AnnotationHelper::loadAnnotations(const QUrl &modelUrl)
{
    if(m_annotationTool == nullptr)
        return false;

    if(m_annotationModel == nullptr)
        return false;

    m_annotationTool->clearAnnotations();

    m_annotationModel->loadAnnotations(modelUrl);

    for (auto anno : m_annotationModel->annotations())
    {
        m_annotationTool->addAnnotation(anno);
    }

    return true;
}

PartAnnotation AnnotationHelper::getSelectAnnotation(const Term &term, bool *ok)
{
    PartAnnotation annotation;

    auto vertices = m_selectionTool->selectedVertices();
    if(vertices.count() == 0)
    {
        QMessageBox::warning(m_parentWidget,
                             QObject::tr("Error"),
                             QObject::tr("Please select a region of the mesh"));
        if(ok != nullptr)
            *ok = false;
        return annotation;
    }

    auto tag = QString::fromStdString(term.id);
    auto label = QString::fromStdString(term.label);

    AnnotationDialog dialog;
    dialog.setTag(tag);
    dialog.setLabel(label);

    int ret = dialog.exec();

    if(ret != QDialog::Accepted)
    {
        if(ok != nullptr)
            *ok = false;
        return annotation;
    }

    auto color = dialog.color();

    annotation.uri = tag.toStdString();
    annotation.label = label.toStdString();
    annotation.color.r = color.red();
    annotation.color.g = color.green();
    annotation.color.b = color.blue();
    annotation.indices = vertices.toStdVector();

    m_selectionTool->clearSelection();

    return annotation;
}

void AnnotationHelper::saveAnnotation(const PartAnnotation &annotation, const QUrl &modelUrl)
{
    if(!m_annotationModel->saveAnnotation(modelUrl, annotation))
    {
        QMessageBox::critical(m_parentWidget, tr("Error"), tr("Could not save annotation"));
        return;
    }

    m_annotationTool->addAnnotation(annotation);

}

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
#ifndef ANNOTATIONHELPER_H
#define ANNOTATIONHELPER_H

#include "term.h"
#include "mesh/annotation/selectiontool.h"
#include "mesh/annotation/annotationtool.h"
#include "partannotation.h"
#include "annotationmodelbase.h"
#include <QObject>
#include <QWidget>
#include <QUrl>

class AnnotationHelper : public QObject
{
    Q_OBJECT
public:
    explicit AnnotationHelper(SelectionTool *selectionTool,
                     AnnotationTool *annotationTool,
                     AnnotationModelBase *annotationModel,
                     QWidget *parentWidget = nullptr);

    void onAnnotationRequest(const QUrl &modelUrl, const Term &term);
    void onAnnotationDelete(const QModelIndex &index);

    virtual bool loadAnnotations(const QUrl &modelUrl);

    PartAnnotation Q_DECL_DEPRECATED getSelectAnnotation(const Term &term, bool *ok = nullptr);

protected:
    virtual void saveAnnotation(const PartAnnotation &annotation, const QUrl &modelUrl);

protected:
    SelectionTool *m_selectionTool;
    AnnotationTool *m_annotationTool;
    AnnotationModelBase *m_annotationModel;
    QWidget *m_parentWidget;

};

#endif // ANNOTATIONHELPER_H

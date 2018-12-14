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
#ifndef ANNOTATIONMODELBASE_H
#define ANNOTATIONMODELBASE_H

#include <QObject>
#include <QUrl>
#include <QAbstractListModel>
#include <QList>
#include "partannotation.h"

class AnnotationModelBase : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AnnotationModelBase(QObject *parent = Q_NULLPTR);

    virtual bool loadAnnotations(const QUrl &modelUrl) = 0;

    virtual bool saveAnnotation(const QUrl &modelUrl, const PartAnnotation &annotation) = 0;

    virtual void removeAnnotation(const QModelIndex &index) = 0;


    /**
     * @brief annotations Returns the list of part annotations on the 3D model
     * @return The list of part annotations
     */
    virtual QList<PartAnnotation> annotations();

protected:
    QList<PartAnnotation> m_annotations;
};

#endif // ANNOTATIONMODELBASE_H

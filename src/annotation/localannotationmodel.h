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
#ifndef LOCALANNOTATOR_H
#define LOCALANNOTATOR_H

#include <QString>
#include <QObject>
#include <QVector>
#include <QDir>
#include <QAbstractListModel>
#include "annotationmodelbase.h"
#include "partannotation.h"
#include <list>
#include <vector>

class LocalAnnotationModel : public AnnotationModelBase
{
    Q_OBJECT
public:
    explicit LocalAnnotationModel(QObject *parent = Q_NULLPTR);


    bool loadAnnotations(const QUrl &modelUrl) override;

    /**
     * @brief saveAnnotation Saves the annotation
     * @param modelUrl URL of the target 3D model
     * @param annotation The annotation body
     * @return true if the annotation has been saved successfully, false otherwise
     */
    bool saveAnnotation(const QUrl &modelUrl, const PartAnnotation &annotation) override;
    /**
     * @brief clearAnnotations clears the current annotations; useful for
     * resetting the UI to the initial state
     */
    void clearAnnotations();

    void removeAnnotation(const QModelIndex &index) override;

    // removed from public interface
private:
    /**
     * @brief create Creates a new annotation index
     * @param indexFile The path of the index file
     * @return true if the operation has succeeded, false otherwise
     */
    bool create(const QString &indexFile);

    /**
     * @brief Loads an existing annotation index
     * @param indexFile The path of the index file
     * @return
     */
    bool load(const QString &indexFile);

private:
    QUrl m_modelUrl;

    std::vector<std::string> m_annotationIndex;

    QString randomPartId();

    bool readAnnotationFile(PartAnnotation &annotation, const QString &filename);

    bool writeAnnotationFile(const PartAnnotation &annotation, QString filename);

    void appendAnnotationFileToIndex(const QString &fileName, const QString &indexFile);

    bool readAnnotationIndex(const QString &indexFile);

    bool writeAnnotationIndex(const QString &indexFile);

    QString annotationsBasePath(const QString &indexFile);

    QString createAnnotationFileName();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QString getIndexFileLocation(const QUrl &modelUrl);

    QByteArray computeHash(const QUrl &modelUrl);

    void removeAnnotationFile();
};

#endif // LOCALANNOTATOR_H

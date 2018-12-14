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
#include "localannotationmodel.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUuid>
#include <QPixmap>
#include <QCryptographicHash>

#include <iostream>
#include <fstream>

#include <QStandardPaths>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

LocalAnnotationModel::LocalAnnotationModel(QObject *parent)
    : AnnotationModelBase (parent)
{

}

bool LocalAnnotationModel::create(const QString &indexFile)
{
    QFileInfo info(indexFile);

    QString indexDir = info.absolutePath();


    if(!QDir().mkpath(indexDir))
    {
        qCritical() << "Could not create directory" << indexDir;
        return false;
    }

    auto basePath = annotationsBasePath(indexFile);

    if(!QDir().mkpath(basePath))
    {
        qCritical() << "Could not create directory" << basePath;
        return false;
    }

    QFile file(indexFile);
    if(!file.open(QFile::WriteOnly))
    {
        qCritical() << "Could not create file" << info.fileName();
        return false;
    }
    file.close();

    clearAnnotations();
    return true;
}

bool LocalAnnotationModel::load(const QString &indexFile)
{
    if(!readAnnotationIndex(indexFile))
    {
        return false;
    }

    clearAnnotations();

    for(auto file: m_annotationIndex)
    {
        PartAnnotation ann;
        QFileInfo info(indexFile);

        auto qFile = QString::fromStdString(file);

        auto path = info.absoluteDir().absoluteFilePath(qFile);

        if(!readAnnotationFile(ann, path ))
        {
            continue;
        }
        m_annotations.append(ann);
    }

    beginInsertRows(QModelIndex(), 0, m_annotations.count());
    endInsertRows();
    return true;
}

bool LocalAnnotationModel::saveAnnotation(const QUrl &modelUrl, const PartAnnotation &annotation)
{
    auto indexFile = getIndexFileLocation(modelUrl);
    if(indexFile.isEmpty())
    {
        qCritical() << "Invalid model URL; cannot annotate this model";
        return false;
    }

    QFileInfo info(indexFile);

    auto subdir = info.dir().filePath(info.baseName());

    if(!QDir().mkpath(subdir))
    {
        qCritical() << "Could not create directory" << subdir;
        return false;
    }

    auto annotationAbsolutePath = QDir(subdir).filePath(createAnnotationFileName());

    // Write annotation file
    if(!writeAnnotationFile(annotation, annotationAbsolutePath))
    {
        return false;
    }

    auto row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_annotations.append(annotation);
    endInsertRows();

    appendAnnotationFileToIndex(annotationAbsolutePath, indexFile);

    if(!writeAnnotationIndex(indexFile))
    {
        return false;
    }

    return true;
}

QString LocalAnnotationModel::randomPartId()
{
    QString withBraces = QUuid::createUuid().toString();
    return withBraces.remove("{").remove("}");
}

bool LocalAnnotationModel::readAnnotationFile(PartAnnotation &annotation, const QString &filename)
{
    try
    {
        std::ifstream is(filename.toStdString());

        json j;

        is >> j;

        annotation = j;

        qDebug() << "Loaded annotation file:" << filename;

        return true;
    }
    catch(std::exception &e)
    {
        qCritical() << "Could not read file:" << filename;
        qCritical() << e.what();
        return false;
    }

}

bool LocalAnnotationModel::writeAnnotationFile(const PartAnnotation &annotation, QString filename)
{
    try
    {
        std::ofstream os(filename.toStdString());

        json j = annotation;

        os << j;

        qDebug() << "Created annotation file:" << filename;

        return true;
    }
    catch(std::exception &)
    {
        qCritical() << "Could not write file:" << filename;
        return false;
    }
}

void LocalAnnotationModel::appendAnnotationFileToIndex(const QString &fileName, const QString &indexFile)
{
    QFileInfo info(indexFile);

    // The path of the file is relative to the directory of the index
    // file, so the annotation folder can be moved to a different location
    auto entry = info.dir().relativeFilePath(fileName);

    m_annotationIndex.push_back(entry.toStdString());
}

bool LocalAnnotationModel::readAnnotationIndex(const QString &indexFile)
{
    try
    {
        std::ifstream is(indexFile.toStdString());

        json j;

        is >> j;

        m_annotationIndex = j.get<std::vector<std::string>>();

        return true;
    }
    catch(std::exception &)
    {
        return false;
    }
}

bool LocalAnnotationModel::writeAnnotationIndex(const QString &indexFile)
{
    try
    {
        std::ofstream os(indexFile.toStdString());

        json j;

        j = m_annotationIndex;

        os << j;

        return true;
    }
    catch(std::exception &)
    {
        return false;
    }
}

void LocalAnnotationModel::clearAnnotations()
{
    beginResetModel();
    m_annotations.clear();
    endResetModel();
}

void LocalAnnotationModel::removeAnnotation(const QModelIndex &index)
{
    if(!index.isValid())
    {
        qDebug() << Q_FUNC_INFO;
        qDebug() << "Invalid index";
        return;
    }

    if(!m_modelUrl.isValid())
    {
        qDebug() << "invalid m_modelUrl value:" << m_modelUrl.toString() << Q_FUNC_INFO;
        return;
    }

    auto row = index.row();
    auto annotationFile = m_annotationIndex[row];

    beginRemoveRows(QModelIndex(), row, row);
    m_annotations.removeAt(row);

    // Remove using iterators...
    auto it = m_annotationIndex.begin();
    std::advance(it, row);
    m_annotationIndex.erase(it);

    auto indexFile = getIndexFileLocation(m_modelUrl);

    if(!writeAnnotationIndex(indexFile))
    {
        qCritical() << "Error writing the index";
        qCritical() << "The application is in a dirty state";
    }

    QFileInfo info(indexFile);
    auto annotationFileToDelete = info.dir().filePath(QString::fromStdString(annotationFile));

    if(!QFile(annotationFileToDelete).remove())
    {
        qDebug() << "Error deleting file" << annotationFileToDelete;
    }

    endRemoveRows();
}

QString LocalAnnotationModel::annotationsBasePath(const QString &indexFile)
{
    QFileInfo info(indexFile);
    return info.absoluteDir().filePath(info.baseName());
}

QString LocalAnnotationModel::createAnnotationFileName()
{
    return randomPartId() + ".sem3d";
}

int LocalAnnotationModel::rowCount(const QModelIndex &parent) const
{
    return m_annotations.count();
}

QVariant LocalAnnotationModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    auto row = index.row();

    auto annotation = m_annotations[row];

    auto partUri = QUrl(QString::fromStdString(annotation.uri));
    auto label = QString::fromStdString(annotation.label);

    switch (role) {
    case Qt::DisplayRole:
        return label;
    case Qt::ToolTipRole:
        return partUri.toString();
    case Qt::DecorationRole:
    {
        QPixmap pixmap(32,32);
        QColor color(annotation.color.r,
                     annotation.color.g,
                     annotation.color.b);

        pixmap.fill(color);
        return pixmap;
    }
    default:
        return QVariant();
    }
}

bool LocalAnnotationModel::loadAnnotations(const QUrl &modelUrl)
{
    clearAnnotations();

    auto indexFile = getIndexFileLocation(modelUrl);

    if(indexFile.isEmpty())
    {
        qCritical() << "Invalid model URL; cannot load annotations";
        return false;
    }

    QFileInfo info(indexFile);

    bool indexFileExists = info.exists();

    bool ok = true;

    if(indexFileExists)
    {
        ok = load(indexFile);
    }

    if(ok)
    {
        m_modelUrl = modelUrl;
    }

    return ok;

}

QString LocalAnnotationModel::getIndexFileLocation(const QUrl &modelUrl)
{
    if(!modelUrl.isValid())
    {
        return QString::null;
    }

    auto hash = computeHash(modelUrl);
    if(hash.isEmpty())
    {
        return QString::null;
    }

    auto filename = hash.toHex().toLower();

    filename = filename.append(".json");

    auto appdataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto indexFile = QDir(appdataDir).absoluteFilePath("annotations/" + filename);

    return indexFile;
}

QByteArray LocalAnnotationModel::computeHash(const QUrl &modelUrl)
{
    QByteArray dataForChecksum;

    if(modelUrl.isLocalFile())
    {
        QFile file(modelUrl.toLocalFile());
        if(!file.open(QIODevice::ReadOnly))
        {
            return QByteArray();
        }
        dataForChecksum = file.readAll();
    }
    else
    {
        dataForChecksum = modelUrl.toEncoded();
    }

    return QCryptographicHash::hash(dataForChecksum, QCryptographicHash::Sha256);
}

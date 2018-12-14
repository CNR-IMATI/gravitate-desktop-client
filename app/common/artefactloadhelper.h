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
#ifndef ARTEFACTLOADHELPER_H
#define ARTEFACTLOADHELPER_H

#include "backendprovider.h"
#include "abstractdownloader.h"
#include <QWidget>
#include "artefact3dmetadata.h"
#include "enums.h"
#include "propertyselector.h"
#include "abstractdashboard.h"

namespace ArtefactLoading {

void initPropertySelector(PropertySelector *selector, PropertyType type);

template<class Property, class Viewer>
class Noop
{
public:
    Noop(Viewer *viewer)
        : m_viewer(viewer)
    {}

    void apply(const Property &property, qudt::Unit unit)
    {
        m_viewer->apply(property, unit);
    }

private:
    Viewer * m_viewer;
};

class ArtefactLoadHelper : public QObject
{
    Q_OBJECT
public:
    ArtefactLoadHelper(AbstractDashboard *dashboard, QObject *parent = 0);

    bool loadArtefactInfo(const ArtefactId &artefactId, Artefact3DMetadata &artefact);

    bool getLocalFile(const QUrl &remoteUrl, QString &localFile);

    template<class Property, class Viewer>
    bool loadLocalPropertyFile(const QString &file, Viewer *viewer, qudt::Unit unit)
    {
        QFileInfo info(file);

        qInfo() << "Loading file:" << info.fileName();

        try {
            Property p;

            load(file.toStdString(), p);

            viewer->apply(p, unit);
            return true;

        } catch (const std::exception &e) {

            emit error(e.what());
            return false;
        }
    }


signals:
    void error(const QString &errorMessage);

private:
    AbstractDashboard *m_dashboard;
    QString m_errorMessage;
};

}

#endif // ARTEFACTLOADHELPER_H

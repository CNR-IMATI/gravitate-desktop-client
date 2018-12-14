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
#include "gravitateextrametadata.h"
#include "csv.h"
#include <QDebug>
#include <QTemporaryFile>

GravitateExtraMetadata::GravitateExtraMetadata()
{
}

bool GravitateExtraMetadata::readCsv(const QString &fileName)
{
    auto temp = QTemporaryFile::createNativeFile(fileName);

    try {
        auto actualFileName = temp != 0 ?
                    temp->fileName().toStdString() :
                    fileName.toStdString();


        // Load data from CSV
        io::CSVReader<4,
                io::trim_chars<' ', '\t'>,
                io::double_quote_escape<',','"'>
                > in(actualFileName);
        in.read_header(io::ignore_missing_column,
                       "Inventory ID",
                       "URI",
                       "Thickness",
                       "Unit");

        // Reset dictionary
        m_metadata.clear();

        std::string inventoryId, uri, unit;
        double thickness;

        while (in.read_row(inventoryId, uri, thickness, unit)) {
            auto artefactId = ArtefactId::fromUri(QString::fromStdString(uri));

            auto qInventoryId = QString::fromStdString(inventoryId);
            auto qUnit = qudt::fromUri(unit);

            m_metadata.insert(artefactId, { qInventoryId, thickness, qUnit } );
        }

        return true;
    } catch (io::error::can_not_open_file &e) {
        qCritical() << e.what();
        return false;
    }
    temp->deleteLater();
}

bool GravitateExtraMetadata::hasInfo(const ArtefactId &artefactId)
{
    return m_metadata.contains(artefactId);
}

GravitateExtraMetadata::Info GravitateExtraMetadata::get(const ArtefactId &artefactId)
{
    return m_metadata.value(artefactId, m_nullInfo);
}

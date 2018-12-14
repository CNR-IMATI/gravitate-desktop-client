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
#include "artefact3dmetadata.h"

void Artefact3DMetadata::setUrl(ModelResolution res, MultiResProperty prop, const QUrl &url)
{
    m_multiResMap.insert({res, prop}, url);
}

QUrl Artefact3DMetadata::getUrl(ModelResolution res, MultiResProperty prop, bool useFallback) const
{
    auto value = m_multiResMap.value({res, prop});
    if(value.isEmpty() && useFallback)
    {
        value = m_multiResMap.value({ModelResolution::RES_FULL, prop});
    }

    return value;
}

bool Artefact3DMetadata::hasUrl(ModelResolution res, MultiResProperty prop) const
{
    return m_multiResMap.contains({res, prop});
}

QList<ModelResolution> Artefact3DMetadata::availableResolutions(MultiResProperty property) const
{
    QList<ModelResolution> list;

    for(auto k : m_multiResMap.keys())
    {
        if(k.second == property)
        {
            list.append(k.first);
        }
    }

    return list;
}

bool Artefact3DMetadata::isValid()
{
    return hasUrl(ModelResolution::RES_ORIGINAL, MultiResProperty::MESH) ||
           hasUrl(ModelResolution::RES_FULL, MultiResProperty::MESH);
}

ModelResolution Artefact3DMetadata::primaryResolution()
{
    if( hasUrl(ModelResolution::RES_ORIGINAL, MultiResProperty::MESH) )
    {
        return ModelResolution::RES_ORIGINAL;
    }
    return ModelResolution::RES_FULL;
}

qudt::Unit Artefact3DMetadata::fromQudt(const QUrl &url)
{
    auto uriString = url.toString().toStdString();
    return qudt::fromUri(uriString);
}

QString Artefact3DMetadata::unitString(qudt::Unit unit)
{
    auto info = qudt::getInfo(unit);
    return QString::fromStdString(info.abbreviation);
}

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
#ifndef ARTEFACTURLADAPTER_H
#define ARTEFACTURLADAPTER_H

#include "artefact3dmetadata.h"
#include <QUrl>
#include "enums.h"

#include "boundingbox.h"
#include "facet.h"
#include "scalarfieldproperty.h"

template<class T>
QUrl getUrl(ModelResolution res, const Artefact3DMetadata &artefact)
{
    Q_UNUSED(res);
    Q_UNUSED(artefact);
    return QUrl();
}

template <>
inline QUrl getUrl<BoundingBox>(ModelResolution res, const Artefact3DMetadata &artefact)
{
    Q_UNUSED(res);
    return artefact.mbbUrl;
}

template <>
inline QUrl getUrl<std::vector<Facet>>(ModelResolution res, const Artefact3DMetadata &artefact)
{
    // NOTE: this condition has been added because facets are not loaded on the original models
    // This condition is necessary to avoid that the facets are loaded on the original models
    if(res == ModelResolution::RES_ORIGINAL)
        return QUrl();

    return artefact.getUrl(res, MultiResProperty::FACETS, true);
}

template <>
inline QUrl getUrl<MeanCurvatureProperty>(ModelResolution res, const Artefact3DMetadata &artefact)
{
    return artefact.getUrl(res, MultiResProperty::MEAN_CURVATURE);
}

template <>
inline QUrl getUrl<ShapeIndexProperty>(ModelResolution res, const Artefact3DMetadata &artefact)
{
    return artefact.getUrl(res, MultiResProperty::SHAPE_INDEX);
}

template <>
inline QUrl getUrl<LightnessProperty>(ModelResolution res, const Artefact3DMetadata &artefact)
{
    return artefact.getUrl(res, MultiResProperty::LAB_COLOR);
}

template <>
inline QUrl getUrl<SDFProperty>(ModelResolution res, const Artefact3DMetadata &artefact)
{
    return artefact.getUrl(res, MultiResProperty::SDF);
}

#endif // ARTEFACTURLADAPTER_H

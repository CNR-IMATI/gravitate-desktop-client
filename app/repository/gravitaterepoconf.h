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
#ifndef GRAVITATEREPOCONF_H
#define GRAVITATEREPOCONF_H

#include <QString>
#include <QPair>
#include <QMap>
#include "enums.h"

namespace GravitateRepo
{
    // HACK: The "1m" suffix is used in the URIs. To be consistent with other resolutions, it should be "1M"
    const QString ID_3D_MODEL_FINAL = "http://gravitate-project.eu/id/object/3D_model_file/final";
    const QString ID_3D_MODEL_CLEAN = "http://gravitate-project.eu/id/object/3D_model_file/clean";
    const QString ID_3D_MODEL_1M    = "http://gravitate-project.eu/id/object/3D_model_file/1m";
    const QString ID_3D_MODEL_100K  = "http://gravitate-project.eu/id/object/3D_model_file/100K";
    const QString ID_3D_MODEL_50K   = "http://gravitate-project.eu/id/object/3D_model_file/50K";
    const QString ID_FACETING_FILE_CLEAN = "http://gravitate-project.eu/id/object/3D_faceted_model_file/clean";
    const QString ID_FACETING_FILE_1M    = "http://gravitate-project.eu/id/object/3D_faceted_model_file/1m";
    const QString ID_FACETING_FILE_100K  = "http://gravitate-project.eu/id/object/3D_faceted_model_file/100K";
    const QString ID_FACETING_FILE_50K   = "http://gravitate-project.eu/id/object/3D_faceted_model_file/50K";
    const QString ID_CURVM_1M = "http://gravitate-project.eu/id/object/curvature_measurement_for_resolution/1m";
    const QString ID_SI_1M = "http://gravitate-project.eu/id/object/shape_index/1m";
    const QString ID_LAB_FINAL = "http://gravitate-project.eu/id/object/dimension/lab";
    const QString ID_SDF_50K = "http://gravitate-project.eu/id/object/shape_diameter_function/50K";
    const QString ID_MBB = "http://gravitate-project.eu/id/object/minimal_bounding_box";
    const QString ID_AREA = "http://gravitate-project.eu/id/object/dimension/area";
    const QString ID_VOLUME = "http://gravitate-project.eu/id/object/dimension/volume";

    const QMap<QString, QPair<MultiResProperty, ModelResolution>> PropertyResolutionMapping = {
        // 3D Models
        { ID_3D_MODEL_FINAL, { MultiResProperty::MESH, ModelResolution::RES_ORIGINAL }},
        { ID_3D_MODEL_CLEAN, { MultiResProperty::MESH, ModelResolution::RES_FULL }},
        { ID_3D_MODEL_1M,    { MultiResProperty::MESH, ModelResolution::RES_1M }},
        { ID_3D_MODEL_100K,  { MultiResProperty::MESH, ModelResolution::RES_100K }},
        { ID_3D_MODEL_50K,   { MultiResProperty::MESH, ModelResolution::RES_50K }},
        // Faceting
        { ID_FACETING_FILE_CLEAN, { MultiResProperty::FACETS, ModelResolution::RES_FULL }},
        { ID_FACETING_FILE_1M,    { MultiResProperty::FACETS, ModelResolution::RES_1M }},
        { ID_FACETING_FILE_100K,  { MultiResProperty::FACETS, ModelResolution::RES_100K }},
        { ID_FACETING_FILE_50K,   { MultiResProperty::FACETS, ModelResolution::RES_50K }},
        // Properties
        { ID_CURVM_1M, { MultiResProperty::MEAN_CURVATURE, ModelResolution::RES_1M }},
        { ID_SI_1M,    { MultiResProperty::SHAPE_INDEX, ModelResolution::RES_1M }},
        { ID_LAB_FINAL,{ MultiResProperty::LAB_COLOR, ModelResolution::RES_ORIGINAL }},
        { ID_SDF_50K,  { MultiResProperty::SDF, ModelResolution::RES_50K }},
    };

    const QList<QString> AllowedPropertyTypeUri = {
        ID_3D_MODEL_FINAL,
        ID_3D_MODEL_CLEAN,
        ID_3D_MODEL_1M,
        ID_3D_MODEL_100K,
        ID_3D_MODEL_50K,
        ID_FACETING_FILE_CLEAN,
        ID_FACETING_FILE_1M,
        ID_FACETING_FILE_100K,
        ID_FACETING_FILE_50K,
        ID_CURVM_1M,
        ID_SI_1M,
        ID_LAB_FINAL,
        ID_SDF_50K,
        ID_MBB,
        ID_AREA,
        ID_VOLUME
    };
}

#endif // GRAVITATEREPOCONF_H

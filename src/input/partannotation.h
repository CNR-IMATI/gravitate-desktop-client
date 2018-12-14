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
#ifndef PARTANNOTATION_H
#define PARTANNOTATION_H

#include <QUrl>
#include <QVector>
#include <QColor>

#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

///
/// Currently, the annotation type is not used in the code;
/// The only supported type of annotation is currently the
/// "Triangles" annotation.
///
/// However, all the three types are encoded as a list of
/// vertex indices
enum class AnnotationType
{
    VERTICES,
    EDGES,
    TRIANGLES
};


// Note: currently, all the annotations are represented
// as a list of vertex indices; this representation may be
// changed at any time, since the specification has not
// been finalised yet
struct PartAnnotation
{
    struct RGB
    {
        int r; int g; int b;
    };

    std::string uri;
    std::string label;
    std::vector<unsigned int> indices;
    RGB color;
};

// TODO: move the to_json and from_json functions to a .cpp file

inline void to_json(json &j, const PartAnnotation &anno)
{
    j = json{
        {"uri", anno.uri},
        {"label", anno.label},
        {"color", anno.color},
        {"indices", anno.indices}
    };
}

inline void from_json(const json &j, PartAnnotation &anno)
{
    anno.uri =     j.at("uri").get<std::string>();
    anno.label =   j.at("label").get<std::string>();
    anno.color =   j.at("color").get<PartAnnotation::RGB>();
    anno.indices = j.at("indices").get<std::vector<unsigned int>>();
}

inline void to_json(json &j, const PartAnnotation::RGB &color)
{
    j = json{
        {"r", color.r},
        {"g", color.g},
        {"b", color.b}
    };
}

inline void from_json(const json &j, PartAnnotation::RGB &color)
{
    color.r = j.at("r").get<int>();
    color.g = j.at("g").get<int>();
    color.b = j.at("b").get<int>();
}

#endif // PARTANNOTATION_H

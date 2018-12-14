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
#ifndef HOUGHFEATURE_H
#define HOUGHFEATURE_H

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct HoughFeature
{
    struct Axis
    {
        std::array<double, 3> p1;
        std::array<double, 3> p2;
    };

    std::vector<double> params;
    Axis axis1;
    Axis axis2;
    std::vector<unsigned int> vertices;
};


inline void from_json(const json& j, HoughFeature& f) {
    f.params   = j.at("param").get<std::vector<double>>();
    f.axis1    = j.at("axis1").get<HoughFeature::Axis>();
    f.axis2    = j.at("axis2").get<HoughFeature::Axis>();
    f.vertices = j.at("vertices").get<std::vector<unsigned int>>();
}

inline void from_json(const json& j, HoughFeature::Axis& a) {
    a.p1 = j.at("Pt1").get<std::array<double, 3>>();
    a.p2 = j.at("Pt2").get<std::array<double, 3>>();
}

class HoughFeatureReader
{
public:
    std::vector<HoughFeature> readJson(const std::string &fileName)
    {
        std::ifstream is(fileName);
        return parse(is);
    }

    std::vector<HoughFeature> parse(std::istream &stream)
    {
        json j;
        stream >> j;

        std::vector<HoughFeature> features = j["features"];

        return features;
    }
};

inline void load(const std::string &file, std::vector<HoughFeature> &featureVector)
{
    HoughFeatureReader reader;

    featureVector = reader.readJson(file);
}

#endif // HOUGHFEATURE_H

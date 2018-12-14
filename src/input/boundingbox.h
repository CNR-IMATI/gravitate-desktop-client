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
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <vector>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct BoundingBox
{
    std::vector<double> vertices;
    std::vector<double> length;
    double diam;

    bool readJson(std::string fileName)
    {
        try {
            std::ifstream is(fileName);

            json doc;

            is >> doc;

            std::vector<std::vector<double>> coords;

            coords = doc.at("coord").get<std::vector<std::vector<double>>>();
            vertices = flatten(coords);

            length = doc.at("length").get<std::vector<double>>();
            diam = doc.at("diam").get<double>();
            return true;

        } catch (std::exception &) {
            return false;
        }
    }

private:
    std::vector<double> flatten(const std::vector<std::vector<double>> &coords)
    {
        std::vector<double> out;
        for( auto c : coords)
        {
            out.push_back(c[0]);
            out.push_back(c[1]);
            out.push_back(c[2]);
        }
        return out;
    }
};

inline void load(const std::string &file, BoundingBox &bb)
{
    if(!bb.readJson(file))
    {
        throw std::runtime_error("Error while parsing Bounding Box file");
    }
}

#endif // BOUNDINGBOX_H

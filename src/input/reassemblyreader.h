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
#ifndef REASSEMBLYREADER_H
#define REASSEMBLYREADER_H

#include <array>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct PartialAssemblyFragment
{
    std::string fileName;

    std::array<double, 9> rotation;
    std::array<double, 3> translation;

    std::array<double, 16> getAffineTransformMatrix()
    {
        return {
            rotation[0], rotation[1], rotation[2], translation[0],
            rotation[3], rotation[4], rotation[5], translation[1],
            rotation[6], rotation[7], rotation[8], translation[2],
            0, 0, 0, 1
        };
    }
};

struct PartialAssemblyPair
{
    enum class State
    {
        UNKNOWN,
        ACCEPTED,
        REJECTED
    };

    State mode;

    PartialAssemblyFragment first;
    PartialAssemblyFragment second;
};

class ReAssemblyReader
{
public:
    ReAssemblyReader();

    bool parseJson(std::istream &stream);

    bool readJson(const std::string &fileName);

    std::vector<PartialAssemblyPair> pairs();

private:
    void parseJsonDocument(const json &doc);

    PartialAssemblyFragment parseFragment(const json &map);

    std::array<double,9> parseRotationMatrix(const json &json);
    std::array<double,3> parseTranslationVector(const json &json);

public:
    std::vector<PartialAssemblyPair> m_pairs;
};

#endif // REASSEMBLYREADER_H

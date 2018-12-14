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
#include "reassemblyreader.h"
#include <fstream>

namespace {
    const char* ATTR_FRAGMENT = "Fragment";
    const char* ATTR_FILENAME = "FName";
    const char* ATTR_ROTATION = "R";
    const char* ATTR_TRANSLATION = "T";
    const char* ATTR_MODE = "Mode";
}

ReAssemblyReader::ReAssemblyReader()
{



}

bool ReAssemblyReader::parseJson(std::istream &stream)
{
    try {
        json doc;
        stream >> doc;

        parseJsonDocument(doc);

        return true;
    } catch (std::exception) {
        return false;
    }
}

bool ReAssemblyReader::readJson(const std::string &fileName)
{
    std::ifstream is(fileName);
    return parseJson(is);
}

std::vector<PartialAssemblyPair> ReAssemblyReader::pairs()
{
    return m_pairs;
}

void ReAssemblyReader::parseJsonDocument(const json &doc)
{
    for(auto i : doc)
    {
        PartialAssemblyPair pair;
        auto p1 = i;
        auto p2 = p1[ATTR_FRAGMENT];

        pair.first = parseFragment(p1);
        pair.second = parseFragment(p2);

        m_pairs.push_back(pair);
    }
}

PartialAssemblyFragment ReAssemblyReader::parseFragment(const json &map)
{
    PartialAssemblyFragment fragment;
    fragment.fileName = map[ATTR_FILENAME];
    fragment.rotation = parseRotationMatrix(map[ATTR_ROTATION]);
    fragment.translation = parseTranslationVector(map[ATTR_TRANSLATION]);

    return fragment;
}

std::array<double,9> ReAssemblyReader::parseRotationMatrix(const json &json)
{
    std::array<double, 9> outR;
    int i = 0;
    for(auto row : json)
    {
        outR[i*3]   = row[0];
        outR[i*3+1] = row[1];
        outR[i*3+2] = row[2];
        i++;
    }
    return outR;
}

std::array<double,3> ReAssemblyReader::parseTranslationVector(const json &json)
{
    return json;
}

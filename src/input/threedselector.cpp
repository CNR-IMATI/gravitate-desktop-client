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
#include "threedselector.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

std::vector<size_t> rle_compress(std::vector<size_t> indices)
{
    return indices;
}

std::vector<size_t> rle_decompress(std::vector<size_t> indices)
{
    return indices;
}

}

ThreeDSurfaceSelectorWriter::ThreeDSurfaceSelectorWriter(ThreeDSurfaceSelector &selector, IndexCompression compression)
{
    m_selector = selector;
    m_compression = compression;
}

void ThreeDSurfaceSelectorWriter::saveJson(const std::string &fileName)
{
    std::ofstream os(fileName);

    saveJson(os);
}

void ThreeDSurfaceSelectorWriter::saveJson(std::ostream &os)
{   
    json j;

    switch (m_compression) {
    case IndexCompression::NONE:
        j["compression"] = "None";
        j["indices"] = m_selector.indices;
        break;
    case IndexCompression::RLE:
        j["compression"] = "RLE";
        j["indices"] = rle_compress(m_selector.indices);
        break;
    }

    os << j;
}
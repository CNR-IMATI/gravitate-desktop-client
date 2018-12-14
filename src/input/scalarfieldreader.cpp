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
#include "scalarfieldreader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

ScalarFieldReader::ScalarFieldReader(int columnNumber)
{
    m_data_columns = std::vector<std::vector<double>>(size_t(columnNumber), std::vector<double>());
}

int ScalarFieldReader::columnNumber()
{
    return int(m_data_columns.size());
}

std::vector<double> ScalarFieldReader::getColumn(int index)
{
    if( index > columnNumber() - 1 )
    {
        std::cerr << "Column " << index << " not found" << std::endl;
        return std::vector<double>();
    }
    return m_data_columns.at(size_t(index));
}

bool ScalarFieldReader::readTxt(const std::string &file)
{
    std::fstream fs;
    fs.open(file, std::fstream::in);
    if(!fs.is_open())
    {
        return false;
    }

    double value;
    std::string line;
    while(std::getline(fs, line))
    {
        if(line.empty())
            continue;

        std::istringstream ss(line);

        for( int i = 0; i < columnNumber(); i++)
        {
            ss >> value;
            if(std::isnan(value))
            {
                return false;
            }
            m_data_columns[i].push_back(value);
        }
    }

    return true;
}

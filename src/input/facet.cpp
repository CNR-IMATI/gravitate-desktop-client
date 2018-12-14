/****************************************************************************
* GRAVITATE Desktop Client                                                  *
*                                                                           *
* Consiglio Nazionale delle Ricerche                                        *
* Istituto di Matematica Applicata e Tecnologie Informatiche                *
* Sezione di Genova                                                         *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Authors: Andrea Repetto and Andreas Scalas                                *
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
#include "facet.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

FacetReader::FacetReader()
{
    m_numFacets=0;
    m_facetTypeMap =
        {
            { "external", Facet::FacetType::EXTERNAL },
            { "internal", Facet::FacetType::INTERNAL},
            { "fracture", Facet::FacetType::FRACTURE },
        };
}

bool FacetReader::readFct(const std::string &filename)
{
    std::fstream fs;
    fs.open(filename, std::fstream::in);
    if(!fs.is_open())
    {
        return false;
    }

    try
    {
        readHeader(fs);
        for(int i = 0; i < m_numFacets; i++)
        {
           readFacet(fs);
        }
    }
    catch(fct_parse_error &e)
    {
        return false;
    }

    return true;
}

std::vector<Facet> FacetReader::getFacets()
{
    return m_facets;
}

void FacetReader::readHeader(std::fstream &fs)
{
    m_filename = readField(fs, "Filename: (.*)");

    std::string nFacetsStr = readField(fs, "Number of Facets: (.*)");
    m_numFacets = std::stoi(nFacetsStr);

    readExact(fs, ""); // Read empty line
}

void FacetReader::readFacet(std::fstream &fs)
{
    Facet facet;

    facet.label = readField(fs, "Facet ID: (.*)");

    std::string typeString;
    typeString = readField(fs, "Type: (.*)");
    facet.type = strToFacetType(typeString);

    readExact(fs, "Border:");
    std::vector<int> boundary;

    do{
        boundary = readVector<int>(fs);
        if(boundary.size() > 0)
            facet.boundaries.push_back(boundary);
    }while(boundary.size() > 0);

    facet.innerVertices = readVector<int>(fs);
    readExact(fs, "");

    m_facets.push_back(facet);
}

std::string trim(const std::string &str)
{
    if(str.empty())
        return str;

    std::size_t firstScan = str.find_first_not_of(' ');
    std::size_t first     = firstScan == std::string::npos ? str.length() : firstScan;
    std::size_t last      = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);

}

std::string FacetReader::readField(std::istream &fs, const std::string &regex, int matchIndex)
{
    try
    {
        std::regex re(regex);
        std::smatch match;

        std::string line;

        std::getline(fs, line);

        line = trim(line);

        if( !std::regex_search(line, match, re) || match.size() <= matchIndex )
        {
            std::stringstream err_str;
            err_str << "Could not parse field " << regex << "." << std::endl
                    << "Found " << line << " instead";

            throw fct_parse_error(err_str.str());
        }

        return match.str(matchIndex);
    }
    catch (std::regex_error &e)
    {
        throw fct_parse_error("Could not parse field " + regex + ": " + e.what());
    }
}

void FacetReader::readExact(std::istream &fs, const std::string &match_str)
{
    readField(fs, match_str, 0);
}

Facet::FacetType FacetReader::strToFacetType(std::string &value)
{
    try
    {
        return m_facetTypeMap.at(value);
    }
    catch(std::out_of_range &e)
    {
        throw fct_parse_error("Invalid facet type string: " + value);
    }
}

std::string FacetReader::getFilename() const
{
    return m_filename;
}

FacetWriter::FacetWriter(){
    m_numFacets = 0;
    m_facetTypeMap =
       {
           { Facet::FacetType::EXTERNAL, "external" },
           { Facet::FacetType::INTERNAL, "internal" },
           { Facet::FacetType::FRACTURE, "fracture"  },
       };
}

bool FacetWriter::writeFct(const std::string &filename)
{
    std::cout << "Writing file " << filename << std::endl;
    std::fstream fs;
    fs.open(filename, std::fstream::out);
    if(!fs.is_open())
        return false;

    try
    {
        writeHeader(fs);
        for(auto facet : m_facets)
        {
            writeFacet(facet, fs);
        }
    }
    catch(fct_write_error &e)
    {
        return false;
    }
    fs.close();

    return true;
}

void FacetWriter::writeHeader(std::fstream &fs)
{
    fs << "Filename: " + m_filename;
    fs << std::endl;
    fs << "Number of Facets: " + std::to_string(m_numFacets);
    fs << std::endl << std::endl;
}


void FacetWriter::writeFacet(const Facet &facet, std::fstream &fs)
{
    fs << "Facet ID: " + facet.label;
    fs << std::endl;
    fs << "Type: " + facetTypeToString(facet.type);
    fs << std::endl;
    fs << "Border:";
    fs << std::endl;

    for(auto boundary : facet.boundaries)
    {
        writeVector<int>(fs, boundary);
        fs << std::endl;
    }

    fs << "Vertices:";
    fs << std::endl;
    writeVector<int>(fs, facet.innerVertices);
    fs << std::endl;
    fs << std::endl;
}

void FacetWriter::writeField(std::ostream &fs, const std::string field)
{
    fs << field;
}

std::string FacetWriter::facetTypeToString(const Facet::FacetType &value)
{
    try
    {
        return m_facetTypeMap.at(value);
    }
    catch(std::out_of_range &e)
    {
        throw fct_parse_error("Invalid facet type");
    }
}

void FacetWriter::setFilename(const std::string &filename)
{
    m_filename = filename;
}

void FacetWriter::setFacets(std::vector<Facet> facets){
    m_facets.insert(m_facets.end(), facets.begin(), facets.end());
    m_numFacets = m_facets.size();
}

void load(const std::string file, std::vector<Facet> &facet)
{
    FacetReader reader;

    if(!reader.readFct(file))
    {
        throw std::runtime_error("Error parsing FCT file");
    }

    facet = reader.getFacets();
}

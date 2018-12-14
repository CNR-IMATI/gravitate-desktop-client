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
#include "webannotation.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

const char *WA_DEFAULT_CONTEXT = "http://www.w3.org/ns/anno.jsonld";
const char *THREED_DEFAULT_CONTEXT = "http://gravitate-project.eu/scheme/threedanno.jsonld";
const char *SPECIFIC_RESOURCE = "SpecificResource";

void to_json(json &j, const WebAnnotation::Body &b);

void from_json(const json &j, WebAnnotation::Body &b);

void to_json(json &j, const WebAnnotation::Selector &s);

void from_json(const json &j, WebAnnotation::Selector &s);

void to_json(json &j, const WebAnnotation::Source &s);

void from_json(const json &j, WebAnnotation::Source &s);

void to_json(json &j, const WebAnnotation::Target &t);

void from_json(const json &j, WebAnnotation::Target &t);

void to_json(json &j, const WebAnnotation &anno);

void from_json(const json &j, WebAnnotation &anno);

}

WebAnnotationReader::WebAnnotationReader(const std::string &fileName)
    : fileName(fileName)
{
}

WebAnnotation WebAnnotationReader::readJson()
{
    std::ifstream is(fileName);

    json j;

    is >> j;

    WebAnnotation anno = j;

    return anno;
}

void to_json(json &j, const WebAnnotation::Body &b)
{
    j = json{
        {"id", b.id},
        {"type", b.type}
    };
}

void from_json(const json &j, WebAnnotation::Body &b)
{
    b.id =   j.at("id").get<std::string>();
    b.type = j.at("type").get<std::string>();
}

void to_json(json &j, const WebAnnotation::Selector &s)
{
    j = json{
    {"id", s.id},
    {"type", s.type}
};
}

void from_json(const json &j, WebAnnotation::Selector &s)
{
    s.id =   j.at("id").get<std::string>();
    s.type = j.at("type").get<std::string>();
}

void to_json(json &j, const WebAnnotation::Source &s)
{
    j = json{
        {"id", s.id}
    };
}

void from_json(const json &j, WebAnnotation::Source &s)
{
    s.id = j.at("id").get<std::string>();
}

void to_json(json &j, const WebAnnotation::Target &t)
{
    j = json{
        {"@context", t.context},
        {"type", t.type},
        {"selector", t.selector},
        {"source", t.source}
    };
}

void from_json(const json &j, WebAnnotation::Target &t)
{
    try {
        t.context = j.at("@context").get<std::string>();
    } catch (json::out_of_range) {
        t.context = THREED_DEFAULT_CONTEXT;
    }

    try {
        t.type = j.at("type").get<std::string>();
    } catch (json::out_of_range) {
        t.type = SPECIFIC_RESOURCE;
    }

    t.selector = j.at("selector").get<WebAnnotation::Selector>();
    t.source = j.at("source").get<WebAnnotation::Source>();
}


void to_json(json &j, const WebAnnotation &anno)
{
    j = json{
        {"@context", anno.context},
        {"created",  anno.created},
        {"body",     anno.body},
        {"target",   anno.target}
    };
}

void from_json(const json &j, WebAnnotation &anno)
{
    try {
        anno.context = j.at("@context").get<std::string>();
    } catch (json::out_of_range) {
        anno.context = WA_DEFAULT_CONTEXT;
    }

    anno.body = j.at("body").get<WebAnnotation::Body>();
    anno.target = j.at("target").get<WebAnnotation::Target>();
}

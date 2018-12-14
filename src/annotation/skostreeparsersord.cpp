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
#include "skostreeparsersord.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <vector>

namespace {

struct triple
{
    QVariant s;
    QVariant o;
};

}

SkosTreeParserSord::SkosTreeParserSord()
{
    m_world = sord_world_new();
    m_model = nullptr;

    initUris();
}

void SkosTreeParserSord::initUris()
{
    uri.skos_broader = fromUri("http://www.w3.org/2004/02/skos/core#broader");
    uri.skos_narrower = fromUri("http://www.w3.org/2004/02/skos/core#narrower");
    uri.skos_prefLabel = fromUri("http://www.w3.org/2004/02/skos/core#prefLabel");
    uri.skos_scopeNote = fromUri("http://www.w3.org/2004/02/skos/core#scopeNote");
}

void SkosTreeParserSord::fill_narrower_property()
{
    auto iter = sord_search(m_model, nullptr, uri.skos_broader, nullptr, nullptr);

    std::vector<triple> triples;

    while(!sord_iter_end(iter))
    {
        auto s = sord_iter_get_node(iter, SORD_SUBJECT);
        auto o = sord_iter_get_node(iter, SORD_OBJECT);

        triples.push_back({value(o), value(s)});

        sord_iter_next(iter);
    }

    sord_iter_free(iter);

    for( auto t : triples )
    {
        auto s = fromUri(t.s.toUrl());
        auto o = fromUri(t.o.toUrl());

        SordQuad q{ s, uri.skos_narrower, o, nullptr };

        // Do not insert the triple, if already present
        if(sord_contains(m_model, q))
            continue;

        bool ok = sord_add(m_model, q);
        if(!ok)
        {
            qCritical() << "Error inserting "
                        << t.s.toUrl()
                        << "skos:narrower"
                        << t.o.toUrl();
            //break;
        }

        sord_node_free(m_world, s);
        sord_node_free(m_world, o);
    }
}

SkosTreeParserSord::~SkosTreeParserSord()
{
    sord_node_free(m_world, (SordNode*)uri.skos_broader);
    sord_node_free(m_world, (SordNode*)uri.skos_narrower);
    sord_node_free(m_world, (SordNode*)uri.skos_prefLabel);
    sord_node_free(m_world, (SordNode*)uri.skos_scopeNote);

    sord_free(m_model);
    sord_world_free(m_world);
}

QByteArray SkosTreeParserSord::parseToJson(const QString &filename, const QUrl &rootUrl)
{
    readModel(filename);

    auto root_node_ba = rootUrl.toString().toUtf8();

    auto root_uri_node = sord_new_uri(
                m_world,
                reinterpret_cast<const uint8_t *>(root_node_ba.data()));

    fill_narrower_property();

    auto tree = getTree(root_uri_node);

    auto doc = QJsonDocument::fromVariant(tree);

    sord_node_free(m_world, root_uri_node);

    return doc.toJson();
}

SordIter *SkosTreeParserSord::search_skos_broader(const SordNode *s)
{
    return sord_search(m_model, s, uri.skos_narrower, nullptr, nullptr);
}

SordIter * SkosTreeParserSord::search_skos_narrower(const SordNode *s)
{
    return sord_search(m_model, s, uri.skos_narrower, nullptr, nullptr);
}

QVariant SkosTreeParserSord::value(const SordNode *node)
{
    if(node == nullptr)
        return QVariant();

    auto node_str = (const char *) sord_node_get_string(node);
    return QVariant(node_str);
}

bool SkosTreeParserSord::readModel(const QString &filename)
{
    QFile f(filename);

    qDebug() << "Reading:" << f.fileName();

    if(!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open file" << f.fileName();
        return false;
    }

    if(m_model != nullptr)
        sord_free(m_model);

    m_model = sord_new(m_world, SORD_SPO|SORD_OPS|SORD_POS, false);

    auto input = f.readAll();

    auto base_uri_str = "http://www.example.org/skosparser#";

    SerdURI base_uri = SERD_URI_NULL;
    auto base = serd_node_new_uri_from_string(
                reinterpret_cast<const uint8_t*>(base_uri_str),
                NULL, &base_uri);


    auto input_syntax = SERD_TURTLE;

    auto env = serd_env_new(&base);

    auto reader = sord_new_reader(m_model, env, input_syntax, NULL);

    serd_reader_read_string(
                reader,
                reinterpret_cast<const uint8_t*>(input.data()));

    serd_reader_free(reader);

    return true;
}

QVariantMap SkosTreeParserSord::getTree(const SordNode *node)
{
    QVariantMap map;

    QVariantList children;

    auto iter = search_skos_narrower(node);

    while(!sord_iter_end(iter))
    {
        auto o = sord_iter_get_node(iter, SORD_OBJECT);

        children.append(getTree(o));

        sord_iter_next(iter);
    }

    auto prefLabel_object_node = sord_get(m_model, node, uri.skos_prefLabel, nullptr, nullptr );

    auto scopeNote_object_node = sord_get(m_model, node, uri.skos_scopeNote, nullptr, nullptr );

    map["uri"] = value(node).toUrl();

    auto label = value(prefLabel_object_node).toString();
    if(!label.isEmpty())
        map["label"] = label;

    auto description = value(scopeNote_object_node).toString();
    if(!description.isEmpty())
        map["description"] = description;

    if(children.size() > 0)
        map["children"] = children;

    sord_node_free(m_world, prefLabel_object_node);
    sord_node_free(m_world, scopeNote_object_node);
    sord_iter_free(iter);

    return map;
}

SordNode *SkosTreeParserSord::fromUri(const QString &uriString)
{
    return fromUri(QUrl(uriString));
}

SordNode *SkosTreeParserSord::fromUri(const QUrl &url)
{
    auto utf8 = url.toString().toUtf8();
    auto uri_node = sord_new_uri(
        m_world,
        reinterpret_cast<const uint8_t*>(utf8.data())
    );

    return uri_node;
}


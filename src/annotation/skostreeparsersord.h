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
#ifndef SKOSTREEPARSERSORD_H
#define SKOSTREEPARSERSORD_H

#include <QString>
#include <QUrl>
#include <QVariant>
#include <QVariantMap>
#include <QByteArray>
#include "serd/serd.h"
#include "sord/sord.h"

class SkosTreeParserSord
{
    struct URI
    {
        const SordNode *skos_broader;
        const SordNode *skos_narrower;
        const SordNode *skos_prefLabel;
        const SordNode *skos_scopeNote;
    };

public:
    SkosTreeParserSord();
    ~SkosTreeParserSord();

    QByteArray parseToJson(const QString &filename, const QUrl &rootUrl);

private:
    void initUris();

    void fill_narrower_property();

    SordIter * search_skos_broader(const SordNode *s = nullptr);

    SordIter * search_skos_narrower(const SordNode *s = nullptr);

    QVariant value(const SordNode *node);

    bool readModel(const QString &filename);

    QVariantMap getTree(const SordNode *node);

    SordNode *fromUri(const QString &uriString);
    SordNode *fromUri(const QUrl &url);

    SordWorld *m_world;
    SordModel *m_model;

    URI uri;
};




#endif // SKOSTREEPARSERSORD_H

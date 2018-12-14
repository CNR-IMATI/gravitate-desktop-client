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
#include "sparqlreplybuilder.h"
#include <QVariantMap>
#include <QJsonDocument>
#include <QUrl>
#include <QDebug>

SparqlReplyBuilder::SparqlReplyBuilder()
{

}

SparqlReplyBuilder &SparqlReplyBuilder::addVar(const QString &name, SparqlReplyBuilder::VarType type)
{
    m_vars.append({name, type});
    return *this;
}

SparqlReplyBuilder &SparqlReplyBuilder::addRow(const QVariantList &values)
{
    m_rows.push_back(values);
    return *this;
}

SparqlReplyBuilder &SparqlReplyBuilder::addRows(const QList<QVariantList> &rows)
{
    for(auto row: rows)
    {
        m_rows.push_back(row);
    }
    return *this;
}

QByteArray SparqlReplyBuilder::getJson() const
{
    QVariantMap map
    {
        {"head", createHeader() },
        {"results", createBindings() }
    };

    auto doc = QJsonDocument::fromVariant(map);

    return doc.toJson();
}

QVariant SparqlReplyBuilder::createHeader() const
{
    QStringList varNames;
    for(auto v : m_vars)
    {
        varNames.append(v.first);
    }

    return QVariantMap {
      { "vars", varNames }
    };
}

QVariant SparqlReplyBuilder::createBindings() const
{
    QVariantList bindings;

    for(auto row : m_rows)
    {
        QVariantMap binding;

        for(int i = 0; i < m_vars.count(); i++)
        {
            auto rowList = qvariant_cast<QVariantList>(row);
            auto value = rowList[i];
            auto varName = m_vars[i].first;
            auto type = m_vars[i].second;

            binding[varName] = QVariantMap
            {
                {"type", getTypeString(type) },
                {"value",value.toString()}
            };
        }
        bindings.append(binding);
    }

    return QVariantMap{
        {"bindings", bindings}
    };
}

QString SparqlReplyBuilder::getTypeString(VarType type) const
{
    switch (type) {
    case URI: return "uri";
    case LITERAL: return "literal";
    default:
        return "literal";
    }
}

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
#include "sparqlresults.h"
#include <QDebug>

SparqlResults::SparqlResults()
{
    reset();
}

QVariantList SparqlResults::parse(const QByteArray &jsonSparql)
{
    SparqlResults results;

    results.readJson(jsonSparql);

    QVariantList list;

    while( results.next() )
    {
        QVariantMap row;
        for(auto var : results.m_vars)
        {
            row[var] = results.value(var);
        }
        list.append(row);
    }

    return list;
}

void SparqlResults::readJson(const QByteArray &json)
{
    if(json.isEmpty())
    {
//        qCritical() << "JSON response is empty";
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);

    if(error.error != QJsonParseError::NoError )
    {
        qCritical() << "JSON parsing error:" << error.errorString();
        qCritical() << "Response content: " << json;
        return;
    }

    QJsonObject object = doc.object();

    if(!parseHeader(object["head"].toObject()))
    {
        qDebug() << "Error reading SPARQL results header";
    }

    if(!parseResults(object["results"].toObject()))
    {
        qDebug() << "Error reading SPARQL results content";
    }
}

void SparqlResults::reset()
{
    m_currentIndex = -1;
}

int SparqlResults::count() const
{
    return m_resultArray.count();
}

bool SparqlResults::next()
{
    if( m_currentIndex >= count() - 1)
    {
        return false;
    }

    /*
     *  TODO: implement an iterator interface, so that the
     *  SparqlResults class can be used as a const object
     */
    m_currentIndex++;
    m_currentValue = parseRow(m_resultArray[m_currentIndex]);

    return true;
}

QVariant SparqlResults::value(const QString& index, const QVariant &defaultValue) const
{
    return m_currentValue.value(index, defaultValue);
}

bool SparqlResults::parseHeader(const QJsonObject &header)
{
    if(! (header.contains("vars") && header["vars"].isArray()) )
    {
        return false;
    }

    foreach(const QJsonValue &var, header["vars"].toArray())
    {
        if(var.isString())
        {
            m_vars.append(var.toString());
        }
    }

    return true;
}

bool SparqlResults::parseResults(const QJsonObject &results)
{
    if(! (results.contains("bindings") &&
          results["bindings"].isArray() ) )
    {
        return false;
    }
    m_resultArray = results["bindings"].toArray();

    return true;
}

QVariant SparqlResults::parseValue(const QJsonValue &valueItem)
{
    if(valueItem.isObject())
    {
        QJsonObject obj =valueItem.toObject();
//        qDebug() << obj["value"].toString();
        return obj["value"].toString();
    }
    return QVariant();
}

QVariantMap SparqlResults::parseRow(const QJsonValue &row)
{
    QVariantMap map;
    QJsonObject obj = row.toObject();
    foreach(const QString &var, m_vars)
    {
        map[var] = parseValue(obj[var]);
//        qDebug() << var << map[var];
    }
    return map;
}

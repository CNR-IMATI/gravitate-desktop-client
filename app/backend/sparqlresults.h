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
#ifndef SPARQLRESULTS_H
#define SPARQLRESULTS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QVariantMap>
#include <QMetaType>

class SparqlResults
{
public:
    SparqlResults();

    static QVariantList parse(const QByteArray &jsonSparql);

private:
    void readJson(const QByteArray &json);

    void reset();

    int count() const;
    bool next();
    QVariant value(const QString &index, const QVariant &defaultValue = QVariant()) const;

    /**
     * @brief parseHeader
     * @param header The header object
     * @return true if the header has been read correctly, false otherwise
     */
    bool parseHeader(const QJsonObject &header);
    bool parseResults(const QJsonObject &results);
    QVariant parseValue(const QJsonValue &valueItem);
    QVariantMap parseRow(const QJsonValue &row);

    QStringList m_vars;
    QJsonArray m_resultArray;
    QVariantMap m_currentValue;
    int m_currentIndex;
};

Q_DECLARE_METATYPE(SparqlResults)

#endif // SPARQLRESULTS_H

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
#include "stringresources.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <iostream>

StringResources::StringResources()
{

}

bool StringResources::readXml(const QString &fileName)
{
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
        return false;

    auto ba = file.readAll();

    return parseXmlData(ba);
}

QString StringResources::value(const QString &key)
{
    return m_stringMap.value(key, QString());
}

int StringResources::count()
{
    return m_stringMap.count();
}

bool StringResources::parseXmlData(const QByteArray &data)
{
    QXmlStreamReader xml;

    xml.addData(data);

    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.tokenType() == QXmlStreamReader::StartElement &&
            xml.name() == "string")
        {
            // Get key
            auto attr = xml.attributes();
            auto key = attr.value("name").toString();

            // Get content
            auto value = parseHtml(xml);

            m_stringMap.insert(key, value);
        }
    }

    if(xml.hasError())
    {
        qCritical() << "XML parsing error:" << xml.errorString();
        qCritical() << "Line:" << xml.lineNumber();

        return false;
    }

    return true;
}

QString StringResources::parseHtml(QXmlStreamReader &xml)
{
    // Code from: https://stackoverflow.com/a/30435931/3283537
    if(xml.namespaceUri().toString() != "http://www.w3.org/1999/xhtml")
    {
        return xml.readElementText(QXmlStreamReader::IncludeChildElements);
    }
    QString terminatingElement = xml.name().toString();
    QString html;
    QXmlStreamWriter writer(&html);
    do
    {
          xml.readNext();
          switch( xml.tokenType() )
          {
          case QXmlStreamReader::StartElement:
              writer.writeStartElement(xml.name().toString());
              writer.writeAttributes(xml.attributes());
              break;
          case QXmlStreamReader::EndElement:
              writer.writeEndElement();
              break;
          case QXmlStreamReader::Characters:
              writer.writeCharacters(xml.text().toString());
              break;
          default:
              break;
          }
    }
    while (!xml.atEnd() && xml.name() != terminatingElement );
    return html;
}

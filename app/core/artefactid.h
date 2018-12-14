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
#ifndef ARTEFACTID_H
#define ARTEFACTID_H

#include <QUrl>
#include <QString>
#include <QMetaType>

class ArtefactId
{
    friend bool operator<(const ArtefactId &a, const ArtefactId &b);

public:

    static ArtefactId null()
    {
        static ArtefactId id;
        return id;
    }

    static ArtefactId fromUri(const QUrl &uri)
    {
        ArtefactId id;
        id.uri = uri;
        return id;
    }

    QUrl uri;

    QString toString() const
    {
        return uri.toString();
    }

    bool isNull() const
    {
        return uri.isEmpty();
    }

    bool operator==(const ArtefactId &other) const
    {
        return this->uri == other.uri;
    }


    bool operator<(const ArtefactId &other)
    {
        return uri < other.uri;
    }

    ArtefactId operator=(const QUrl &uri)
    {
        return ArtefactId::fromUri(uri);
    }
//protected:
//    ArtefactId() {}
};

inline bool operator<(const ArtefactId &a, const ArtefactId &b)
{
    return a.uri < b.uri;
}

Q_DECLARE_METATYPE(ArtefactId)

#endif // ARTEFACTID_H

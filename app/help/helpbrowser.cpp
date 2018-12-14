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
#include "helpbrowser.h"
#include <QDebug>
#include <QFileInfo>

HelpBrowser::HelpBrowser(QWidget *parent)
    : QTextBrowser(parent)
{
}


QVariant HelpBrowser::loadResource(int type, const QUrl &name)
{
    QByteArray ba;
    if (type < 4 && m_helpEngine) {
        QUrl url(name);
        if (name.isRelative())
            url = source().resolved(url);
        ba = m_helpEngine->fileData(url);
    }
    return ba;
}

void HelpBrowser::setCollectionFile(const QString &collectionFile)
{
    if(collectionFile.isEmpty())
    {
        qCritical() << "Collection file name is empty";
        return;
    }

    if(!QFileInfo::exists(collectionFile))
    {
        qCritical() << "No help collection file found at" << collectionFile;
        return;
    }

    m_helpEngine = new QHelpEngineCore(collectionFile, this);

    if (!m_helpEngine->setupData()) {
        qCritical() << "Could not find help collection file in:" << collectionFile;
        delete m_helpEngine;
        m_helpEngine = nullptr;
        qCritical() << "Could not load the help collection file:" << collectionFile;
    }
}

bool HelpBrowser::setHelpForKeyword(const QString &keyword)
{
    if(!isValid())
    {
        qCritical() << "Help engine is in invalid state";
        return false;
    }

    auto links = m_helpEngine->linksForKeyword(keyword);

    if(links.count() == 0)
    {
        qCritical() << "Help page for keyword" << keyword << "not found";
        return false;
    }

    // We have at least one link related to the keyword, we take the first
    auto url = links.constBegin().value();
    setSource(url);
    return true;
}

bool HelpBrowser::isValid()
{
    return m_helpEngine != nullptr;
}

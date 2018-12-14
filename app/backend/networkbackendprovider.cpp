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
#include "networkbackendprovider.h"
#include <exception>
#include "gravitatebackendconf.h"

NetworkBackendProvider::NetworkBackendProvider(QNetworkAccessManager *manager, QObject *parent)
    : BackendProvider(parent), manager_(manager)
{
}

void NetworkBackendProvider::setAuthenticator(Authenticator * const auth)
{
    m_auth = auth;
    m_auth->setParent(this);
}

Authenticator *NetworkBackendProvider::authenticator() const
{
    return m_auth;
}

bool NetworkBackendProvider::hasAuthenticator() const
{
    return m_auth != nullptr;
}

const QString &NetworkBackendProvider::hostname() const
{
    return hostname_;
}

void NetworkBackendProvider::setHostname(QString hostname)
{
    if(hostname.isEmpty() || hostname.isNull())
    {
        throw new std::invalid_argument("hostname is empty or null");
    }

    hostname_ = hostname;
}

QNetworkRequest NetworkBackendProvider::createHttpRequest(const QString &route, const QUrlQuery &query) const
{
    QNetworkRequest req;

    if(hasAuthenticator())
    {
        req = authenticator()->addAuthenticationHeader(req);
    }

    QUrl url = endpoint(route);

    if(!query.isEmpty())
    {
        url.setQuery(query);
    }

    req.setUrl(url);

    return req;
}



QNetworkRequest NetworkBackendProvider::createSparqlRequest() const
{
    QNetworkRequest req = createHttpRequest(GravitateBackendConf::SparqlEndpoint);

    // the content of the request is a SPARQL query
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/sparql-query");

    // The expected return type is "SPARQL results in JSON syntax"
    req.setRawHeader("Accept", "application/sparql-results+json");

    return req;
}

QNetworkRequest NetworkBackendProvider::createContainerListingRequest()
{
    QNetworkRequest req = createHttpRequest(GravitateBackendConf::ContainersEndpoint);

    // The expected return type is a RDF graph in Turtle syntax
    req.setRawHeader("Accept", "text/turtle");

    return req;
}

QNetworkRequest NetworkBackendProvider::createContainerUpdateRequest()
{
    QNetworkRequest req = createHttpRequest(GravitateBackendConf::ContainersEndpoint);

    // The content type is a RDF graph in Turtle syntax
    req.setHeader(QNetworkRequest::ContentTypeHeader, "text/turtle");

    // For the Slug header see https://blog.cdivilly.com/2016/03/01/slug-http-header/
    req.setRawHeader("Slug", "http://it-innovation.com/ontology/citationContainer");

    return req;
}

QUrl NetworkBackendProvider::endpoint(const QString &route) const
{
    QUrl url(hostname() + route);
    return url;
}

QNetworkReply *NetworkBackendProvider::apiGet(const QString &route, const QUrlQuery &query)
{
    QNetworkRequest req = createHttpRequest(route, query);
//    qDebug().noquote() << "==> GET" << req.url().toString();

    return manager_->get(req);
}

QNetworkReply *NetworkBackendProvider::apiPost(const QString &route, const QUrlQuery &query, const QByteArray &data, const QString &contentTypeHeader)
{
    QNetworkRequest req = createHttpRequest(route, query);

    req.setHeader(QNetworkRequest::ContentTypeHeader, contentTypeHeader);

//    qDebug().noquote() << "==> POST" << req.url().toString();
//    qDebug().noquote() << data;

    return manager_->post(req, data );
}

QNetworkReply *NetworkBackendProvider::apiDelete(const QString &route, const QUrlQuery &query)
{
    QNetworkRequest req = createHttpRequest(route, query);

    return manager_->deleteResource(req);
}

QNetworkReply *NetworkBackendProvider::sparqlQuery(const QString &queryString)
{
    QNetworkRequest req = createSparqlRequest();

    return manager_->post(req, queryString.toUtf8());
}


QNetworkReply *NetworkBackendProvider::apiPostJson(const QJsonDocument &doc, const QString &route, const QUrlQuery &query)
{
    return apiPost(route, query, doc.toJson(QJsonDocument::Compact), "application/json");
}

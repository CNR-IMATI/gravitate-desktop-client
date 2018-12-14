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
#include "mainwindow.h"
#include <QSurfaceFormat>
#include <QVTKOpenGLWidget.h>
#include <QApplication>
#include <QSettings>
#include <QCommandLineParser>
#include "logindialog.h"
#include "localfilecache.h"
#include "debugmessagehandling.h"
#include "backendfactorybase.h"
#include <QOpenGLWidget>
#include "core/graphicssettings.h"
#include <QFile>

void initSettings();
void handleLogin();

int main(int argc, char *argv[])
{
    initializeQtOpenGLSettings();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Use always the INI format for the settings
    QSettings::setDefaultFormat(QSettings::IniFormat);

    // This sets the default formatting for qDebug messages
    qSetMessagePattern(customMessagePattern);

    QCoreApplication::setOrganizationName("IMATI");
    QCoreApplication::setOrganizationDomain("ge.imati.cnr.it");
    QCoreApplication::setApplicationName("gravitate-client");


    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption cacheDirOption("cachedir",
            QCoreApplication::translate("main", "Set the cache directory to <cachedir>"),
            QCoreApplication::translate("main", "cachedir"));
    cacheDirOption.setDefaultValue("cache");

    parser.addOption(cacheDirOption);

    parser.process(a);

    QString cacheDir = parser.value(cacheDirOption);

    LocalFileCache::instance()->setCacheLocation(cacheDir);

    initSettings();

    handleLogin();

    MainWindow w;
    w.show();

    return a.exec();
}

void initSettings()
{
    QSettings settings;

    qDebug() << "Settings file location:" << settings.fileName();

    if(QFile::exists(settings.fileName()))
    {
        return;
    }

    settings.setValue("hostname", "https://gravitate-1.cytera.cyi.ac.cy");
    settings.setValue("user", "");
    settings.setValue("password", "");
    settings.setValue("scheduler/user", "");
    settings.setValue("scheduler/password", "");
}

void handleLogin()
{
    QSettings settings;

    auto hostname = settings.value("hostname", "https://gravitate-1.cytera.cyi.ac.cy").toString();
    auto user = settings.value("user", "").toString();
    auto password = settings.value("password", "").toString();

    LoginDialog dialog;
    dialog.setHostname(hostname);
    dialog.setUser(user);
    dialog.setPassword(password);

    int code = dialog.exec();
    if(code == QDialog::Rejected)
    {
        exit(0);
    }

    hostname = dialog.hostname();
    user = dialog.user();
    password = dialog.password();

    BackendFactoryBase::instance()->setHostname(hostname);

    auto auth = new BasicHttpAuth(user, password);
    BackendFactoryBase::instance()->setAuthenticator(auth);

    settings.setValue("hostname", hostname);
    settings.setValue("user", user);
    settings.setValue("password", password);
}

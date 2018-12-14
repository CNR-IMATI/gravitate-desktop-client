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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "abstractdashboard.h"
#include "clipboardmodel.h"
#include "helpdialog.h"
#include "helphandler.h"
#include "thumbnailprovider.h"
#include "clipgroupsprovider.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public AbstractDashboard
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void changeCurrentToolWidget(QWidget* newtoolWidget);

    ClipboardModel *clipboardModel() const override;

    HelpHandler *helpHandler() const override;

    AbstractThumbnailProvider *thumbnailProvider() const override;

    AbstractDownloader *downloader() const override;

    GravitateJobManager *jobManager() const override;

private slots:

    void onViewChanged(int index);

signals:
    void userChanged(const QString &user);

private:
    QMenu *dataMenu;
    Ui::MainWindow *ui;

    QMetaObject::Connection m_clipboardMenuConnection;

    void requestCredentials();

    void setupViews();

    ClipboardModel *m_clipboardModel;

    QString currentViewName();

    HelpDialog * m_helpDialog;
    HelpHandler *m_helpHandler;
    AbstractThumbnailProvider *m_thumbnailProvider;

    AbstractDownloader *m_downloader;
    GravitateJobManager *m_jobManager;

    void initHelp();

    // QObject interface
public:
    bool event(QEvent *event) override;
};

#endif // MAINWINDOW_H

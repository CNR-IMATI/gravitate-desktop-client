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
#include "ui_mainwindow.h"

#include <QTimer>
#include <QAction>
#include <QAbstractButton>
#include <QDebug>
#include <QSettings>
#include <QWhatsThis>
#include <QKeyEvent>
#include <QDesktopServices>

#include "gravitatebackend.h"
#include "clipboardpanel.h"
#include "inspectionviewmain.h"
#include "fragmentviewmain.h"
#include "reassemblyviewmain.h"
#include "historyviewmain.h"
#include "groupsviewmain.h"
#include "gravitateclipgroupsprovider.h"
#include "backendfactorybase.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_clipboardModel(nullptr)
{
    this->showMaximized();
    ui->setupUi(this);

    auto factory = BackendFactoryBase::instance();

    auto selectorWidget = ui->viewContainer->selectorWidget();
    ui->selectionBarLayout->addWidget(selectorWidget);

    m_downloader = GravitateBackend::defaultDownloader();

    m_jobManager = GravitateBackend::jobManager();

    // ThumbnailProvider initialization
    // NOTE: releasing std::unique_ptr, mainwindow takes care of this
    m_thumbnailProvider = factory->createThumbnailProvider().release();
    m_thumbnailProvider->setParent(this);

    auto clipGroups = BackendFactoryBase::instance()->createClipGroupsProvider();

    // NOTE: ClipboardModel takes care of ClipGroupsProvider lifetime
    //       so we can release the std::unique pointer
    m_clipboardModel = new ClipboardModel(clipGroups.release(), this);
    m_clipboardModel->setThumbnailProvider(m_thumbnailProvider);

    ui->clipboardPanel->setClipboardModel(m_clipboardModel);

    connect (ui->refreshButton, &QPushButton::clicked,
             m_clipboardModel, &ClipboardModel::loadClipboard);

    initHelp();

    setupViews();

    // Other signals initialization
    connect (this, &MainWindow::userChanged,
             ui->userLabel, &QLabel::setText);

    // AR: This is to make the upper bar smaller
    ui->upperBarDockWidget->setTitleBarWidget(new QWidget());

    ui->centralWidget->layout()->setMargin(0);
    ui->viewContainer->layout()->setMargin(0);

    QTimer::singleShot(0, this, &MainWindow::requestCredentials);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestCredentials()
{
    QSettings settings;

    auto hostName = settings.value("hostname").toString();
    auto user = settings.value("user").toString();

    qInfo() << "Hostname:" << hostName;
    qInfo() << "User:" << user;

    emit userChanged(user);

    m_clipboardModel->loadClipboard();
}

void MainWindow::setupViews()
{
    QStringList names = {
        tr("Inspection"),
        tr("Fragment"),
        tr("ReAssembly"),
        tr("History"),
        tr("Groups")
    };

    AbstractView *views[] = {
        new InspectionViewMain(this, this),
        new FragmentViewMain(this, this),
        new ReassemblyViewMain(this, this),
        new HistoryViewMain(this, this),
        new GroupsViewMain(this, this)
    };

    connect(ui->viewContainer, &ViewContainer::currentChanged,
            this, &MainWindow::onViewChanged);

    for(int i = 0; i < 5; i++)
    {
        auto view = views[i];
        auto name = names[i];

        ui->viewContainer->addView(name, view);

        if(view == nullptr)
            continue;

        view->initTools();
    }

    ui->viewContainer->setCurrentView(0);
}

void MainWindow::changeCurrentToolWidget(QWidget* newtoolWidget)
{
    if(newtoolWidget == nullptr)
        return;

    ui->toolsDockWidget->setWidget(newtoolWidget);






}

ClipboardModel *MainWindow::clipboardModel() const
{
    return m_clipboardModel;
}

HelpHandler *MainWindow::helpHandler() const
{
    return m_helpHandler;
}

AbstractThumbnailProvider *MainWindow::thumbnailProvider() const
{
    return m_thumbnailProvider;
}

AbstractDownloader *MainWindow::downloader() const
{
    return m_downloader;
}

GravitateJobManager *MainWindow::jobManager() const
{
    return m_jobManager;
}

QString MainWindow::currentViewName()
{    
    auto currentView = ui->viewContainer->currentView();
    return ui->viewContainer->name(currentView);
}

void MainWindow::initHelp()
{
    m_helpDialog = new HelpDialog(this);

    m_helpHandler = new HelpHandler(this);

    connect(ui->actionGravitate_Website, &QAction::triggered,
            [] {
        QDesktopServices::openUrl(QUrl("http://gravitate-project.eu"));
    });

    connect(ui->actionUser_Manual, &QAction::triggered,
            [] {
        QDesktopServices::openUrl(QUrl("help://gravitate-manual/index"));
    });

    QDesktopServices::setUrlHandler("help", m_helpDialog, "showHelpForUrl");

    connect(ui->whatsThisButton, &QAbstractButton::clicked,
            [] {
        QWhatsThis::enterWhatsThisMode();
    });
}


void MainWindow::onViewChanged(int index)
{
    auto view = ui->viewContainer->currentView();
    ui->toolsDockWidget->setWidget(view->getToolWidget());

    disconnect(m_clipboardMenuConnection);

    //ui->clipboardPanel->disconnect(SIGNAL(contextMenuOpen(QMenu*, GResource)));
    auto currentView = ui->viewContainer->currentView();

    m_clipboardMenuConnection = connect(ui->clipboardPanel, &ClipboardPanel::contextMenuOpen,
                                        currentView, &AbstractView::onClipboardContextMenuOpen, Qt::UniqueConnection);
}

bool MainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        auto ke = static_cast<QKeyEvent*>(event);
        if(ke->key() == Qt::Key_F1)
        {
            auto viewName = currentViewName();

            m_helpDialog->showHelpForKeyword(viewName.toLower());
            return true;
        }
    }

    if(event->type() == QEvent::WhatsThisClicked)
    {
        auto wte = static_cast<QWhatsThisClickedEvent*>(event);
        QDesktopServices::openUrl(wte->href());
        return true;
    }

    return QMainWindow::event(event);
}

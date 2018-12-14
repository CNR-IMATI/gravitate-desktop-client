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
#include "viewcontainer.h"
#include <QVBoxLayout>

ViewContainer::ViewContainer(QWidget *parent)
    : QWidget (parent)
{
    auto layout = new QVBoxLayout(this);

    m_stackedWidget = new QStackedWidget(this);
    layout->addWidget(m_stackedWidget);

    m_selectionBar = new ViewSelectionBar();

    connect(m_selectionBar, &ViewSelectionBar::currentViewSelected,
            m_stackedWidget, &QStackedWidget::setCurrentIndex);

    setLayout(layout);

    connect(m_stackedWidget, &QStackedWidget::currentChanged,
            this, &ViewContainer::currentChanged);

    connect(this, &ViewContainer::currentChanged,
            [this] (int index) {
        m_selectionBar->blockSignals(true);
        auto button = m_selectionBar->getButtonAtIndex(index);
        button->setChecked(true);
        m_selectionBar->blockSignals(false);
    });
}

QWidget *ViewContainer::selectorWidget()
{
    return m_selectionBar;
}

AbstractView *ViewContainer::currentView()
{
    return static_cast<AbstractView*>(m_stackedWidget->currentWidget());
}

void ViewContainer::addView(const QString &name, AbstractView *view)
{
    auto button = m_selectionBar->addButton(name);

    if(view != nullptr)
    {
        m_stackedWidget->addWidget(view);
    }
    else
    {
        button->setEnabled(false);
        m_stackedWidget->addWidget(new QWidget());
    }
}

QString ViewContainer::name(AbstractView *view)
{
    int index = indexOf(view);
    auto button = m_selectionBar->getButtonAtIndex(index);
    return button->text();
}

int ViewContainer::indexOf(AbstractView *view)
{
    return m_stackedWidget->indexOf(view);
}

void ViewContainer::setCurrentView(int index)
{
    m_stackedWidget->setCurrentIndex(index);
}

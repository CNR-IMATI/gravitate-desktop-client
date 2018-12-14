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
#include "toolsetwidget.h"
#include <QDebug>
#include <QLayout>
#include <QToolButton>
#include <QVBoxLayout>

ToolsetWidget::ToolsetWidget(QWidget *parent) : QWidget(parent)
{
    // "outer" layout initialization
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Container initialization
    QWidget *container = new QWidget(this);

    layout->addWidget(container);
    layout->addStretch();

    m_buttonLayout = new QVBoxLayout(container);
    m_buttonLayout->setMargin(0);
}

ToolsetWidget &ToolsetWidget::addToolPlaceholder(const QString &text, bool enabled)
{
    QAction * action = new QAction(text);
    action->setEnabled(enabled);
    return this->addTool(action);
}

void ToolsetWidget::addWidget(QWidget* widget)
{
    m_buttonLayout->addWidget(widget);
}

ToolsetWidget &ToolsetWidget::addTool(QAction *action, const QString &text)
{
    //QAction *actionCopy = new QAction(*action);
    //actionCopy->setText(name);

    QString actionText;

    if(text.isEmpty() || text.isNull())
    {
        actionText = action->text();
    }
    else
    {
        actionText = text;
    }

    if(m_menuStack.size() > 0)
    {
        m_menuStack.top()->addAction(action);
    }
    else
    {
        auto button = createButton(actionText);
        button->setDefaultAction(action);
        m_currentButton = button;
        m_buttonMap.insert(action, button);
        addWidget(button);
    }
    return *this;
}

ToolsetWidget &ToolsetWidget::beginMenu(const QString &text)
{
    return beginMenu(new QMenu(text));
}

ToolsetWidget &ToolsetWidget::beginMenu(QMenu *menu)
{
    if(menu == nullptr)
    {
        qCritical() << Q_FUNC_INFO << "called with a NULL argument";
        return *this;
    }

    if(m_menuStack.size() == 0)
    {
        m_currentButton = createButton(menu->title());
        m_currentButton->setPopupMode(QToolButton::InstantPopup);
        m_currentButton->setMenu(menu);
        m_buttonMap.insert(menu, m_currentButton);
        addWidget(m_currentButton);
    }
    else
    {
        QMenu *currentMenu = m_menuStack.top();
        currentMenu->addMenu(menu);
    }

    m_menuStack.push(menu);
    return *this;
}

ToolsetWidget &ToolsetWidget::endMenu()
{
    m_menuStack.pop();
    return *this;
}

QAction *ToolsetWidget::findTool(const QString &text)
{
    auto children = m_buttonLayout->findChildren<QToolButton*>();
    for(auto c : children)
    {
        if(c->text().toLower() == text.toLower())
        {
            return c->defaultAction();
        }
    }
    return nullptr;
}

QToolButton *ToolsetWidget::getToolButton(QObject *object)
{
    return m_buttonMap.value(object, nullptr);
}

int ToolsetWidget::nestingLevel()
{
    return m_menuStack.size();
}

QString ToolsetWidget::debugIndent()
{
    return QString("   ").repeated(nestingLevel());
}

QToolButton *ToolsetWidget::createButton(const QString &text)
{
    QToolButton * button = new QToolButton();
    button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setText(text);
    return button;
}

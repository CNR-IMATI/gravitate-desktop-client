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
#ifndef TOOLSETWIDGET_H
#define TOOLSETWIDGET_H

#include <QWidget>
#include <QString>
#include <QAction>
#include <QStack>
#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>

class ToolsetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolsetWidget(QWidget *parent = 0);

    /**
     * @brief Creates a placeholder
     * @param text The tool button text
     * @param enabled Sets the initial state of the button (it cannot be changed)
     * @return The current ToolsetWidget instance
     */
    ToolsetWidget &addToolPlaceholder(const QString &text, bool enabled = false);


    /**
     * @brief Adds a tool button for the given QAction.
     * The tool text is given by the text of the QAction.
     *
     * @param action The action
     * @return The current ToolsetWidget instance
     */
    ToolsetWidget &addTool(QAction *action, const QString &text = "");

    /**
     * @brief beginMenu
     * @param text
     * @return The current ToolsetWidget instance
     */
    ToolsetWidget &beginMenu(const QString &text);

    ToolsetWidget &beginMenu(QMenu *menu);

    /**
     * @brief endMenu
     * @return The current ToolsetWidget instance
     */
    ToolsetWidget &endMenu();

    QAction *findTool(const QString &text);

    QToolButton *getToolButton(QObject *object);

signals:

public slots:

private:
    int nestingLevel();
    QString debugIndent();
    QToolButton *createButton(const QString &text);

    QVBoxLayout *m_buttonLayout;
    QToolButton *m_currentButton;
    QStack<QMenu*> m_menuStack;
    void addWidget(QWidget* widget);

    QMap<QObject*, QToolButton*> m_buttonMap;
};

#endif // TOOLSETWIDGET_H

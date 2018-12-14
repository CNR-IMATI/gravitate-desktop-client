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
#include "parameterselectiondialog.h"
#include <QSpinBox>
#include <QLabel>
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>

ParameterSelectionDialog::ParameterSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    m_layout = new QFormLayout();
    setLayout(m_layout);
}

void ParameterSelectionDialog::addIntParameter(const QString &paramName, const QString &label, int min, int max)
{
    auto spin = new QSpinBox(this);

    spin->setMinimum(min);
    spin->setMaximum(max);

    m_layout->addRow(label, spin);
    m_widgetMap.insert(paramName, {WidgetType::SPINBOX, spin});
}

void ParameterSelectionDialog::addDoubleParameter(const QString &paramName,  const QString &label, double min, double max, double step)
{
    auto spin = new QDoubleSpinBox(this);
    spin->setMinimum(min);
    spin->setMaximum(max);
    spin->setSingleStep(step);

    m_layout->addRow(label, spin);
    m_widgetMap.insert(paramName, {WidgetType::DOUBLESPINBOX, spin});
}

void ParameterSelectionDialog::addBoolParameter(const QString &paramName, const QString &label)
{
    auto check = new QCheckBox(this);
    m_layout->addRow(label, check);
    m_widgetMap.insert(paramName, {WidgetType::CHECKBOX, check});

}

void ParameterSelectionDialog::addListParameter(const QString &paramName, const QString &label, const QStringList &values)
{
    auto combo = new QComboBox(this);
    combo->insertItems(0, values);
    combo->setEditable(false);
    m_layout->addRow(label, combo);
    m_widgetMap.insert(paramName, {WidgetType::COMBOBOX, combo});
}

void ParameterSelectionDialog::addListParameterValue(const QString &paramName, const QString &text, const QVariant &userData)
{
    if(!m_widgetMap.contains(paramName))
    {
        qCritical() << "Parameter" << paramName << "not found";
        return;
    }

    auto widgetInfo = m_widgetMap.value(paramName);
    if(widgetInfo.type != WidgetType::COMBOBOX)
    {
        qCritical() << "Parameter" << paramName << "is not a combo box";
        return;
    }

    QComboBox* combo = static_cast<QComboBox*>(widgetInfo.widget);
    combo->addItem(text, userData);
}

void ParameterSelectionDialog::setValue(const QString &paramName, const QVariant &value)
{
    if(!m_widgetMap.contains(paramName))
    {
        return;
    }

    auto widgetInfo = m_widgetMap.value(paramName);

    switch(widgetInfo.type)
    {
    case WidgetType::SPINBOX:
    {
        auto spin = static_cast<QSpinBox*>(widgetInfo.widget);
        spin->setValue(value.toInt());
        break;
    }
    case WidgetType::DOUBLESPINBOX:
    {
        auto spin = static_cast<QDoubleSpinBox*>(widgetInfo.widget);
        spin->setValue(value.toDouble());
        break;
    }
    case WidgetType::CHECKBOX:
    {
        auto check = static_cast<QCheckBox*>(widgetInfo.widget);
        check->setChecked(value.toBool());
        break;
    }
    case WidgetType::COMBOBOX:
    {
        auto combo = static_cast<QComboBox*>(widgetInfo.widget);
        combo->setCurrentText(value.toString());
        break;
    }
    }
}

QVariant ParameterSelectionDialog::value(const QString &paramName)
{
    if(!m_widgetMap.contains(paramName))
        return QVariant();

    auto widgetInfo = m_widgetMap.value(paramName);

    switch (widgetInfo.type)
    {
    case WidgetType::SPINBOX:
    {
        auto spin = static_cast<QSpinBox*>(widgetInfo.widget);
        return spin->value();
    }
    case WidgetType::DOUBLESPINBOX:
    {
        auto spin = static_cast<QDoubleSpinBox*>(widgetInfo.widget);
        return spin->value();
        break;
    }
    case WidgetType::CHECKBOX:
    {
        auto check = static_cast<QCheckBox*>(widgetInfo.widget);
        return check->isChecked();
        break;
    }
    case WidgetType::COMBOBOX:
    {
        auto combo = static_cast<QComboBox*>(widgetInfo.widget);
        return combo->currentText();
    }
    }

    return QVariant();
}

QVariant ParameterSelectionDialog::data(const QString &paramName)
{
    if(!m_widgetMap.contains(paramName))
        return QVariant();

    auto widgetInfo = m_widgetMap.value(paramName);

    if(widgetInfo.type == WidgetType::COMBOBOX)
    {
        auto combo = static_cast<QComboBox*>(widgetInfo.widget);
        return combo->currentData();
    }
    return QVariant();
}

QWidget *ParameterSelectionDialog::getWidget(const QString &paramName)
{
    if(!m_widgetMap.contains(paramName))
    {
        return nullptr;
    }

    return m_widgetMap.value(paramName).widget;
}


void ParameterSelectionDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(m_buttonBox, &QDialogButtonBox::accepted,
            this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
    m_layout->addRow(m_buttonBox);
}

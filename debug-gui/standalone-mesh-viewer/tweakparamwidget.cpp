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
#include "tweakparamwidget.h"
#include <QHBoxLayout>

TweakParamWidget::TweakParamWidget(QWidget *parent) : QWidget(parent)
{
    m_layout = new QGridLayout;
    setLayout(m_layout);    
}

void TweakParamWidget::addParam(const QString &name, double min, double max, double init_val)
{
    QDoubleSlider *slider = new QDoubleSlider(Qt::Horizontal);
    slider->setMinimum(int(std::ceil(min * m_coeff)));
    slider->setMaximum(int(std::ceil(max * m_coeff)));

    QLabel *dispLabel = new QLabel("0");
    dispLabel->setMinimumWidth(30);

    int row = m_layout->rowCount();

    Param param{
        new QLabel(name),
          dispLabel,
          slider,
          min,
          max
    };

    m_params.insert(name, param);

    m_layout->addWidget(param.nameLabel, row, 0);
    m_layout->addWidget(param.slider, row, 1);
    m_layout->addWidget(param.valueDisp, row, 2);

    connect(slider, &QSlider::valueChanged,
            [=] (int value){
        double outVal = double(value) / m_coeff;
        dispLabel->setText(QString::number(outVal));

        emit paramChanged(name, outVal);
    });

    setParam(name, init_val);

}

void TweakParamWidget::setParam(const QString &name, double value)
{
    if(!m_params.contains(name))
        return;

    Param param = m_params.value(name);

    double outVal = value * m_coeff;

    param.valueDisp->setText(QString::number(outVal));

    param.slider->setValue(int(std::floor(outVal)));
}

double TweakParamWidget::value(const QString &name)
{
    if(!m_params.contains(name))
        return 0;

    Param param = m_params.value(name);
    int value = param.slider->value();
    return value / m_coeff;
}

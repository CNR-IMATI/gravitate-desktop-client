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
#include "transferfunctionfactory.h"

TransferFunctionFactory::TransferFunctionFactory()
{

}

vtkSmartPointer<vtkScalarsToColors> TransferFunctionFactory::createLut(PropertyType type, Mesh *mesh)
{
    vtkSmartPointer<vtkScalarsToColors> lut;
            //vtkSmartPointer<vtkScalarsToColors>::New();

    vtkSmartPointer<vtkColorSeries> series
            = vtkSmartPointer<vtkColorSeries>::New();

    switch(type)
    {
    case PropertyType::MEAN_CURVATURE:
    {
        // Automatic min and max;
        double level = 0.0;
        double window = 0.2;
        double contrast = 0.95;
        // Diverging color scale
        // Contrast = 0.95
        // Color scheme: diverging spectral ColorBrewer scheme (11 colors)
        series->SetColorScheme(vtkColorSeries::BREWER_DIVERGING_SPECTRAL_11);
        lut = makeCtfDiverging(series, window, level, contrast);
        break;
    }
    case PropertyType::SHAPE_INDEX:
    {
        // min = -1.0; max = 1.0
        double level = 0.0;
        double window = 1.0;
        double contrast = 0.0;
        // Diverging color scale
        // Contrast = 0
        // Color scheme: diverging spectral ColorBrewer scheme (11 colors)
        series->SetColorScheme(vtkColorSeries::BREWER_DIVERGING_SPECTRAL_11);
        lut = makeCtfDiverging(series, window, level, contrast);
        break;
    }
    case PropertyType::LIGHTNESS:
    {
        // min = 10, max = 98
        // Linear color scale
        // Color scheme: black to white
        double min = 10;
        double max = 98;
        lut = makeBwLut(min, max);
        break;
    }
    case PropertyType::SDF:
    {
        // min = 0, max = 25
        // Linear color scale
        // Color scheme: yellow-orange-brown sequential ColorBrewer scheme (9 colors)
        double min = 0;
        double max = 5;
        bool invert = true;
        series->SetColorScheme(vtkColorSeries::BREWER_SEQUENTIAL_YELLOW_ORANGE_BROWN_9);
        lut = makeCtf(series, min, max, invert);
        break;
    }
    default:
        break;
    }

    return lut;
}

vtkSmartPointer<vtkColorTransferFunction> TransferFunctionFactory::makeCtf(vtkColorSeries *colorSeries, double min, double max, bool invert)
{
    vtkSmartPointer<vtkColorTransferFunction> ctf
        = vtkSmartPointer<vtkColorTransferFunction>::New();

    int numColors = colorSeries->GetNumberOfColors();
    for(int i = 0; i < numColors; i++)
    {
        int colorIndex = invert ? i : numColors - 1 - i;

        vtkColor3ub color = colorSeries->GetColor( colorIndex );
        double rgb[3];
        rgb[0] = static_cast<double> (color[0]) / 255.0;
        rgb[1] = static_cast<double> (color[1]) / 255.0;
        rgb[2] = static_cast<double> (color[2]) / 255.0;
        double t = min + double(i) * (max - min) / double ( numColors -1 );

        ctf->AddRGBPoint( t , rgb[0], rgb[1], rgb[2]);
//        qDebug() << tr("f[%1] = %2 %3 %4")
//                    .arg(t, 0, 'f', 8)
//                    .arg(rgb[0], 0, 'f', 5)
//                    .arg(rgb[1], 0, 'f', 5)
//                    .arg(rgb[2], 0, 'f', 5);
    }

    return ctf;
}

vtkSmartPointer<vtkColorTransferFunction> TransferFunctionFactory::makeBwLut(double min, double max)
{
    auto lut = vtkSmartPointer<vtkColorTransferFunction>::New();
    lut->SetColorSpaceToRGB();
    lut->AddRGBPoint(min,0,0,0); // Black
    lut->AddRGBPoint(max,1,1,1); // White
    lut->SetScaleToLinear();
    return lut;
}

vtkSmartPointer<vtkColorTransferFunction> TransferFunctionFactory::makeCtfDiverging(vtkColorSeries *colorSeries, double range, double midpoint, double compress, bool invert)
{
    vtkSmartPointer<vtkColorTransferFunction> ctf
            = vtkSmartPointer<vtkColorTransferFunction>::New();

    // Partially adapted from:
    // http://www.flong.com/texts/code/shapers_exp/
    auto dese = [] (double t, double k)
    {
        double epsilon = 0.000001;
        double min_param_k = 0.0 + epsilon;
        double max_param_k = 1.0 - epsilon;
        k = std::min( std::max(min_param_k, k ), max_param_k );

        return t <= 0.5 ?
            std::pow(2.0*t, 1.0 - k) / 2.0  :
            1.0 - std::pow( 2.0*( 1.0 - t), 1.0 - k ) / 2.0;

    };

    double min = midpoint - range;
    double max = midpoint + range;

    int numColors = colorSeries->GetNumberOfColors();
    int maxIndex = numColors - 1;
    for(int i = 0; i < numColors; i++)
    {
        int colorIndex = invert ?
                i : maxIndex - i;

        vtkColor3ub color = colorSeries->GetColor( colorIndex );
        double rgb[3];
        rgb[0] = static_cast<double> (color[0]) / 255.0;
        rgb[1] = static_cast<double> (color[1]) / 255.0;
        rgb[2] = static_cast<double> (color[2]) / 255.0;


        double tnorm = double(i) / double ( maxIndex );

        if(i > 0 && i < maxIndex)
        {
            tnorm = dese(tnorm, compress);
        }

        double t = min + tnorm * (max - min);


        ctf->AddRGBPoint( t , rgb[0], rgb[1], rgb[2]);
//        qDebug() << QObject::tr("f[%1] = %2 %3 %4 - %5")
//                    .arg(t, 0, 'f', 8)
//                    .arg(rgb[0], 0, 'f', 5)
//                    .arg(rgb[1], 0, 'f', 5)
//                    .arg(rgb[2], 0, 'f', 5)
//                    .arg(tnorm, 0, 'f', 5);
    }

    return ctf;
}

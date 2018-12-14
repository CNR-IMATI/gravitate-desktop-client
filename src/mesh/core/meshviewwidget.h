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
#ifndef MESHVIEWWIDGET_H
#define MESHVIEWWIDGET_H

#include <QWidget>
#include <QAbstractButton>
#include <QVector>
#include <QString>
#include <QToolBar>
#include <QList>
#include <QToolButton>

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkCornerAnnotation.h>
#include <vtkLightKit.h>

namespace Ui {
class MeshViewWidget;
}

enum class MeshVisualizationMode
{
    SOLID,
    WIREFRAME,
    POINTS
};

class MeshViewWidget : public QWidget
{
    Q_OBJECT

    struct LightingConf
    {
        const char * label;

        double keyLightAngle[2];

        double fillLightAngle[2];
//        double keyToFillRatio;

//        double headLightAngle[2];
//        double keyToHeadRatio;
    };

public:
    explicit MeshViewWidget(QWidget *parent = 0);
    ~MeshViewWidget();

    vtkRenderer *renderer();

    vtkRenderer *overlayRenderer();

    vtkRenderWindow *renderWindow();

    vtkRenderWindowInteractor *renderWindowInteractor();

    QToolBar *toolbar();

    QAction *addToolWidget(QWidget *widget);

    void setTextOverlay(const QString &text);

signals:
    void visualizationModeChanged(const MeshVisualizationMode &mode);

    void sceneResetToDefaultState();

public slots:
    void resetCameraToDefault();

    void clear();

    void clearInfo();

    void setInfo(const QString &info);

    void changeLightingToNextConf();

private:
    Ui::MeshViewWidget *ui;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderer> m_overlayRenderer;
    vtkSmartPointer<vtkCornerAnnotation> m_cornerAnnotation;
    vtkSmartPointer<vtkCamera> m_camera;
    vtkSmartPointer<vtkLightKit> m_lightKit;

    QToolButton *m_lightingButton;

    QList<LightingConf> m_lightingConfs;
    int m_currentLightConf = 0;

    QToolBar *m_toolBar;

    void initVisualizationContext();
    void initToolbar();
    void initLightingConfigurations();
    void setLightingConfiguration(int index);

    double m_defaultPosition[3];
    double m_defaultViewUp[3];
    double m_defaultFocalPoint[3];
    void saveCameraState();
    void loadCameraState();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif // MESHVIEWWIDGET_H

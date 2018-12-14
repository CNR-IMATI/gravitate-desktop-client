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
#ifndef INSPECTIONVIEWMAIN_H
#define INSPECTIONVIEWMAIN_H

#include <QWidget>
#include <QUrl>
#include <QList>
#include "abstractview.h"
#include "abstractdashboard.h"
#include "resolutionselector.h"
#include "meshpropertytoolbar.h"
#include <QCheckBox>
#include "artefactloadhelper.h"
#include "artefacturladapter.h"
#include "gravitaterepoutils.h"
#include "gravitatemetadata.h"

#include "inspectionitemwidget.h"

namespace Ui {
class InspectionViewMain;
}

class InspectionItemWidget;
class FacetOverlay;
class PropertyColoring;
class WireframeBox;
class AnnotationModelBase;
class AnnotationTool;

struct InspectionItem
{
    InspectionItemWidget *widget;
    Artefact3DMetadata artefact;
    FacetOverlay *facetOverlay;
    PropertyColoring *propertyColoring;
    WireframeBox *wireframeBox;
    AnnotationModelBase *annotationModel;
    AnnotationTool *annotationTool;
    MetadataModel *metadataModel;
};

class InspectionViewMain : public AbstractView
{
    Q_OBJECT
public:
    explicit InspectionViewMain(AbstractDashboard *dashboard, QWidget *parent = 0);
    ~InspectionViewMain();

    int numberOfLoadedItems();

    inline int maxItems() { return 2; } // Maximum number of items that can be loaded at once

    virtual void initTools() override;

    ResolutionSelector *m_resolutionSelector;
    MeshPropertyToolbar *m_propertyToolbar;

    void addInspectionItem(const InspectionItem &item);

    // AbstractView interface

signals:
    void resolutionChanged(ModelResolution resolution);

public slots:
    void onClipboardContextMenuOpen(QMenu *menu, const GResource &resource) override;

private slots:

    void loadAllFacets();

    void setFacetsVisible(bool visible);

    void loadBoundingBoxes();

    void setBoundingBoxesVisible(bool visible);

    void loadAnnotations();

    void setAnnotationsVisible(bool visible);

    void loadArtefact(const ArtefactId &artefactId);

    void changeResolution(ModelResolution resolution);

private:
    Ui::InspectionViewMain *ui;

    InspectionItem createInspectionItem(InspectionItemWidget *widget, const Artefact3DMetadata &artefact);

    QList<InspectionItem> activeItems();

    // Resolution selection

    void initResolutionSelection();

    ModelResolution activeResolution();

    ModelResolution m_currentResolution;

    // ---

    void initPropertySelection();

    void initPropertyLoadSignals();

    ModelResolution findTargetResolution(PropertyType type);

    void forEachLoadedItem(std::function<void (const InspectionItem &)> f);

    bool load3DModel(const InspectionItem &item);

    // Geometric Characterization
    void loadMeanCurvature(const InspectionItem &item);
    void loadShapeIndex(const InspectionItem &item);
    void loadLightness(const InspectionItem &item);
    void loadSDF(const InspectionItem &item);

    // Other properties
    void loadFacets(const InspectionItem &item);

    void loadBoundingBox(const InspectionItem &item);

    template<class Property, class Viewer>
    bool loadProperty(const InspectionItem &item, Viewer *viewer)
    {
        using namespace ArtefactLoading;

        ArtefactLoadHelper helper(m_dashboard, this);

        connect(&helper, &ArtefactLoadHelper::error,
                [this] (const QString &errorMessage) {
            QMessageBox::critical(this, tr("Error"), errorMessage );
        });

        QUrl remoteUrl = getUrl<Property>(activeResolution(), item.artefact);

        QString localFile;

        if(!helper.getLocalFile(remoteUrl, localFile))
        {
            return false;
        }

        if(!helper.loadLocalPropertyFile<Property>(localFile, viewer, item.artefact.unit))
        {
            return false;
        }

        return true;
    }

    template<class Property, class Viewer>
    void loadScalarField(const InspectionItem &item, Viewer *viewer)
    {
        auto type = propertyType<Property>();

        auto targetResolution = findTargetResolution(type);

        if(targetResolution != m_currentResolution)
        {
            changeResolution(targetResolution);
        }

//        if(!load3DModel(item, targetResolution))
//        {
//            qCritical() << "Could not load the 3D model - in" << Q_FUNC_INFO;
//            return;
//        }

        if(!loadProperty<Property>(item, viewer))
        {
            qCritical() << "Could not load the property - in" << Q_FUNC_INFO;
            return;
        }

        // FIXME: Remove duplicated code
        ArtefactLoading::initPropertySelector(m_propertyToolbar->propertySelector, type);
    }

    void loadScalarFieldProperty(const InspectionItem &item, PropertyType type);

    // Toolbar
    QList<InspectionItem> m_activeItems;

    QMenu *m_geomCharMenu;
};

#endif // INSPECTIONVIEWMAIN_H

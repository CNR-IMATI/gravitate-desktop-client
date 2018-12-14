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
#ifndef FRAGMENTVIEWMAIN_H
#define FRAGMENTVIEWMAIN_H

#include <QWidget>
#include "localannotationmodel.h"
#include "thesaurusmodel.h"
#include "artefact3dmetadata.h"
#include "metadatamodel.h"
#include "enums.h"
#include "property/propertycoloring.h"
#include "property/wireframebox.h"
#include "facet/facetoverlay.h"
#include "annotation/selectiontool.h"
#include "annotation/annotationtool.h"
#include "annotation/houghfeaturetool.h"

#include "abstractdashboard.h"
#include "abstractview.h"
#include "resolutionchangedialog.h"
#include "inspectionitemwidget.h"
#include "resolutionselector.h"
#include "meshpropertytoolbar.h"
#include "featureextractionalgorithm.h"

// Added for generalized property loader
#include "downloadprogressdialog.h"
#include <QMessageBox>
#include <exception>
#include "artefacturladapter.h"
#include "gravitatemetadata.h"
#include "artefactloadhelper.h"

struct FeatureExtractionDemoParams
{
    QString label;
    ArtefactId artefactId;
    QString outputFile;
    QList<int> outputClusters;
};

namespace Ui {
class FragmentViewMain;
}

class FragmentViewMain : public AbstractView
{
    Q_OBJECT

    friend class AnnotationHelperFragment;

public:
    explicit FragmentViewMain(AbstractDashboard *dashboard, QWidget *parent = 0);
    ~FragmentViewMain();
    virtual void initTools() override;

    void load3DModelFile(const QString &filename);

    // AbstractView interface
public:

    InspectionItemWidget *itemWidget();

    void setResolutionBlockingSignals(ModelResolution resolution);

public slots:
    void onClipboardContextMenuOpen(QMenu *menu, const GResource &resource) override;

private slots:

    void loadArtefact(const ArtefactId &artefactId);

    void handleLoadCHAP();

    void onAnnotationModeEnabled(bool enabled);
    /**
     * @brief loadArtefactAnnotations loads all the annotations of the
     * currently loaded artefact
     */
    void loadArtefactAnnotations();

    void onAnnotationRequest();

    void loadFacets();

    void onFacetsEnabled(bool enabled);

    void loadBoundingBox();

    void onMinimalBoundingBoxToggled(bool enabled);

    void onDebugLoadFileFromDisk();

    void onAnnotationDeleteRequested(const QModelIndex &index);

    void onActionFeatureRecognition();

    void setAnnotationToolsEnabled(bool enabled);

    void demoFeatureRecognition(const FeatureExtractionDemoParams &params);

    void onSaveCurrentFeaturesRequest();

private:
    Ui::FragmentViewMain *ui;

    bool getFeatureExtractionParamsDialog(const QUrl &modelUrl, const QUrl &facetUrl, FeatureExtractionParams &outParams);

    void initItemWidget();

    void initButtonStates();

    void initProperties();

    void initAnnotation();

    void initMetadataModel();

    void initFeatureRecognition();

    void initDemoFeatureRecognition();

    void initResolutionSelector();

    void setupSignals();

    void saveAnnotation(const PartAnnotation &annotation);

    bool isTermSelected();

    Term getSelectedTerm();

    QString indexFileLocation();

    ModelResolution selectVersionBySize(ModelResolution res);

    ModelResolution activeResolution();

    ModelResolution findTargetResolution(PropertyType type);

    bool change3DModelResolution(ModelResolution targetResolution);

    Artefact3DMetadata m_artefact;

    ResolutionSelector *m_resolutionSelector;

    ModelResolution m_previousResolution;

    MeshPropertyToolbar *m_propertyToolbar;

    // Scalar field property

    PropertyColoring *m_propertyColoring;

    // Other properties

    FacetOverlay *m_facetOverlay;

    WireframeBox *m_wireframeBox;

    // Partonomy

    ThesaurusModel *m_thesaurusModel;

    MetadataModel *m_metadataModel;

    LocalAnnotationModel *m_annotationModel;

    AnnotationTool *m_annotationTool;

    SelectionTool *m_selectionTool;

    // Feature Extraction
    HoughFeatureTool *m_houghFeatureTool;

    FeatureExtractionAlgorithm *m_featureAlgo;

    bool load3DModelFromUrl(const QUrl modelUrl);

    template<class Property, class Viewer>
    void loadLocalProperty(const Property &property, Viewer *viewer)
    {
        viewer->apply(property, m_artefact.unit);
    }

    template<class Property, class Viewer>
    void loadLocalPropertyFile(const QString &file, Viewer *viewer)
    {
        if(file.isEmpty())
        {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("File name is empty, could not load property"));
            return;
        }

        QFileInfo info(file);

        qInfo() << "Loading file:" << info.fileName();

        try {
            Property p;

            load(file.toStdString(), p);

            viewer->apply(p, m_artefact.unit);

        } catch (const std::exception &e) {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  e.what());
        }
    }


    template<class Property, class Viewer>
    bool loadProperty(Viewer *viewer)
    {
        using ArtefactLoading::ArtefactLoadHelper;

        ArtefactLoadHelper helper(m_dashboard, this);

        connect(&helper, &ArtefactLoadHelper::error,
                [this] (const QString &errorMessage) {
            QMessageBox::critical(this, tr("Error"), errorMessage );
        });

        QUrl remoteUrl = getUrl<Property>(activeResolution(), m_artefact);

        if(remoteUrl.isEmpty())
        {
            qCritical() << "Cannot load this property at the current resolution";
            return false;
        }

        QString localFile;

        if(!helper.getLocalFile(remoteUrl, localFile))
        {
            return false;
        }

        if(!helper.loadLocalPropertyFile<Property>(localFile, viewer, m_artefact.unit))
        {
            return false;
        }

        return true;
    }

    template<class Property>
    bool changeResolutionByProperty()
    {
        auto type = propertyType<Property>();

        auto targetResolution = findTargetResolution(type);

        if(targetResolution != activeResolution())
        {
            bool ok = change3DModelResolution(targetResolution);
            if(!ok)
            {
                return false;
            }
        }
        return true;
    }

    template<class Property, class Viewer>
    bool loadScalarFieldProperty(Viewer *viewer)
    {
        if(!changeResolutionByProperty<Property>())
        {
            return false;
        }

        auto type = propertyType<Property>();

        auto ok = loadProperty<Property, Viewer>(viewer);
        if(!ok)
        {
            return false;
        }

        ArtefactLoading::initPropertySelector(
                    m_propertyToolbar->propertySelector,
                    type);

        return true;
    }

    QUrl getCurrentUrlForProperty(MultiResProperty property);
};

#endif // FRAGMENTVIEWMAIN_H

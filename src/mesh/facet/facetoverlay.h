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
#ifndef FACETOVERLAY_H
#define FACETOVERLAY_H

#include <QObject>
#include "core/viewermodule.h"
#include "core/mesh.h"
#include "facet.h"
#include <vtkSmartPointer.h>
#include <QMap>
#include <vector>
#include "qudt_units.h"

class FacetOverlay : public ViewerModule
{
    struct RGB
    {
        int r, g, b;
    };

    Q_OBJECT
public:
    explicit FacetOverlay(Mesh *mesh, MeshViewWidget *parent = 0);

    void apply(const std::vector<Facet> &facets, qudt::Unit unit);

    bool loadFct(const QString &fileName);

    bool isLoaded();
signals:

public slots:
    void setVisible(bool visible);

    void reset();

private:

    bool m_loaded;

    Mesh * m_mesh;

    QList<Facet> m_facets;

    QList<vtkSmartPointer<vtkActor>> m_actors;

    void applyFacetColors();

    void applyFacetColors_v2();

    void removeCurrentActors();

    std::vector<int> extractCellIds(int facetId);

    QMap<Facet::FacetType, RGB> m_colorMap {
        { Facet::FacetType::EXTERNAL, { 156, 0, 0 }   },
        { Facet::FacetType::INTERNAL, { 0, 156, 0 }   },
        { Facet::FacetType::FRACTURE, { 0, 156, 156 } }
    };
};

#endif // FACETOVERLAY_H

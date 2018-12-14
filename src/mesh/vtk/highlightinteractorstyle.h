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
#ifndef HIGHLIGHTINTERACTORSTYLE_H
#define HIGHLIGHTINTERACTORSTYLE_H

#include <vtkVersion.h>


#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkAreaPicker.h>

#include <vtkActor.h>
#include <vtkAreaPicker.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h>
#include <vtkIdTypeArray.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkObjectFactory.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVersion.h>
#include <vtkVertexGlyphFilter.h>


#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1

class HighlightInteractorStyle : public vtkInteractorStyleRubberBandPick
{
  public:
    static HighlightInteractorStyle* New();
    vtkTypeMacro(HighlightInteractorStyle,vtkInteractorStyleRubberBandPick);

    HighlightInteractorStyle() : vtkInteractorStyleRubberBandPick()
    {
      this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
      this->SelectedActor = vtkSmartPointer<vtkActor>::New();
      this->SelectedActor->SetMapper(SelectedMapper);
    }

    virtual void OnLeftButtonUp()
    {
      // Forward events
      vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

      if(this->CurrentMode == VTKISRBP_SELECT)
        {
        vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

        vtkSmartPointer<vtkExtractPolyDataGeometry> extractPolyDataGeometry =
          vtkSmartPointer<vtkExtractPolyDataGeometry>::New();

        extractPolyDataGeometry->SetInputData(this->PolyData);
        extractPolyDataGeometry->SetImplicitFunction(frustum);
        extractPolyDataGeometry->Update();

        std::cout << "Extracted " << extractPolyDataGeometry->GetOutput()->GetNumberOfCells() << " cells." << std::endl;

        this->SelectedMapper->SetInputData(extractPolyDataGeometry->GetOutput());

        this->SelectedMapper->ScalarVisibilityOff();

//        vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetArray("OriginalIds"));

        this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
        this->SelectedActor->GetProperty()->SetPointSize(5);

        this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(SelectedActor);
        this->GetInteractor()->GetRenderWindow()->Render();
        this->HighlightProp(NULL);
        }
    }

    void SetPolyData(vtkSmartPointer<vtkPolyData> polyData) {this->PolyData = polyData;}
  private:
    vtkSmartPointer<vtkPolyData> PolyData;
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;

};
vtkStandardNewMacro(HighlightInteractorStyle);



#endif // HIGHLIGHTINTERACTORSTYLE_H

/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkMeshTovtkPolyData_hxx
#define itkMeshTovtkPolyData_hxx

#include <iostream>
#include "itkMeshTovtkPolyData.h"

namespace itk{

template<typename T>
itkMeshTovtkPolyData<T>
::itkMeshTovtkPolyData()
{
  m_itkTriangleMesh = TriangleMeshType::New();

}


template<typename T>
void
itkMeshTovtkPolyData<T>
::SetInput(typename TriangleMeshType::ConstPointer mesh)
{
  m_itkTriangleMesh = mesh;
}

template<typename T>
vtkSmartPointer<vtkPolyData>
itkMeshTovtkPolyData<T>
::GetOutput()
{
  return this->ConvertitkTovtk();
}

template<typename T>
vtkSmartPointer<vtkPolyData>
itkMeshTovtkPolyData<T>
::ConvertitkTovtk()
{
  int numPoints =  m_itkTriangleMesh->GetNumberOfPoints();

  InputPointsContainerPointer      myPoints = m_itkTriangleMesh->GetPoints();
  InputPointsContainerIterator     points = myPoints->Begin();
  PointType point;

  vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> m_Polys = vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkPolyData> m_PolyData = vtkSmartPointer<vtkPolyData>::New();

  if (numPoints == 0)
    {
      printf( "Aborting: No Points in GRID\n");
      return m_PolyData;
    }
  m_Points->SetNumberOfPoints(numPoints);

  int idx=0;
  double vpoint[3];
  while( points != myPoints->End() )
    {
    point = points.Value();
    vpoint[0]= point[0];
    vpoint[1]= point[1];
    vpoint[2]= point[2];
    m_Points->SetPoint(idx++,vpoint);
    points++;
    }

  m_PolyData->SetPoints(m_Points);

  CellsContainerPointer cells = m_itkTriangleMesh->GetCells();
  CellsContainerIterator cellIt = cells->Begin();
  vtkIdType pts[3];
  while ( cellIt != cells->End() )
    {
    CellType *nextCell = cellIt->Value();
    CellType::PointIdIterator pointIt = nextCell->PointIdsBegin() ;
    PointType  p;
    int i;

    switch (nextCell->GetType())
      {
      case CellType::VERTEX_CELL:
      case CellType::LINE_CELL:
      case CellType::POLYGON_CELL:
        break;
      case CellType::TRIANGLE_CELL:
        i=0;
        while (pointIt != nextCell->PointIdsEnd() )
        {
        pts[i++] = *pointIt++;
        }
        m_Polys->InsertNextCell(3,pts);
        break;
      default:
        printf("something \n");
      }
    cellIt++;
    }

  m_PolyData->SetPolys(m_Polys);
  return m_PolyData;
}
}
#endif

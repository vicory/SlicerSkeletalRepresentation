/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// SkeletalRepresentationInitializer Logic includes
#include "vtkSlicerSkeletalRepresentationInitializerLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

// include igl libraries
#include <igl/barycenter.h>
#include <igl/cotmatrix.h>
#include <igl/doublearea.h>
#include <igl/grad.h>
#include <igl/jet.h>
#include <igl/massmatrix.h>
#include <igl/per_vertex_normals.h>
#include <igl/readDMAT.h>
#include <igl/readOFF.h>
#include <igl/writeOFF.h>
#include <igl/repdiag.h>


//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerSkeletalRepresentationInitializerLogic);

//----------------------------------------------------------------------------
vtkSlicerSkeletalRepresentationInitializerLogic::vtkSlicerSkeletalRepresentationInitializerLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerSkeletalRepresentationInitializerLogic::~vtkSlicerSkeletalRepresentationInitializerLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerSkeletalRepresentationInitializerLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerSkeletalRepresentationInitializerLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerSkeletalRepresentationInitializerLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerSkeletalRepresentationInitializerLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerSkeletalRepresentationInitializerLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerSkeletalRepresentationInitializerLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}
void vtkSlicerSkeletalRepresentationInitializerLogic::FlowSurfaceMesh(const std::string &filename)
{
    using namespace Eigen;
    using namespace std;

    MatrixXd V,U;
    MatrixXi F;
    igl::readOFF(filename, V, F);
}
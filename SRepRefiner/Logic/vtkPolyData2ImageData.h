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
#ifndef VTKPOLYDATA2IMAGEDATA_H
#define VTKPOLYDATA2IMAGEDATA_H

#include <string>
#include <vtkSmartPointer.h>

class vtkImageData;
class vtkPolyData2ImageData
{
public:
    vtkPolyData2ImageData();

    void Convert(const std::string &inputFileName, vtkSmartPointer<vtkImageData> output);

};

#endif // VTKPOLYDATA2IMAGEDATA_H

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

#include "vtkSrep.h"
#include <math.h>
#include <stdlib.h>
#include "vtkSpoke.h"

// STD includes
#include <cstddef>

vtkSrep::vtkSrep()
{

}

vtkSrep::vtkSrep(int r, int c,  std::vector<double> &radii, std::vector<double> &dirs, std::vector<double> &skeletalPoints)
{
    nRows = r;
    nCols = c;
    int nCrestPoints = r * 2 + (c-2) * 2;
    int numSteps = static_cast<int>(floor(r/2)); // steps from crest point to the skeletal point

    for(int i = 0; i < nCrestPoints * (numSteps + 1); ++i)
    {
        int idTuple = i * 3;
        int currRowNum = i / (numSteps + 1);
        if(currRowNum > nCrestPoints / 2  && i % (numSteps + 1) == 0) {
            // repeated middle spokes in bot half
            idTuple = ((nCrestPoints/2 - (currRowNum - nCrestPoints/2)) * (numSteps + 1)) * 3;
        }
        vtkSpoke *s = new vtkSpoke(radii[idTuple / 3], skeletalPoints[idTuple], skeletalPoints[idTuple + 1], skeletalPoints[idTuple + 2],
                dirs[idTuple], dirs[idTuple + 1], dirs[idTuple + 2]);
        spokes.push_back(s);
    }
}

vtkSrep::~vtkSrep()
{
    for(size_t i = 0; i < spokes.size(); ++i)
    {
        if(spokes[i] == nullptr)
        {
            continue;
        }
        delete spokes[i];
        spokes[i] = nullptr;
    }
}

vtkSpoke *vtkSrep::GetSpoke(int r, int c) const
{
    if(spokes.empty())
    {
        return nullptr;
    }
    int num_row = nRows * 2 + (nCols-2) * 2;
    int num_col = 1+static_cast<int>(floor(nRows/2)); // steps from crest point to the skeletal point


    int id = 0;
    if(r > num_row - 1)
        id = c; // last row connect with the first row, because it's a circle
    else
        id = r * num_col + c;

    return spokes[id];
}

bool vtkSrep::IsEmpty() const
{
    return spokes.empty();
}

std::vector<vtkSpoke *> &vtkSrep::GetAllSpokes()
{
    return spokes;
}

std::vector<vtkSpoke *> &vtkSrep::copyFrom(std::vector<vtkSpoke *> &source)
{
    for (int i = 0; i < source.size(); ++i) {
        spokes[i] = source[i];
    }
    return spokes;
}

std::vector<double> &vtkSrep::GetAllSkeletalPoints()
{
    skeletalPts.clear();

    for(size_t i = 0; i< spokes.size(); ++i)
    {
        double pt[3];
        spokes[i]->GetSkeletalPoint(pt);
        skeletalPts.push_back(pt[0]);
        skeletalPts.push_back(pt[1]);
        skeletalPts.push_back(pt[2]);
    }
    return skeletalPts;
}

void vtkSrep::Refine(const double *coeff)
{
    if(spokes.empty())
    {
        return;
    }
    int nCrestPoints = nRows * 2 + (nCols-2) * 2;
    int numSteps = static_cast<int>(floor(nRows/2)); // steps from crest point to the skeletal point

    double epsilon = 1e-13;
    for(size_t i = 0; i < spokes.size(); ++i)
    {
        size_t idx = i * 4;
        int currRowNum = i % (numSteps + 1);
        if(i >= (nCrestPoints / 2 - 1) * (numSteps + 1) && i % (numSteps + 1) == 0) {
            // repeated middle spokes in bot half
            idx = ((nCrestPoints/2 - 1 - (currRowNum + 1 - nCrestPoints/2)) * (numSteps + 1)) * 4;
        }
        double newU[3], newR, oldR;
        newU[0] = coeff[idx];
        newU[1] = coeff[idx+1];
        newU[2] = coeff[idx+2];

        vtkSpoke* thisSpoke = spokes[i];
        oldR = thisSpoke->GetRadius();
        newR = exp(coeff[idx+3]) * oldR;

        double oldU[3];
        thisSpoke->GetDirection(oldU);

        if(abs(newR - oldR) < epsilon &&
                abs(newU[0] - oldU[0]) < epsilon  &&
                abs(newU[1] - oldU[1]) < epsilon  &&
                abs(newU[2] - oldU[2]) < epsilon) {

            //no changes for this spoke
            continue;
        }
        thisSpoke->SetDirection(newU);
        thisSpoke->SetRadius(newR);
    }
}

void vtkSrep::AddSpokes(std::vector<double> &radii, std::vector<double> &dirs, std::vector<double> &skeletalPoints)
{
    for (size_t i = 0; i < radii.size(); ++i) {
        size_t idTuple = i * 3;
        vtkSpoke *s = new vtkSpoke(radii[i], skeletalPoints[idTuple], skeletalPoints[idTuple + 1], skeletalPoints[idTuple + 2],
                dirs[idTuple], dirs[idTuple + 1], dirs[idTuple + 2]);
        spokes.push_back(s);
    }
}

void vtkSrep::ShiftSpokes(double shift)
{
    if(spokes.empty()) return;

    for (size_t i = 0; i < spokes.size(); ++i) {
        vtkSpoke *s = spokes[i];
        double dir[3], pt[3];
        s->GetDirection(dir);
        s->GetSkeletalPoint(pt);
        pt[0] = pt[0] + shift * dir[0];
        pt[1] = pt[1] + shift * dir[1];
        pt[2] = pt[2] + shift * dir[2];
        s->SetSkeletalPoint(pt[0], pt[1], pt[2]);
    }
}

void vtkSrep::DeepCopy(vtkSrep &src)
{
    this->nCols = src.GetNumCols();
    this->nRows = src.GetNumRows();

    spokes.clear();
    std::vector<vtkSpoke*> srcSpokes = src.GetAllSpokes();
    for (size_t i = 0; i < srcSpokes.size(); ++i) {
        vtkSpoke *tempSpoke = new vtkSpoke(*srcSpokes[i]);
        spokes.push_back(tempSpoke);
    }
}

int vtkSrep::GetNumRows() const
{
    return nRows;
}

int vtkSrep::GetNumCols() const
{
    return nCols;
}

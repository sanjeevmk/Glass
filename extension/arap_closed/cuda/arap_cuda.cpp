#include <torch/extension.h>
//#include "eigen/Eigen/Core"
//#include "eigen/Eigen/SVD"
//#include "../eigen/Eigen/Eigen"

//using namespace Eigen;
using namespace std;
// CUDA forward declarations

void ArapClosedKernelLauncher(
    const int b, const int n,
    const float* xyz,
    const float* xyzP,
    int* neighborList,
    int* numNeighbors,
    int* accnumNeighbors,
    const float* weightMatrix,
    const int blocklength,
    float* rotations,
    float* gradxyz);

void arap_forward_cuda(
    const at::Tensor xyz, 
    const at::Tensor xyzP, 
    const at::Tensor neighborList, 
    const at::Tensor numNeighbors, 
    const at::Tensor accnumNeighbors, 
    const at::Tensor weightMatrix,
    const at::Tensor rotations,
    const at::Tensor rhs)
{
    const int batchsize = xyz.size(0);
    const int n = xyz.size(1);

    const float* xyz_data = xyz.data<float>();
    const float* xyzP_data = xyzP.data<float>();
    int* neighborList_data = neighborList.data<int>();
    int* numNeighbors_data = numNeighbors.data<int>();
    int* accnumNeighbors_data = accnumNeighbors.data<int>();
    float* weightMatrix_data = weightMatrix.data<float>();
    const int blocklength_data = weightMatrix.size(1); //blocklength.data<int>();
    float* rotations_data = rotations.data<float>();
    float* rhs_data = rhs.data<float>();

    ArapClosedKernelLauncher(batchsize, n, xyz_data, xyzP_data,neighborList_data,numNeighbors_data,accnumNeighbors_data,weightMatrix_data,blocklength_data,rotations_data,rhs_data);
}


PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
    m.def("forward", &arap_forward_cuda, "Arap forward (CUDA)");
}

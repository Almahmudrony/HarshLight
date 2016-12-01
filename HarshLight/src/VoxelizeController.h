#pragma once

#include "Util.h"
#include "Camera.h"
#include "Material.h"
#include "Component.h"
#include <cuda_gl_interop.h>
#include <cuda_runtime.h>

class VoxelizeController : public Component
{
public: 
    explicit VoxelizeController(uint32_t dim, float extent, Camera* voxel_cam);
	virtual ~VoxelizeController();

    void Start() override;
    void Update(float dt) override { }
    
    void SetVoxelDim(uint32_t dim);
    uint32_t GetVoxelDim() const;

	inline const Texture3dCompute* GetVoxelizeTex()
	{ return m_VoxelizeTex; }

    cudaSurfaceObject_t TransferVoxelDataToCuda();
	void FinishVoxelDataFromCuda(cudaSurfaceObject_t surf_obj);
	
private:
	
	void DispatchVoxelization();

    static const char* s_VoxelDimName;
    static const char* s_ViewMtxToDownName;
    static const char* s_ViewMtxToLeftName;
    static const char* s_ViewMtxToForwardName;

    uint32_t m_VoxelDim;
    float m_Extent;
    Camera* m_VoxelCam;

	Texture3dCompute* m_VoxelizeTex;

	cudaGraphicsResource* m_CudaResource;
};
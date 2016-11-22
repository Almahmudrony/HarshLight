#version 450 core

in vec2 vs_Texcoord;
in vec3 vs_WorldPosition;
in vec3 vs_WorldNormal;

out vec4 fragColor;

uniform sampler2D TexAlbedo;
layout (rgba8) coherent uniform image3D TexVoxel;

uniform mat4 CamVoxelViewMtx;
uniform mat4 CamVoxelProjMtx;
uniform float VoxelDim

void main()
{
	vec4 voxel_space_pos = CamVoxelProjMtx * CamVoxelViewMtx * vs_WorldPosition;
	voxel_space_pos /= voxel_space_pos.w;
	voxel_space_pos *= VoxelDim;
	ivec3 voxel_idx = floor(voxel_space_pos);
	fragColor = imageLoad(TexVoxel?, voxel_idx);
}
#pragma once
#include "DirectionalLight.h"
#include <d3d12.h>

class Light
{
public:
	ID3D12Resource* GetDirectionalLightResource() { return directionalLightResource_; }
	void SetDirectionalLightResource(ID3D12Resource* directionalLightResource) { directionalLightResource_ = directionalLightResource; }

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	void CreateDirectionalResource(ID3D12Device* device);

	void Initialize(ID3D12Device* device);

	void DrawDebugParameter();

	void Release();

private:
	ID3D12Resource* directionalLightResource_;
	DirectionalLight* directionalLightData_;
};


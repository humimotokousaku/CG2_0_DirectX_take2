#pragma once
#include "../ModelData.h"
#include <vector>

enum ModelName {
	PLANE,
	AXIS,
	MULTIMESH,
	MULTIMATERIAL,
	BUNNY
};

class ObjManager
{
public:
	static ObjManager* GetInstance();

	ModelData* GetObjModelData() { return modelData_; }

	void Initialize();

	void LoadObjFiles();

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	static const int32_t kMaxModelData = 5;
	ModelData modelData_[kMaxModelData];
};

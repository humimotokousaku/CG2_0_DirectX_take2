#pragma once

enum Scene {
	TITLESCENE,
	GAMESCENE
};

class IScene
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	int GetSceneNum() { return sceneNum; }
protected:
	static int sceneNum;
};

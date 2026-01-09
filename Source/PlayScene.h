#pragma once
#include "../Library/SceneBase.h"

class PlayScene : public SceneBase
{
public:

	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
private:
	int hImage_;
	int prevTime_;
	
};

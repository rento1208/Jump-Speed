#pragma once
#include "../Library/SceneBase.h"

/// <summary>
/// タイトルシーン
/// 
/// タイトルを表示して、キーを押したらプレイシーンに移行する。
/// </summary>
class GameClearScene : public SceneBase
{
public:
	GameClearScene();
	~GameClearScene();
	void Update() override;
	void Draw() override;
private:
	int hImage;
	float changeTimer;

};

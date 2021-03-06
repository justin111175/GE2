#include <Dxlib.h>
#include <_DeBugConOut.h>
#include "GameScene.h"
#include "TitleScene.h"
#include <Vector2.h>
#include "SceneMng.h"
#include <utility>
#include "EffectMng.h"
#include "../common/ImageMng.h"
#include "../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"
#include "MeanScene.h"
#include "GameOverScene.h"

// 一人プレイモード
GameScene::GameScene()
{
	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(450, 50)), std::move(Vector2(700 + 450, 800))));

	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });
	IpImageMng.GetID("X", "image/X.png", { 57,57 }, { 1,1 });

	for (int i = 0; i < 2; i++)
	{
		controller[i].try_emplace(conType::Key, std::make_unique<KeyInput>());
		controller[i][conType::Key]->SetUp(i);
		controller[i].try_emplace(conType::Pad, std::make_unique<PadInput>());
		controller[i][conType::Pad]->SetUp(i);

	}


	overFlag_ = false;
}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{

	Draw();

	if (!FadeUpdate())
	{

		for (auto&& state : playerState)
		{
			if (!_timeCount.GetFlag("待つ"))
			{
				state->Run();
			}

			if (state->overFlag_)
			{
				return std::make_unique<GameOverScene>(std::move(own));
			}
		}
		for (auto&& conType_ : conType())
		{

			if (MeanCtl(conType_))
			{
				return std::make_unique<MeanScene>(std::move(own));
			}

		}
	}

	IpEffect.Updata();
	return std::move(own);
}



void GameScene::BaseDraw(void)
{
	for (auto&& state : playerState)
	{
		state->Draw();
	}

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, IMAGE_ID("BG")[0], true);

	DrawGraph(0, 0, playerState[0]->GetScreenId(), true);
}



void GameScene::Draw(void)
{
	ClsDrawScreen();
	BaseDraw();

	IpEffect.Draw();

	if (IpSceneMng._blendCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, IpSceneMng._blendCnt);
		DrawGraph(0, 0, IpImageMng.GetID(_fadeType)[0], true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}
	ScreenFlip();
}





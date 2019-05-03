/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "MainGameScene.h"
#include "TitleScene.h"
#include <memory>
#include <iostream>


/// エントリーポイント.
int main()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();


	if (!window.Init(1440, 810, "Infection Village")) {

		return 1;
	}

	//OpenALのライブラリ初期化
	Sound::EngineInitialize();

	std::shared_ptr<Scene> pScene(new TitleScene);
	if (!pScene->Initialize()) {
		return 1;
	}

	//ゲーム終了フラグ
	bool Exit = false;


	// メインループ.
	window.InitTimer();
	while (!window.ShouldClose() && !Exit)
	{
		const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

		window.Update();
		Sound::EngineUpdate(deltaTime);


		//ゲーム終了
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			Exit = true;
		}



		// シーンを切り替える.
		if (!pScene->NextScene().empty()) {
			glFinish();
			pScene->Finalize();

			if (pScene->NextScene() == "Title")
			{

				pScene.reset(new TitleScene);

			}
			else if (pScene->NextScene() == "MainGame")
			{
				pScene.reset(new MainGameScene);

			}
			if (!pScene->Initialize())
			{
				return 1;
			}
			window.InitTimer();
		}

		pScene->ProcessInput();
		pScene->Update();
		pScene->Render();
		window.SwapBuffers();

	}
	pScene->Finalize();
	return 0;
}

/**
* @file TitleScene.h
*/
#include "TitleScene.h"
#include <glm/gtc/matrix_transform.hpp>

/**
* 初期化.
*/
bool TitleScene::Initialize()
{
	std::vector<std::string> modelList;
	modelList.push_back("Res/Plane.obj");

	if (!meshList.Allocate(modelList)) {
		return false;
	}

	//シェーダーのセット
	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));

	//タイトル背景のセット
	texBackGround.Reset(Texture::LoadImage2D("Res/TitleBack1.tga"));

	//SOUNDの初期化やVOLUME設定
	sTitleBGM.Init(SOUND_PASS_TITLEBGM);
	sTitleBGM.SetVolume(1.2f);
	sTitleCall.Init(SOUND_PASS_TITLECALL);
	sTitleCall.SetVolume(0.2f);
	sTitleBGM.Play(DEFAULT_FADETIME * 2);
	sTitleBGM.SetLooping(true);

	backGroundAlpha = 1.0f;
	fadetime = 0;
	ChangeStageFlag = false;

	return true;
}

/**
* 入力の処理.
*/
void TitleScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.IsKeyDown(GLFW_KEY_ENTER) || window.GetMouseButton(0) || window.GetMouseButton(1)) {
		if (Start)
		{
			Start = false;
			//fadeを終わらせる
			fadetime = 1;
		}
		else
		{
			ChangeStageFlag = true;
			sTitleBGM.Stop(DEFAULT_FADETIME * 2);
			sTitleCall.Stop(DEFAULT_FADETIME * 2);
		}

	}
}

/**
* 状態の更新.
*/
void TitleScene::Update()
{
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

	// 座標変換行列を作成する.
  //  const glm::mat4x4 matProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
	const glm::mat4x4 matProj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, 1.0f, 500.0f);
	const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progSimple.SetViewProjectionMatrix(matProj * matView);

	if (ChangeStageFlag)
	{

		if (fadetime >= 0)
		{
			fadetime -= deltaTime / 2;
		}
		else
		{
			NextScene("MainGame");
		}
	}

	if (Start)
	{
		if (fadetime <= 1)
		{
			fadetime += deltaTime / 2;
		}
		else
		{
			fadetime = 1;
			Start = false;
			sTitleCall.Play();
		}
	}

}

/**
* 描画.
*/
void TitleScene::Render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	progSimple.Use();

	meshList.BindVertexArray();

	progSimple.BindTexture(0, texBackGround.Get());
	progSimple.Draw(meshList[0], glm::vec3(640, 360, -1), glm::vec3(0), glm::vec3(640, 360, 1), backGroundAlpha * fadetime);


}

/**
* 終了.
*/
void TitleScene::Finalize()
{
}


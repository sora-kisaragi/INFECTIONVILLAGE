/**
* @file TitleGameScene.h
*/
#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include "Scene.h"
#include "SoundList.h"

#define	DEFAULT_FADETIME 1.0f

/**
* タイトル画面.
*/
class TitleScene : public Scene
{
public:
	TitleScene() = default;
	virtual ~TitleScene() = default;

	virtual bool Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Finalize() override;

private:
	MeshList meshList;

	Texture::Image2D texBackGround;
	Texture::Image2D texLogo;
	Texture::Image2D texPushAnyKey;

	Shader::Program progSimple;

	float backGroundAlpha = 1.0f;
	float fadetime = DEFAULT_FADETIME;
	bool Start = true;
	bool ChangeStageFlag = false;

	Sound::Player sTitleBGM;
	Sound::Player sTitleCall;

};

#endif // TITLESCENE_H_INCLUDED
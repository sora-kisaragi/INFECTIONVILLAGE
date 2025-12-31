/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include "Scene.h"
#include "Actor.h"
#include "SoundList.h"
#include <random>
#include <iostream>
#include <memory>



//フェード用マクロ
#define	DEFAULT_FADERATE 1.0f
//ゾンビの攻撃速度
#define DEFAULT_ATTACKSPEED 1.2f
//カメラのオフセット座標
#define DEFAULT_CAM_OFFSET_TPS glm::vec3(0.4,2.3,2)
#define DEFAULT_CAM_OFFSET_FPS glm::vec3(0,1.8,0);
//銃のMAX残弾数
#define DEFAULT_SG_REMAINING 10
#define DEFAULT_AL_REMAINING 60




/**
* プレイヤーが操作するアクター.
*/
class PlayerActor : public Actor
{
public:
	virtual ~PlayerActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
* プレイヤーから発射される弾のアクター.
*/
class BulletActor : public Actor
{
public:
	virtual ~BulletActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
* 敵(ゾンビ)のアクター.
*/
class ZombieActor : public Actor
{
public:
	virtual ~ZombieActor() = default;
	virtual void Update(float deltaTime) override;

public:
	Actor* target = nullptr;
	float attackingTimer = DEFAULT_ATTACKSPEED;
	bool isAttacking = false;
	float baseSpeed = 1.0f;
};

/**
* メインゲーム画面.
*/
class MainGameScene : public Scene
{
public:
	MainGameScene() = default;
	virtual ~MainGameScene() = default;

	virtual bool Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Finalize() override;


private:
	MeshList meshList;

	Texture::Image2D texGround;
	Texture::Image2D texTree;
	Texture::Image2D texHouse;
	Texture::Image2D texRock;
	Texture::Image2D texHuman;
	Texture::Image2D texZombie;
	Texture::Image2D texStageClear;
	Texture::Image2D texGameOver;
	Texture::Image2D texBullet;
	Texture::Image2D texHP;
	Texture::Image2D texDay;
	Texture::Image2D texScore;
	Texture::Image2D texNumber[10];
	Texture::Image2D texFadeSheet;
	Texture::Image2D texEffectSheet;
	Texture::Image2D texDebugImage;

	Shader::Program progSimple;
	Shader::Program progLighting;
	Shader::LightList lights;

	glm::vec3 viewPos;
	float pointLightAngle;


	//==================カメラ関係==========================
	float P_T_Distance = 100;
	glm::vec3 targetPos = glm::vec3(0, 0, 0);
	glm::vec3 vPos = glm::vec3(0, 0, 0);
	int posy = 0;
	int lastposY = 0;
	float CameraAngleY = 0;
	float CameraAngleX = 0;
	float CameraRotateSpeed = 75;
	enum class PlayMode
	{
		eFPS,
		eTPS,
	};
	PlayMode playmode = PlayMode::eFPS;

	enum class State
	{
		play,
		stageClear,
		gameOver,
	};
	State state = State::play;

	std::mt19937 random;

	PlayerActor player;
	std::vector<std::unique_ptr<Actor>> playerBulletList;
	std::vector<std::unique_ptr<Actor>> enemyList;
	std::vector<std::unique_ptr<Actor>> objectList;
	std::vector<std::unique_ptr<Actor>> houseList;
	std::vector<std::unique_ptr<Actor>> treeList;
	std::vector<std::unique_ptr<Actor>> rockList;

	float playerBulletTimer = 0;
	int score = 0;

	int stageNo = 1;
	float enemySpeed = 1.0f; // ゾンビの移動速度.
	float enemyPoppingInterval = 15.0f;
	float enemyPoppingTimer = 3.0f;

	int enemyTotal = 100; // 敵の総数.
	int enemyLeft = 100; // 未登場の敵の数. 敵を出現させるたびに減少していく.
	int enemyKilled = 0; // 殺した敵の数. この数値がenemyTotalと等しくなったらステージクリア.

	//フェード用変数
	float backGroundAlpha = 1.0f;
	float faderate = DEFAULT_FADERATE;
	bool fadeOut = false;
	bool fadein = true;
	bool fadeinit = false;
	bool stageClearMove = false;
	bool gameOverMove = false;
	float movetimer = 2;

	//StageのSE用変数?
	bool StageFinishSE = true;

	double mousePosX;
	double LastMousePosX;
	//ショットガンの残弾数
	int sg_Remaining = DEFAULT_SG_REMAINING;
	//アサルトライフルの残弾数
	int al_Remaining = DEFAULT_AL_REMAINING;
	float reloadtimer = 0.0f;

	//Sound変数
	Sound::Player sMainBGM;
	Sound::Player sShotSE;
	Sound::Player sChangeWeapon;
	Sound::Player sAssaultSE;
	Sound::Player sShotGunSE;
	Sound::Player sALReload;
	Sound::Player sGameOverBGM;
	Sound::Player sClearSE;
	Sound::Player3D sHitEnemy;
	Sound::Player3D sHitObject;
	Sound::Player3D sZombiePopSE;
	Sound::Player3D sZombieDeadSE;
	Sound::Player3D SpecialBGM;
	Sound::Player3D SpecialBGM2;
	Sound::Player3DCustom SpecialBGM3;


	float soundfade = 0;
	bool isShotGun = false;
	bool isInput = false;
	bool hit = false;

	bool DEBUGMODE = false;
	float time = 0.2f;

};

#endif // MAINGAMESCENE_H_INCLUDED
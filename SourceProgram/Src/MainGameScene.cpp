/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#define DEFAULT_PLAYER_SPEED 10.0f

/**
* プレイヤーの状態を更新する.
*
* @param deltaTime 経過時間.
*/
void PlayerActor::Update(float deltaTime)
{
	//HPが 0以上 ならば
	if (Actor_HP <= 0) {
		//正面へ向ける (rad -45°)
		rotation.x -= glm::radians(45.0f) * deltaTime;

		//Playerのrotation.xが {rad -90°} よりも小さければ
		if (rotation.x < glm::radians(-90.0f)) {

			//正面へ向ける { (-45°) + (-90°) }
			rotation.x = glm::radians(-90.0f);
		}

	}

	Actor::Update(deltaTime);
}

/**
* プレイヤーの弾の状態を更新する.
*/
void BulletActor::Update(float deltaTime)
{
	position.y *= 0.98;
	for (int i = 0; i < 3; ++i) {
		//Position {ステージサイズ} が {-200~200} の間ならば
		if (position[i] < -200 || position[i] > 200) {
			//HPを0にする
			Actor_HP = 0;
		}//地面より下になったら
		if (position[1] < 0)
		{
			//HPを0にする
			Actor_HP = 0;
		}
	}
	Actor::Update(deltaTime);
}

/**
* ゾンビの状態を更新する.
*/
void ZombieActor::Update(float deltaTime)
{
	//もしPlayerをTragetしてなかったら
	if (!target) {
		//Update終了
		return;
	}

	bool hit = false;
	//移動速度を設定 {ベース x2}
	const float moveSpeed = baseSpeed * 2.0f;
	//回転速度を設定 {ベース x2}
	const float rotationSpeed = baseSpeed * glm::radians(150.0f);
	//攻撃範囲?
	const float frontRange = glm::radians(15.0f);

	const glm::vec3 v = target->position - position;
	const glm::vec3 vTarget = glm::normalize(v);
	float radian = std::atan2(-vTarget.z, vTarget.x) - glm::radians(90.0f);
	if (radian <= 0) {
		radian += glm::radians(360.0f);
	}
	const glm::vec3 vZombieFront = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
	if (std::abs(radian - rotation.y) > frontRange) {
		const glm::vec3 vRotDir = glm::cross(vZombieFront, vTarget);
		if (vRotDir.y >= 0) {
			rotation.y += rotationSpeed * deltaTime;
			if (rotation.y >= glm::radians(360.0f)) {
				rotation.y -= glm::radians(360.0f);
			}
		}
		else {
			rotation.y -= rotationSpeed * deltaTime;
			if (rotation.y < 0) {
				rotation.y += glm::radians(360.0f);
			}
		}
	}

	// 十分に接近していなければ移動する. 接近していれば攻撃する.
	if (glm::length(v) > 1.0f) {
		velocity = vZombieFront * moveSpeed;
	}
	else {
		velocity = glm::vec3(0);


		// 定期的に攻撃状態になる.
		if (isAttacking) {
			isAttacking = false;
			attackingTimer = DEFAULT_ATTACKSPEED;
		}
		else {
			attackingTimer -= deltaTime;
			if (attackingTimer <= 0) {
				isAttacking = true;
			}
		}
	}
	Actor::Update(deltaTime);
}



/**
* 初期化.
*/
bool MainGameScene::Initialize()
{
	random.seed(std::random_device()());

	//Soundの初期化
	sMainBGM.Init(SOUND_PASS_MAINMAPBGM);
	sShotSE.Init(SOUND_PASS_SHOTSE);
	sAssaultSE.Init(SOUND_PASS_ASSAULT);
	sShotGunSE.Init(SOUND_PASS_SHOTGUN);
	sChangeWeapon.Init(SOUND_PASS_CANGEGUN);
	sALReload.Init(SOUND_PASS_AS_RELOAD);
	sHitEnemy.Init(SOUND_PASS_HIT_ENEMY);
	sHitObject.Init(SOUND_PASS_HIT_OBJECT);
	sGameOverBGM.Init(SOUND_PASS_GAMEOVER);
	sZombiePopSE.Init(SOUND_PASS_ZOMBIEPOP);
	sZombieDeadSE.Init(SOUND_PASS_ZONBIE_DEADSE);
	sClearSE.Init(SOUND_PASS_CLEARSE);
	SpecialBGM.Init(SOUND_PASS_SPECIAL);
	SpecialBGM2.Init(SOUND_PASS_SPECIAL2);
	SpecialBGM3.Init(SOUND_PASS_SPECIAL3);
	sMainBGM.SetLooping(true);
	sMainBGM.Play(3.0f);
	sAssaultSE.SetLooping(true);
	sShotGunSE.SetVolume(0.5f);
	SpecialBGM.SetLooping(true);
	SpecialBGM.SetPosition(0, 0, 0);
	SpecialBGM.SetReferenceDistance(5.0f);
	SpecialBGM.SetMaxDistance(200.0f);

	SpecialBGM2.SetLooping(true);
	SpecialBGM2.SetVolume(4.0f);
	SpecialBGM3.SetLooping(true);
	SpecialBGM3.SetVolume(1.5f);
	SpecialBGM3.SetDirection(90.0f, 0.0f, 0.0f);
	SpecialBGM3.SetConeInnerAngle(45.0f);
	SpecialBGM3.SetConeOuterAngle(60.0f);
	sHitObject.SetReferenceDistance(5.0f);
	sHitEnemy.SetReferenceDistance(5.0f);
	sZombiePopSE.SetReferenceDistance(5.0f);
	sZombiePopSE.SetMaxDistance(50.0f);
	sZombieDeadSE.SetMaxDistance(8.0f);
	sZombieDeadSE.SetReferenceDistance(5.0f);
	sZombieDeadSE.SetVolume(5.0f);
	Sound::ListenerUpVector(0, 1, 0);


	//Modelの読み込み
	std::vector<std::string> modelFiles;
	modelFiles.push_back("Res/Tree.obj");	//0
	modelFiles.push_back("Res/House.obj");	//1
	modelFiles.push_back("Res/Rock.obj");	//2
	modelFiles.push_back("Res/Ground.obj");	//3
	modelFiles.push_back("Res/Human.obj");	//4
	modelFiles.push_back("Res/Plane.obj");	//5
	modelFiles.push_back("Res/Bullet.obj");	//6
	modelFiles.push_back("Res/zombi.obj");	//7


	if (!meshList.Allocate(modelFiles)) {
		return false;
	}
	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));
	progLighting.Reset(Shader::BuildFromFile("Res/FragmentLighting.vert", "Res/FragmentLighting.frag"));

	//テクスチャの適用
	texGround.Reset(Texture::LoadImage2D("Res/Ground.tga"));
	texTree.Reset(Texture::LoadImage2D("Res/Tree.tga"));
	texHouse.Reset(Texture::LoadImage2D("Res/House.tga"));
	texRock.Reset(Texture::LoadImage2D("Res/Rock.tga"));
	texHuman.Reset(Texture::LoadImage2D("Res/Human.tga"));
	texZombie.Reset(Texture::LoadImage2D("Res/Zombie.tga"));
	texBullet.Reset(Texture::LoadImage2D("Res/Bullet.tga"));

	texStageClear.Reset(Texture::LoadImage2D("Res/StageClear.tga"));
	texGameOver.Reset(Texture::LoadImage2D("Res/GameOver.tga"));
	texHP.Reset(Texture::LoadImage2D("Res/HP.tga"));
	texDay.Reset(Texture::LoadImage2D("Res/Day.tga"));
	texScore.Reset(Texture::LoadImage2D("Res/Score.tga"));
	for (int i = 0; i < 10; ++i) {
		std::string filename = "Res/Number_";
		filename += '0' + i;
		filename += ".tga";
		texNumber[i].Reset(Texture::LoadImage2D(filename.c_str()));
	}
	texFadeSheet.Reset(Texture::LoadImage2D("Res/FadeSheet.tga"));
	texEffectSheet.Reset(Texture::LoadImage2D("Res/Effect.tga"));
	texDebugImage.Reset(Texture::LoadImage2D("Res/sorachan.tga"));

	// ライトの設定.
	lights.ambient.color = glm::vec3(0.05f, 0.01f, 0.01f);
	//赤っぽい色の雰囲気==========================================================================================
	lights.directional.direction = glm::normalize(glm::vec3(3, -8, -2));
	lights.directional.color = glm::vec3(1.2, 0.8, 0.8);


	pointLightAngle = 0;

	//Playerを初期化
	player.Initialize(4, texHuman.Get(), 10, glm::vec3(8, 0, 8), glm::vec3(0), glm::vec3(1));

	//Playerの当たり判定サイズ
	player.colLocal = { {-0.5f, 0, -0.5f }, {1, 1.7f, 1} };
	//Playerの弾数をリサイズ
	playerBulletList.resize(256);

	//弾128個分作る
	for (auto& e : playerBulletList) {
		e = new BulletActor;
	}
	//enemyをリサイズ
	enemyList.resize(128);
	for (auto& e : enemyList) {
		e = new ZombieActor;
	}

	//オブジェクトをリサイズ
	objectList.resize(2);
	for (auto& e : objectList) {
		e = new Actor;
	}

	//家のリストをリサイズ
	houseList.resize(128);
	for (auto& e : houseList)
	{
		e = new Actor;
	}
	//木のリストをリサイズ
	treeList.resize(256);
	for (auto& e : treeList)
	{
		e = new Actor;
	}
	//岩のリストをリサイズ
	rockList.resize(64);
	for (auto& e : rockList)
	{
		e - new Actor;
	}

	//マップの左上をベースにする
	glm::vec3 basepos = glm::vec3(-195, 0, -195);
	glm::vec3 HouseBasePos = glm::vec3(0, 0, 0);

	glm::vec3 treeBasePos1 = glm::vec3(-4, 0, -4);
	glm::vec3 treeBasePos2 = glm::vec3(-6, 0, 8);
	glm::vec3 treeBasePos3 = glm::vec3(5, 0, -6);
	glm::vec3 treeBasePos4 = glm::vec3(4, 0, -10);

	glm::vec3 rockBasePos1 = glm::vec3(4, 0, 5);
	glm::vec3 rockBasePos2 = glm::vec3(-5, 0, -5);
	int createHouse = 90;
	int nowCreateHouse = 0;
	int nowtree = 0;
	int nowrock = 0;

	//家同士の間隔
	std::uniform_int_distribution<int> baseMoveRangeX(80.0f, 120.0f);
	std::uniform_int_distribution<int> baseMoveRangeZ(20.0f, 40.0f);

	//家のrotationとscale
	std::uniform_int_distribution<int> houseRotationRange(0.0f, 360.0f);
	std::uniform_int_distribution<int> houseScaleRange(4.0f, 8.0f);

	//木のrotationとscale
	std::uniform_int_distribution<int> treeRotationRange(0.0f, 360.0f);
	std::uniform_int_distribution<int> treeScaleRangeXZ(0.7f, 1.2f);
	std::uniform_int_distribution<int> treeScaleRangeY(1.0f, 1.8f);

	//岩のrotationとscale
	std::uniform_int_distribution<int> rockRotationRange(0.0f, 360.0f);
	std::uniform_int_distribution<int> rockScaleRange(0.7f, 1.2f);

	//オブジェクトの生成
	for (int i = 0; i < createHouse; ++i)
	{

		//========================家を生成する===============================
		//ベースになるpointを設定
		//もし 1 ~ 3回の作成なら x軸をずらす
		if ((i % 4) != 0)
		{
			basepos.x += baseMoveRangeX(random);

		}
		//もし 5回目の作成なら z軸をずらす
		else if ((i % 4) == 0)
		{
			basepos.x = -195.0f;
			basepos.z += baseMoveRangeZ(random);
		}

		//ベースのtransformを生成
		glm::vec3 houseSpawnPos = basepos + HouseBasePos;
		glm::vec3 houseSpawnRot = glm::vec3(0, houseRotationRange(random), 0);
		glm::vec3 houseSpawnSca = glm::vec3(houseScaleRange(random));

		//家の生成処理
		houseList[(size_t)i]->Initialize(1, texHouse.Get(), 1, houseSpawnPos, houseSpawnRot, houseSpawnSca * 0.1f);
		houseList[(size_t)i]->colLocal = { {-(houseSpawnSca.x * 0.5), 0, -(houseSpawnSca.x * 0.5) }, { houseSpawnSca} };
		houseList[(size_t)i]->Update(0);


		//========================木を生成する=====================
		for (int i = 0; i < 2; ++i)
		{

			//ベースのtransformをセット
			glm::vec3 treeSpawnPos = houseSpawnPos;
			glm::vec3 treeSpawnRot = glm::vec3(0, treeRotationRange(random), 0);
			glm::vec3 treeSpawnSca = glm::vec3(treeScaleRangeXZ(random));
			treeSpawnSca.y = treeScaleRangeY(random);
			//生成回数によってpositionの変更
			if ((i % 3) == 0)
			{
				treeSpawnPos += treeBasePos1;
			}
			else if ((i % 3) == 1)
			{
				treeSpawnPos += treeBasePos2;
			}
			else if ((i % 3) == 2)
			{
				treeSpawnPos += treeBasePos3;
			}
			//生成処理
			treeList[(size_t)nowtree]->Initialize(0, texTree.Get(), 1, treeSpawnPos, treeSpawnRot, treeSpawnSca);
			treeList[(size_t)nowtree]->colLocal = { {-(treeSpawnSca.x * 0.5), 0, -(treeSpawnSca.z * 0.5) }, { treeSpawnSca } };
			treeList[(size_t)nowtree]->Update(0);
			nowtree += 1;
		};

		//if ((i % 2) == 0)
		//{
		//	//ベースのtransformをセット
		//	glm::vec3 rockSpawnPos = houseSpawnPos;
		//	glm::vec3 rockSpawnRot = glm::vec3(0, rockRotationRange(random), 0);
		//	glm::vec3 rockSpawnSca = glm::vec3(rockScaleRange(random), rockScaleRange(random), rockScaleRange(random));

		//	//生成回数によってpositionの変更
		//	if ((i % 2) == 0)
		//	{
		//		rockSpawnPos += rockBasePos1;
		//	}
		//	else if ((i % 2) == 1)
		//	{
		//		rockSpawnPos += rockBasePos2;
		//	}


		//	//生成処理
		//	rockList[(size_t)nowrock]->Initialize(2, texRock.Get(), 1, rockSpawnPos, rockSpawnRot, rockSpawnSca);
		//	rockList[(size_t)nowrock]->colLocal = { {-(rockSpawnSca.x * 0.5), 0, -(rockSpawnSca.z * 0.5) }, { rockSpawnSca } };
		//	rockList[(size_t)nowrock]->Update(0);
		//	nowrock += 1;
		//}

		nowCreateHouse += 1;
	}



	objectList[1]->Initialize(3, texGround.Get(), 1, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1));

	enemyTotal = 20;
	enemyLeft = enemyTotal;
	enemyKilled = 0;

	//PlayModeをFPSに設定
	playmode = PlayMode::eFPS;

	//Shotをアサルトにセット
	isShotGun = false;

	//フェード変数の初期化
	backGroundAlpha = 1.0f;
	faderate = DEFAULT_FADERATE;
	fadeOut = false;
	fadein = true;
	fadeinit = false;
	stageClearMove = false;
	gameOverMove = false;
	movetimer = 2.0f;
	mousePosX = 0;
	LastMousePosX = 0;
	StageFinishSE = true;
	hit = false;
	time = 0.1f;
	return true;
}

/**
* 入力反映.
*/
void MainGameScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();


	if (state == State::play && !fadein) {

		// プレイヤーを移動する.
		const float speed = DEFAULT_PLAYER_SPEED;
		player.velocity = glm::vec3(0);
		const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(CameraAngleX), glm::vec3(0, 1, 0));

		//FPSとTPSを切り替える **テスト実装**
		if (window.IsKeyDown(GLFW_KEY_M))
		{
			if (!window.IsKeyPressed(GLFW_KEY_SPACE) && !window.GetMouseButton(0))
			{
				fadein = true;
				faderate = 1;
				sMainBGM.SetVolume(0.0f);
				if (playmode == PlayMode::eFPS)
				{
					playmode = PlayMode::eTPS;
				}
				else if (playmode == PlayMode::eTPS)
				{
					playmode = PlayMode::eFPS;
				}
			}
		}

		//Shotの切り替え
		if (window.IsKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			//Shotボタンが押されていないときのみ切り替え
			if (!window.IsKeyPressed(GLFW_KEY_SPACE) && !window.GetMouseButton(0))
			{
				sChangeWeapon.Play();
				if (isShotGun)
				{
					isShotGun = false;
				}
				else
				{
					isShotGun = true;
				}
			}
		}

		//=======================Cameraの入力処理=================================
		//矢印キーの場合
		//左右の移動
		if (window.IsKeyPressed(GLFW_KEY_LEFT)) {
			CameraAngleX += CameraRotateSpeed * deltaTime;
		}
		else if (window.IsKeyPressed(GLFW_KEY_RIGHT)) {
			CameraAngleX -= CameraRotateSpeed * deltaTime;
		}
		//上下の移動
		if (window.IsKeyPressed(GLFW_KEY_UP)) {
			CameraAngleY += CameraRotateSpeed * 30 * deltaTime;
		}
		else if (window.IsKeyPressed(GLFW_KEY_DOWN)) {
			CameraAngleY -= CameraRotateSpeed * 30 * deltaTime;
		}


		//Mouseの場合
		mousePosX = window.GetMousePosition().x - (1440 / 2);
		//右に動かした場合
		if (mousePosX > LastMousePosX)
		{
			CameraAngleX -= CameraRotateSpeed * (mousePosX - LastMousePosX) * deltaTime;
		}
		//左に動かした場合
		else if (mousePosX < LastMousePosX)
		{
			CameraAngleX += CameraRotateSpeed * (LastMousePosX - mousePosX) * deltaTime;
		}

		LastMousePosX = mousePosX;
		////上下の移動
		//posy = static_cast<int>(window.GetMousePosition().y - 380);
		//if (lastposY < posy)
		//{
		//	CameraAngleY -= (posy - lastposY) * CameraRotateSpeed * 10 * deltaTime;
		//}
		//else if (lastposY > posy)
		//{
		//	CameraAngleY += (lastposY - posy) * CameraRotateSpeed * 10 * deltaTime;
		//}
		//lastposY = posy;

		/*
		* 方向転換
		* TPSモードならPlayerの向きはカメラに依存せず,入力された方向へ向く.
		* FPSモードならPlayerとカメラの向きは同期する
		*/
		if (playmode == PlayMode::eFPS)
		{
			player.rotation.y = glm::radians(CameraAngleX);
		}

		//=======================移動の入力処理===================================
		if (window.IsKeyPressed(GLFW_KEY_A)) {
			player.velocity = rotaPT * glm::vec4(-1, 0, 0, 1);
		}
		else if (window.IsKeyPressed(GLFW_KEY_D)) {
			player.velocity = rotaPT * glm::vec4(1, 0, 0, 1);
		}
		if (window.IsKeyPressed(GLFW_KEY_W)) {
			player.velocity = rotaPT * glm::vec4(0, 0, -1, 1);
		}
		else if (window.IsKeyPressed(GLFW_KEY_S)) {
			player.velocity = rotaPT * glm::vec4(0, 0, 1, 1);
		}
		if (player.velocity.x || player.velocity.z) {
			player.velocity = glm::normalize(player.velocity);
			player.velocity *= speed;
			// ショットボタンが押されていなければ
			if (!window.IsKeyPressed(GLFW_KEY_SPACE) && !window.GetMouseButton(0)) {

				//左シフトでダッシュ (移動速度を1.5倍)
				if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
				{
					player.velocity *= 1.5;
				}

				if (playmode == PlayMode::eTPS)
				{
					player.rotation.y = std::atan2(-player.velocity.z, player.velocity.x) - glm::radians(90.0f);
				}

			}
			//押されていれば移動速度を半減する
			else
			{
				player.velocity *= 0.5f;
			}
		}


		//================================武器の処理========================================

		//リロード処理
		if (window.IsKeyDown(GLFW_KEY_R) && (reloadtimer <= 0))
		{
			if (!isShotGun)
			{
				sALReload.Play();
				al_Remaining = DEFAULT_AL_REMAINING;
				reloadtimer = 1.3f;
			}
		}

		//アサルトライフルの処理
		if (!isShotGun)
		{
			if ((window.IsKeyPressed(GLFW_KEY_SPACE) || window.GetMouseButton(0)) && (reloadtimer <= 0)) {
				const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(CameraAngleX), glm::vec3(0, 1, 0));
				//発射タイマーが0秒 かつ 残弾数が0以上
				if ((playerBulletTimer <= 0) && (al_Remaining > 0)) {

					//一回目なら
					if (!isInput)
					{
						//鳴らして
						sAssaultSE.Play();
						//フラグをオフにする
						isInput = true;
					}
					//秒間12発出せる
					playerBulletTimer = 1.0f / 12.0f;
					//残弾数を1減らす
					al_Remaining -= 1;
					const glm::mat4 matRotY = glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
					Actor* bullet = FindAvailableActor(playerBulletList);

					//弾の発射処理本体
					if (bullet) {
						//bullet->Initialize(6, texBullet.Get(), 2, player.position + glm::vec3(matRotY * glm::vec4(0.25f, 1.0f, -0.125f, 1.0f)), player.rotation, glm::vec3(0.3f, 0.3f, 0.3f));
						bullet->Initialize(6, texBullet.Get(), 2, player.position + glm::vec3(matRotY * glm::vec4(0.125f, 1.3f, -0.0125f, -1.0f)), player.rotation, glm::vec3(0.3f, 0.3f, 0.3f));
						bullet->colLocal = { glm::vec3(-0.125f, -0.125f, -0.125f), glm::vec3(0.25f, 0.25f, 0.25f) };
						bullet->velocity = matRotY * glm::vec4(0, glm::radians(targetPos.y) * 1.12f, -200, 1);
					}
				}
				if (al_Remaining <= 0)
				{
					sAssaultSE.Stop(0.5f);
				}

			}
			else {
				playerBulletTimer = 0.0f;
				sAssaultSE.Stop(0.5f);
				isInput = false;
			}
		}
		//ショットガンの処理
		else
		{
			std::uniform_int_distribution<int> range(-3, 3);

			if (window.IsKeyPressed(GLFW_KEY_SPACE) || window.GetMouseButton(0) && (reloadtimer <= 0)) {
				//発射タイマーが0秒 かつ 残弾数が0以上
				if ((playerBulletTimer <= 0) && (sg_Remaining > 0))
				{
					//止めて
					sShotGunSE.Stop();
					//鳴らして
					sShotGunSE.Play();
					//1.22秒に1回発射できる
					playerBulletTimer = 1.22f;
					//残弾数を1減らす
					sg_Remaining -= 1;
					const glm::mat4 matRotY = glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
					for (int i = 1; i < 10; ++i)
					{
						//空きアクターを取得
						Actor* bullet = FindAvailableActor(playerBulletList);

						//空きアクターがあれば
						if (bullet)
						{
							//初期化
							//bullet->Initialize(6, texBullet.Get(), 3, player.position + glm::vec3(matRotY * glm::vec4(0.25f, 1.0f, -0.0625f, 1.0f)), player.rotation, glm::vec3(0.2f));
							bullet->Initialize(6, texBullet.Get(), 3, player.position + glm::vec3(matRotY * glm::vec4(0.001f, 1.2f, -0.001f, 0.01f)), player.rotation, glm::vec3(0.2f));
							//コリジョンの設定
							bullet->colLocal = { glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.5, 0.5, 0.5) };
							//velocityの設定
							bullet->velocity = matRotY * glm::vec4(i * range(random), i * range(random), -150, 1);
						}
					}
				}
			}
		}
	}
	else if (state == State::stageClear && !stageClearMove) {
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyDown(GLFW_KEY_ENTER) || window.GetMouseButton(1)) {
			sClearSE.Stop(1.0f);
			fadeOut = true;
			++stageNo;
			enemyTotal = 10 + stageNo * 10;
			enemyLeft = enemyTotal;
			enemyKilled = 0;
			enemySpeed = 0.5f + (float)(stageNo - 1) * 0.1f;
			enemyPoppingInterval = 15.0f - (float)(stageNo - 1) * 1.0f;
			enemyPoppingTimer = 3.0f;
			stageClearMove = true;
			StageFinishSE = true;
		}
	}
	else if (state == State::gameOver && !stageClearMove) {
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyDown(GLFW_KEY_ENTER) || window.GetMouseButton(1)) {
			//==================-フェード処理 シーン移動==========================
			fadeOut = true;
			movetimer = 2;
			gameOverMove = true;
			StageFinishSE = true;
			sGameOverBGM.Stop(movetimer);

		}
	}


	//Debug機能
	if (DEBUGMODE)
	{

		//移動速度を上昇させる
		if (window.IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
		{
			player.velocity *= 10;
		}
		//Ctrl & Alt キー系
		if (window.IsKeyPressed(GLFW_KEY_LEFT_ALT) && window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			//デバッグモードを切る
			if (window.IsKeyDown(GLFW_KEY_D))
			{
				DEBUGMODE = false;
			}
			//Ctrl + F [フェード機能] フェード中でなければ
			if (window.IsKeyPressed(GLFW_KEY_F) && (!fadein && !fadeOut))
			{
				//Ctrl + F + I [フェードイン]
				if (window.IsKeyDown(GLFW_KEY_I))
				{
					fadein = true;
					faderate = 1;
				}
				//Ctrl + F + O [フェードアウト]
				else if (window.IsKeyDown(GLFW_KEY_O))
				{
					fadeOut = true;
				}
			}
			//シーン遷移
			if (window.IsKeyPressed(GLFW_KEY_G))
			{
				//ゲームクリア
				if (window.IsKeyDown(GLFW_KEY_C))
				{
					DEBUGMODE = false;
					state = State::stageClear;
				}
				if (window.IsKeyDown(GLFW_KEY_O))
				{
					state = State::gameOver;
				}
			}
			//テスト用3D音源
			if (window.IsKeyDown(GLFW_KEY_1))
			{
				SpecialBGM.Play(5.0f);
			}
			if (window.IsKeyDown(GLFW_KEY_2))
			{
				SpecialBGM.Stop(5.0f);
			}
			//音関係
			if (window.IsKeyDown(GLFW_KEY_S))
			{
				sMainBGM.Stop(5.0f);
			}
			if (window.IsKeyDown(GLFW_KEY_P))
			{
				sMainBGM.Play(5.0f);
			}
			if (window.IsKeyDown(GLFW_KEY_3))
			{
				SpecialBGM2.Play(5.0f);
			}
			if (window.IsKeyDown(GLFW_KEY_4))
			{
				SpecialBGM2.Stop(2.0f);
			}
			if (window.IsKeyDown(GLFW_KEY_5))
			{
				SpecialBGM3.Play();
			}
			if (window.IsKeyDown(GLFW_KEY_6))
			{
				SpecialBGM3.Stop(5.0f);
			}
			//全てのZombieのHPを減らす
			if (window.IsKeyDown(GLFW_KEY_K))
			{
				for (auto& actor : enemyList)
				{
					actor->Actor_HP = 0;
				}
			}

		}
	}
	else
	{
		//左Alt + 左Ctrl キー系
		if (window.IsKeyPressed(GLFW_KEY_LEFT_ALT) && window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			//左Alt + 左Ctrl + D
			if (window.IsKeyDown(GLFW_KEY_D))
			{
				//デバッグモードを起動する
				DEBUGMODE = true;
			}
		}
	}
}

/**
* 状態更新.
*/
void MainGameScene::Update()
{

	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

	//フェード中でなければ更新する
	if (!fadein && !fadeOut)
	{
		// プレイヤーの更新.
		player.Update(deltaTime);
		// 自機ショットの更新.
		UpdateActorList(playerBulletList, deltaTime);
		// ゾンビの更新.
		UpdateActorList(enemyList, deltaTime);
	};

	//playerと家の当たり判定
	for (auto& object : houseList) {
		if (object->Actor_HP > 0)
		{
			if (DetectCollision(player, *object))
			{
				const CollisionTime t = FindCollisionTime(*object, player, deltaTime);
				if (t.plane != CollisionPlane::none)
				{
					const float time = deltaTime * t.time - 0.00001f;
					player.position += player.velocity * time;

					if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX)
					{
						player.velocity.x = 0;
					}
					else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY)
					{
						player.velocity.y = 0;
					}
					else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ)
					{
						player.velocity.z = 0;
					}

					player.velocity *= -t.time;
					player.position += player.velocity * -time;
				}
			}
		}
	}
	//playerと木の当たり判定
	for (auto& object : treeList) {
		if (object->Actor_HP > 0)
		{
			if (DetectCollision(player, *object))
			{
				const CollisionTime t = FindCollisionTime(*object, player, deltaTime);
				if (t.plane != CollisionPlane::none)
				{
					const float time = deltaTime * t.time - 0.00001f;
					player.position += player.velocity * time;

					if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX)
					{
						player.velocity.x = 0;
					}
					else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY)
					{
						player.velocity.y = 0;
					}
					else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ)
					{
						player.velocity.z = 0;
					}

					player.velocity *= -t.time;
					player.position += player.velocity * -time;
				}
			}
		}
	}
	//playerと岩の当たり判定
	//for (auto& object : rockList) {
	//	if (object->Actor_HP > 0)
	//	{
	//		if (DetectCollision(player, *object))
	//		{
	//			const CollisionTime t = FindCollisionTime(*object, player, deltaTime);
	//			if (t.plane != CollisionPlane::none)
	//			{
	//				const float time = deltaTime * t.time - 0.00001f;
	//				player.position += player.velocity * time;

	//				if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX)
	//				{
	//					player.velocity.x = 0;
	//				}
	//				else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY)
	//				{
	//					player.velocity.y = 0;
	//				}
	//				else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ)
	//				{
	//					player.velocity.z = 0;
	//				}

	//				player.velocity *= -t.time;
	//				player.position += player.velocity * -time;
	//			}
	//		}
	//	}
	//}
	//Playerの行動範囲制限
	for (int i = 0; i < 3; ++i)
	{
		if (player.position[i] < -200.0f)
		{
			player.position[i] = -200.0f;
		}
		if (player.position[i] > 200.0f)
		{
			player.position[i] = 200.0f;
		}
	}

	//reloadtimerを減らす
	if (reloadtimer >= 0)
	{
		reloadtimer -= deltaTime;
	}

	//リスナーのポジションをセット
	Sound::ListenerPostion(player.position);
	//向きベクトルを取得
	glm::vec3 dirNormal = glm::normalize(targetPos - vPos);
	//向きベクトルをリスナーへセット
	Sound::ListenerDirection(dirNormal.x, dirNormal.y, dirNormal.z);

	//デバッグモードでなければゾンビを出す
	if (!DEBUGMODE)
	{
		// ゾンビの発生.
		//Enemyの残り数が0以上なら
		if (enemyLeft > 0 && !fadein) {

			//Enemyの出現タイマーを減らす
			if (enemyPoppingTimer >= 0) {
				enemyPoppingTimer -= deltaTime;
				//Timerが0以下になったらインターバル分 タイマーを加算する
			}
			else {
				enemyPoppingTimer += enemyPoppingInterval;


				const int maxPopCount = 10;
				const int popCount = std::min(enemyLeft, maxPopCount);
				enemyLeft -= popCount;

				//ランダムの範囲をここで設定する
				std::uniform_int_distribution<int> rangeBaseX(player.position.x - 30, player.position.x + 30);
				std::uniform_int_distribution<int> rangeBaseZ(player.position.z - 30, player.position.z + 30);
				std::uniform_int_distribution<int> range(-15, 15);
				std::uniform_int_distribution<int> hpRange(10, 30);
				std::uniform_int_distribution<int> speedRange(4, 12);
				std::uniform_int_distribution<int> scaleRange(12, 16);

				//生成されるベースポジションを決定する
				glm::vec3 posBase(rangeBaseX(random), 0, rangeBaseZ(random));

				//出現位置を知らせるためのSEを鳴らす
				sZombiePopSE.SetPosition(posBase);
				sZombiePopSE.Play();

				for (int i = 0; i < popCount; ++i) {
					glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));


					//Enemyの生成範囲制限
					for (int i = 0; i < 3; ++i)
					{
						if (pos[i] < -200.0f)
						{
							pos[i] += 50.0f;
						}
						if (pos[i] > 200.0f)
						{
							pos[i] -= 50.0f;
						}
					}

					ZombieActor* zombie = (ZombieActor*)FindAvailableActor(enemyList);
					glm::vec3 popScale = glm::vec3(scaleRange(random) * 0.1, scaleRange(random) * 0.1 + 0.2, scaleRange(random) * 0.1);
					if (zombie) {
						//===================エネミーのModelさしかえ　４番から作ったリストNumberに===========
						zombie->Initialize(4, texZombie.Get(), hpRange(random), pos, glm::vec3(0), glm::vec3(popScale));
						zombie->colLocal = { glm::vec3(-popScale.x * 0.5, 0, -popScale.z * 0.5), glm::vec3(popScale) };
						zombie->target = &player;
						zombie->baseSpeed = (enemySpeed * speedRange(random)) * 0.1f;
					}
				}
			}
		}
	}



	//Enemyの状態を更新
	for (auto& actor : enemyList) {
		//HPが0以下なら次のアクターを処理
		if (actor->Actor_HP <= 0) {
			continue;
		}
		//0以上のとき
		ZombieActor* zombie = (ZombieActor*)actor;
		//ゾンビのターゲットがHP0なら処理を終了する
		if (zombie->target->Actor_HP <= 0) {
			continue;
		}

		//アタックフラグがtrueなら
		if (zombie->isAttacking) {
			const glm::vec3 vFront = glm::rotate(glm::mat4(1), zombie->rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 0);
			const glm::vec3 vTarget = zombie->target->position - zombie->position;
			const float angle = std::acos(glm::dot(vFront, vTarget));
			//十分に距離が近く、正面にターゲットがいれば攻撃.
			if (std::abs(angle) < glm::radians(45.0f) && glm::length(vTarget) < 1.5f) {
				--zombie->target->Actor_HP;
				sZombiePopSE.SetPosition(zombie->position);
				sZombiePopSE.Play();
				hit = true;
			}


		}
		//Enemyの行動範囲制限
		for (int i = 0; i < 3; ++i)
		{
			if (zombie->position[i] < -200.0f)
			{
				zombie->position[i] = -200.0f;
			}
			if (zombie->position[i] > 200.0f)
			{
				zombie->position[i] = 200.0f;
			}
		}
	}

	//マップ外ならHP0にする
	for (auto& actor : houseList)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (actor->position[i] < -200.0f)
			{
				actor->Actor_HP = 0;
			}
			if (actor->position[i] > 200.0f)
			{
				actor->Actor_HP = 0;
			}
		}
	}
	for (auto& actor : treeList)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (actor->position[i] < -200.0f)
			{
				actor->Actor_HP = 0;
			}
			if (actor->position[i] > 200.0f)
			{
				actor->Actor_HP = 0;
			}
		}
	}
	/*for (auto& actor : rockList)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (actor->position[i] < -200.0f)
			{
				actor->Actor_HP = 0;
			}
			if (actor->position[i] > 200.0f)
			{
				actor->Actor_HP = 0;
			}
		}
	}*/


	//===========================カメラの更新.====================================--
	const glm::vec3 viewOffset = glm::vec3(0.4f, 1.8f, 2.0f);
	viewPos = player.position + viewOffset;



	// プレイヤーの弾の発射タイマーの更新.
	if (playerBulletTimer > 0) {
		playerBulletTimer -= deltaTime;
	}


	//=================================================================================================
	//=========================================衝突判定=================================================
	//=================================================================================================

	//===============================追加した衝突判定1======================================
	//敵同士の衝突判定
	DetectCollision(enemyList, enemyList, [&](Actor& enemy1, Actor& enemy2)
		{
			const CollisionTime t = FindCollisionTime(enemy1, enemy2, deltaTime);
			if (t.plane != CollisionPlane::none)
			{
				const float time = deltaTime * t.time - 0.00001f;
				enemy1.position += enemy1.velocity * time;

				if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX)
				{
					enemy1.velocity.x = -2.0f;
					enemy2.velocity.x = 2.0f;
				}
				else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY)
				{
					enemy1.velocity.y = -2.0f;
					enemy2.velocity.y = 2.0f;
				}
				else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ)
				{
					enemy1.velocity.z = -2.0f;
					enemy2.velocity.z = 2.0f;
				}

				enemy1.velocity *= -t.time;
				enemy1.position += enemy1.velocity * -time;
				enemy2.velocity *= -t.time;
				enemy2.position += enemy2.velocity * -time;
			}
		});

	//===============================追加した衝突判定2======================================
	//敵と家の衝突判定
	DetectCollision(enemyList, houseList, [&](Actor& enemy, Actor& object)
		{
			const CollisionTime t = FindCollisionTime(enemy, object, deltaTime);
			if (t.plane != CollisionPlane::none)
			{
				const float time = deltaTime * t.time - 0.00001f;
				enemy.position += enemy.velocity * time;

				if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX)
				{
					enemy.velocity.x = 0.0f;
				}
				else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY)
				{
					enemy.velocity.y = 0.0f;
				}
				else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ)
				{
					enemy.velocity.z = 0.0f;
				}

				enemy.velocity *= -t.time;
				enemy.position += enemy.velocity * -time;
			}
		});

	//===============================追加した衝突判定3======================================
	//敵と木の衝突判定
	DetectCollision(enemyList, treeList, [&](Actor& enemy, Actor& object)
		{
			const CollisionTime t = FindCollisionTime(enemy, object, deltaTime);
			if (t.plane != CollisionPlane::none)
			{
				const float time = deltaTime * t.time - 0.00001f;
				enemy.position += enemy.velocity * time;

				if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX)
				{
					enemy.velocity.x = 0.0f;
				}
				else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY)
				{
					enemy.velocity.y = 0.0f;
				}
				else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ)
				{
					enemy.velocity.z = 0.0f;
				}

				enemy.velocity *= -t.time;
				enemy.position += enemy.velocity * -time;
			}
		});

	//===============================追加した衝突判定4======================================
	//敵と木の衝突判定
	/*DetectCollision(enemyList, rockList, [&](Actor& enemy, Actor& object)
	{
		const CollisionTime t = FindCollisionTime(enemy, object, deltaTime);
		if (t.plane != CollisionPlane::none)
		{
			const float time = deltaTime * t.time - 0.00001f;
			enemy.position += enemy.velocity * time;

			if (t.plane == CollisionPlane::negativeX || t.plane == CollisionPlane::positiveX)
			{
				enemy.velocity.x = 0.0f;
			}
			else if (t.plane == CollisionPlane::negativeY || t.plane == CollisionPlane::positiveY)
			{
				enemy.velocity.y = 0.0f;
			}
			else if (t.plane == CollisionPlane::negativeZ || t.plane == CollisionPlane::positiveZ)
			{
				enemy.velocity.z = 0.0f;
			}

			enemy.velocity *= -t.time;
			enemy.position += enemy.velocity * -time;
		}
	});*/

	// プレイヤーの弾と敵の衝突判定.
	DetectCollision(playerBulletList, enemyList, [&](Actor& bullet, Actor& zombie) {
		const CollisionTime t = FindCollisionTime(bullet, zombie, deltaTime);
		if (t.time) {
			zombie.Actor_HP -= bullet.Actor_HP;
			bullet.Actor_HP = 0;
			//===================-着弾SEを鳴らす=======================
			sHitEnemy.SetPosition(zombie.position);
			sHitEnemy.Play();

			if (zombie.Actor_HP <= 0) {
				//==========================死亡Seを鳴らす============================
				sZombieDeadSE.SetPosition(zombie.position);
				sZombieDeadSE.Play();
				score += 200;
				++enemyKilled;
			}
			else {
				score += 50;
			}
		}
		});

	//===============================追加した衝突判定5======================================
	//弾とオブジェクトの衝突判定
	DetectCollision(playerBulletList, houseList, [&](Actor& bullet, Actor& object) {
		//弾とオブジェクトが当たるまでの時間を調べる
		const CollisionTime t = FindCollisionTime(bullet, object, deltaTime);
		//衝突したら
		if (t.time)
		{
			//=========================着弾SE================================
			sHitObject.SetPosition(bullet.position);
			sHitObject.Play();
			//弾だけHPを0にする
			bullet.Actor_HP = 0;
		}
		});


	// 敵をすべて倒したらステージクリア.
	if (enemyKilled == enemyTotal) {
		state = State::stageClear;
		sAssaultSE.Stop();
		if (StageFinishSE)
		{
			StageFinishSE = false;
			sClearSE.Play();
		}
	}


	// プレイヤーの体力が0以下になったらゲームオーバー.
	if (player.Actor_HP <= 0) {
		state = State::gameOver;
		sAssaultSE.Stop();
		sMainBGM.Stop(3.0f);
		if (StageFinishSE)
		{
			StageFinishSE = false;
			sGameOverBGM.Play(10);
		}
	}
	if (hit) {
		time -= deltaTime;
		if (time < 0.0f)
		{
			hit = false;
		}
	}
	else {
		time = 0.1f;
	}
	//===================================================================
	//=========================フェード処理===============================
	//===================================================================

	if (fadein || fadeOut)
	{
		//もしfaderateの初期化がまだなら
		if (!fadeinit)
		{
			//inのときはDEFAULTに
			if (fadein)
			{
				faderate = DEFAULT_FADERATE;
				soundfade = 0.0f;
			}
			//outのときは０に
			else
			{
				faderate = 0.0f;
				soundfade = 1.0f;
			}
			fadeinit = true;
		}



		//Fade IN 処理
		if (fadein)
		{
			if (faderate > 0.0f)
			{
				//2秒でin
				faderate -= deltaTime * 0.5f;
				soundfade += deltaTime * 0.5f;
				sMainBGM.SetVolume(soundfade);
			}
			else
			{
				fadein = false;
				faderate = 0.0f;
				sMainBGM.SetVolume(1.0f);
			}
		}
		if (fadeOut)
		{
			if (faderate < 1.0f)
			{
				//2秒でout
				faderate += deltaTime * 0.5f;
				soundfade -= deltaTime * 0.5f;
				sMainBGM.SetVolume(soundfade);
			}
			else
			{
				fadeOut = false;
				faderate = 1.0f;
				sMainBGM.SetVolume(0.0f);
			}
		}
	}

	//StageClear処理
	if (stageClearMove)
	{
		movetimer -= deltaTime;
		if (movetimer <= 0.0f)
		{
			movetimer = 2.0f;
			stageClearMove = false;
			fadein = true;
			CameraAngleX = 0.0f;
			player.position = glm::vec3(8, 0, 8);
			player.rotation = glm::vec3(0, 0, 0);
			enemyPoppingTimer = 4.0f;
			state = State::play;
			al_Remaining = DEFAULT_AL_REMAINING;
			sg_Remaining += DEFAULT_SG_REMAINING + stageNo;
		}
	}
	//GameOver処理
	if (gameOverMove)
	{
		movetimer -= deltaTime;

		if (movetimer <= 0.0f)
		{
			NextScene("Title");
		}
	}

	if (DEBUGMODE)
	{
		SpecialBGM2.SetPosition(player.position.x, 2, player.position.z + 5);
		SpecialBGM.SetVelocity(-player.velocity.x * 0.5, 0, -player.velocity.z * 0.5);
	}

}

/**
* 描画.
*/
void MainGameScene::Render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.3f, 0.01f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//=======================================座標変換行列を作成する.=========================================
	/*
	* パースペクティブ(3次元投影)の行列
	* @param fovy(Field of View) 視野角
	* @param aspect 画面幅比率
	* @param near 視点とスクリーンの距離
	* @param far 後方面の距離(何かわからない)
	*/
	const glm::mat4x4 matProj = glm::perspective(glm::radians(55.0f), 16.0f / 9.0f, 0.1f, 500.0f);

	//ローカル座標の回転後ポジションを取得するための行列計算
	const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(CameraAngleX), glm::vec3(0, 1, 0));
	//const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(player.rotation.y), glm::vec3(0, 1, 0));
	//カメラの焦点座標
	targetPos = rotaPT * glm::vec4(0, glm::radians(CameraAngleY), -(P_T_Distance), 1);
	/*
	*	@param viewOffset カメラのローカル座標
	*	@mode TPS	デフォルトは	DEFAULT_CAM_OFFSET_TPS
	*	@mode FPS	デフォルトは	DEFAULT_CAM_OFFSET_FPS
	*/
	glm::vec3 viewOffset;
	if (playmode == PlayMode::eTPS)
	{
		viewOffset = DEFAULT_CAM_OFFSET_TPS;
	}
	else if (playmode == PlayMode::eFPS)
	{
		viewOffset = DEFAULT_CAM_OFFSET_FPS;
	}
	//カメラの座標を ローカル座標 から ワールド座標 へ変換する
	vPos = rotaPT * glm::vec4(viewOffset, 1);
	//View座標計算行列
	const glm::mat4x4 matView = glm::lookAt(player.position + vPos, player.position + targetPos, glm::vec3(0, 1, 0));




	progLighting.SetViewProjectionMatrix(matProj * matView);
	progSimple.SetViewProjectionMatrix(matProj * matView);

	progLighting.Use();

	meshList.BindVertexArray();

	// 光源を設定する.
	progLighting.SetLightList(lights);

	progLighting.BindTexture(0, player.texture);
	progLighting.Draw(meshList[player.mesh], player.position, player.rotation, player.scale);

	RenderActorList(objectList, progLighting, meshList);
	RenderActorList(enemyList, progLighting, meshList);
	RenderActorList(playerBulletList, progLighting, meshList);
	RenderActorList(treeList, progLighting, meshList);
	RenderActorList(houseList, progLighting, meshList);
	RenderActorList(rockList, progLighting, meshList);

	progSimple.Use();
	progSimple.BindTexture(0, texGround.Get());



	{
		glDisable(GL_DEPTH_TEST);

		const glm::mat4x4 matProj = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, 1.0f, 500.0f);
		const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		progSimple.SetViewProjectionMatrix(matProj * matView);

		// ステージ数表示.
		progSimple.BindTexture(0, texDay.Get());
		progSimple.Draw(meshList[5], glm::vec3(-486, -300 + 64 + 24 + 4, 0), glm::vec3(0), glm::vec3(256 / 4, 128 / 4, 1.5));


		int tmpStageNo = stageNo;

		if (tmpStageNo > 999) {
			tmpStageNo = 999;
		}

		for (int i = 0; i < 3; ++i) {
			const int number = tmpStageNo % 10;
			progSimple.BindTexture(0, texNumber[number].Get());
			progSimple.Draw(meshList[5], glm::vec3(-450 + 32 * (3 - i), -300 + 64 + 24 + 4, 0), glm::vec3(0), glm::vec3(32, 32, 1.2));
			tmpStageNo /= 10;
		}

		int tmpRE;
		//残弾数
		if (isShotGun)
		{
			tmpRE = sg_Remaining;
		}
		else if (!isShotGun)
		{
			tmpRE = al_Remaining;
		}

		if (tmpRE < 0)
		{
			tmpRE = 0;
		}
		else if (tmpRE > 99)
		{
			tmpRE = 99;
		}
		for (int i = 0; i < 2; ++i)
		{
			const int number = tmpRE % 10;
			progSimple.BindTexture(0, texNumber[number].Get());

			progSimple.Draw(meshList[5], glm::vec3(450 + 32 * (2 - i), -300 + 24 + 4, 0), glm::vec3(0), glm::vec3(32, 32, 1.5));

			tmpRE /= 10;
		}

		// HP表示.
		progSimple.BindTexture(0, texHP.Get());

		progSimple.Draw(meshList[5], glm::vec3(-486, -300 + 24 + 4, 0), glm::vec3(0), glm::vec3(192 / 4, 128 / 4, 1.5));

		int tmpHealth = player.Actor_HP;

		if (tmpHealth < 0)
		{
			tmpHealth = 0;
		}

		else if (tmpHealth > 99)
		{
			tmpHealth = 99;
		}

		for (int i = 0; i < 2; ++i)
		{
			const int number = tmpHealth % 10;
			progSimple.BindTexture(0, texNumber[number].Get());

			progSimple.Draw(meshList[5], glm::vec3(-450 + 32 * (2 - i), -300 + 24 + 4, 0), glm::vec3(0), glm::vec3(32, 32, 1.5));

			tmpHealth /= 10;
		}

		// スコア表示.
		progSimple.BindTexture(0, texScore.Get());

		progSimple.Draw(meshList[5], glm::vec3(-120, 350 - 24 - 4, 0), glm::vec3(0), glm::vec3(384 / 4, 128 / 4, 1.5));

		int tmpScore = score;

		if (tmpScore > 99999999)
		{
			tmpScore = 99999999;
		}

		//数字をテクスチャから取得しさらに適用する
		for (int i = 0; i < 8; ++i)
		{
			const int number = tmpScore % 10;
			progSimple.BindTexture(0, texNumber[number].Get());

			progSimple.Draw(meshList[5], glm::vec3(-32 + 32 * (8 - i), 350 - 24 - 4, 0), glm::vec3(0), glm::vec3(32, 32, 1.5));

			tmpScore /= 10;
		}

		// ステージクリア・ゲームオーバー表示.
		if (state == State::stageClear)
		{
			progSimple.BindTexture(0, texStageClear.Get());
			progSimple.Draw(meshList[5], glm::vec3(0), glm::vec3(0), glm::vec3(350, 60, 1));
		}

		else if (state == State::gameOver)
		{
			progSimple.BindTexture(0, texGameOver.Get());
			progSimple.Draw(meshList[5], glm::vec3(0), glm::vec3(0), glm::vec3(300, 60, 1));
		}
		// フェードアウト用
		progSimple.BindTexture(0, texFadeSheet.Get());

		//もしフェードフラグがTrueだったら
		if ((fadein || fadeOut) || gameOverMove)
		{
			//フェード処理
			progSimple.Draw(meshList[5], glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1920, 1080, 10), backGroundAlpha * faderate);
		}


		// Effect用

		float alpha = 0.0f;
		progSimple.BindTexture(0, texEffectSheet.Get());
		if (hit) {
			alpha = 0.6f;
		}
		else if (!hit) {
			alpha = 0.0f;
		}
		progSimple.Draw(meshList[5], glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1920, 1080, 10), alpha);

	ClearActorList(houseList);
	ClearActorList(treeList);
	ClearActorList(rockList);
		//デバッグ用
		progSimple.BindTexture(0, texDebugImage.Get());
		//もしデバッグモードなら
		if (DEBUGMODE)
		{
			progSimple.Draw(meshList[5], glm::vec3(500, 200, 0), glm::vec3(0), glm::vec3(128, 128, 1), 0.8f);
		}

	}
}

/**
* 終了.
*/
void MainGameScene::Finalize()
{
	ClearActorList(playerBulletList);
	ClearActorList(enemyList);
	ClearActorList(objectList);
}

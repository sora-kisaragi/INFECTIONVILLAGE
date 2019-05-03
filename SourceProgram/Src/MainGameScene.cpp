/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#define DEFAULT_PLAYER_SPEED 10.0f

/**
* �v���C���[�̏�Ԃ��X�V����.
*
* @param deltaTime �o�ߎ���.
*/
void PlayerActor::Update(float deltaTime)
{
	//HP�� 0�ȏ� �Ȃ��
	if (Actor_HP <= 0) {
		//���ʂ֌����� (rad -45��)
		rotation.x -= glm::radians(45.0f) * deltaTime;

		//Player��rotation.x�� {rad -90��} �������������
		if (rotation.x < glm::radians(-90.0f)) {

			//���ʂ֌����� { (-45��) + (-90��) }
			rotation.x = glm::radians(-90.0f);
		}

	}

	Actor::Update(deltaTime);
}

/**
* �v���C���[�̒e�̏�Ԃ��X�V����.
*/
void BulletActor::Update(float deltaTime)
{
	position.y *= 0.98;
	for (int i = 0; i < 3; ++i) {
		//Position {�X�e�[�W�T�C�Y} �� {-200~200} �̊ԂȂ��
		if (position[i] < -200 || position[i] > 200) {
			//HP��0�ɂ���
			Actor_HP = 0;
		}//�n�ʂ�艺�ɂȂ�����
		if (position[1] < 0)
		{
			//HP��0�ɂ���
			Actor_HP = 0;
		}
	}
	Actor::Update(deltaTime);
}

/**
* �]���r�̏�Ԃ��X�V����.
*/
void ZombieActor::Update(float deltaTime)
{
	//����Player��Traget���ĂȂ�������
	if (!target) {
		//Update�I��
		return;
	}


	//�ړ����x��ݒ� {�x�[�X x2}
	const float moveSpeed = baseSpeed * 2.0f;
	//��]���x��ݒ� {�x�[�X x2}
	const float rotationSpeed = baseSpeed * glm::radians(150.0f);
	//�U���͈�?
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

	// �\���ɐڋ߂��Ă��Ȃ���Έړ�����. �ڋ߂��Ă���΍U������.
	if (glm::length(v) > 1.0f) {
		velocity = vZombieFront * moveSpeed;
	}
	else {
		velocity = glm::vec3(0);


		// ����I�ɍU����ԂɂȂ�.
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
* ������.
*/
bool MainGameScene::Initialize()
{
	random.seed(std::random_device()());

	//Sound�̏�����
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


	//Model�̓ǂݍ���
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

	//�e�N�X�`���̓K�p
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
	texDebugImage.Reset(Texture::LoadImage2D("Res/sorachan.tga"));

	// ���C�g�̐ݒ�.
	lights.ambient.color = glm::vec3(0.05f, 0.01f, 0.01f);
	//�Ԃ��ۂ��F�̕��͋C==========================================================================================
	lights.directional.direction = glm::normalize(glm::vec3(3, -8, -2));
	lights.directional.color = glm::vec3(1.2, 0.8, 0.8);


	pointLightAngle = 0;

	//Player��������
	player.Initialize(4, texHuman.Get(), 10, glm::vec3(8, 0, 8), glm::vec3(0), glm::vec3(1));

	//Player�̓����蔻��T�C�Y
	player.colLocal = { {-0.5f, 0, -0.5f }, {1, 1.7f, 1} };
	//Player�̒e�������T�C�Y
	playerBulletList.resize(256);

	//�e128�����
	for (auto& e : playerBulletList) {
		e = new BulletActor;
	}
	//enemy�����T�C�Y
	enemyList.resize(128);
	for (auto& e : enemyList) {
		e = new ZombieActor;
	}

	//�I�u�W�F�N�g�����T�C�Y
	objectList.resize(2);
	for (auto& e : objectList) {
		e = new Actor;
	}

	//�Ƃ̃��X�g�����T�C�Y
	houseList.resize(128);
	for (auto& e : houseList)
	{
		e = new Actor;
	}
	//�؂̃��X�g�����T�C�Y
	treeList.resize(256);
	for (auto& e : treeList)
	{
		e = new Actor;
	}
	//��̃��X�g�����T�C�Y
	rockList.resize(64);
	for (auto& e : rockList)
	{
		e - new Actor;
	}

	//�}�b�v�̍�����x�[�X�ɂ���
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

	//�Ɠ��m�̊Ԋu
	std::uniform_int_distribution<int> baseMoveRangeX(80.0f, 120.0f);
	std::uniform_int_distribution<int> baseMoveRangeZ(20.0f, 40.0f);

	//�Ƃ�rotation��scale
	std::uniform_int_distribution<int> houseRotationRange(0.0f, 360.0f);
	std::uniform_int_distribution<int> houseScaleRange(4.0f, 8.0f);

	//�؂�rotation��scale
	std::uniform_int_distribution<int> treeRotationRange(0.0f, 360.0f);
	std::uniform_int_distribution<int> treeScaleRangeXZ(0.7f, 1.2f);
	std::uniform_int_distribution<int> treeScaleRangeY(1.0f, 1.8f);

	//���rotation��scale
	std::uniform_int_distribution<int> rockRotationRange(0.0f, 360.0f);
	std::uniform_int_distribution<int> rockScaleRange(0.7f, 1.2f);

	//�I�u�W�F�N�g�̐���
	for (int i = 0; i < createHouse; ++i)
	{

		//========================�Ƃ𐶐�����===============================
		//�x�[�X�ɂȂ�point��ݒ�
		//���� 1 ~ 3��̍쐬�Ȃ� x�������炷
		if ((i % 4) != 0)
		{
			basepos.x += baseMoveRangeX(random);

		}
		//���� 5��ڂ̍쐬�Ȃ� z�������炷
		else if ((i % 4) == 0)
		{
			basepos.x = -195.0f;
			basepos.z += baseMoveRangeZ(random);
		}

		//�x�[�X��transform�𐶐�
		glm::vec3 houseSpawnPos = basepos + HouseBasePos;
		glm::vec3 houseSpawnRot = glm::vec3(0, houseRotationRange(random), 0);
		glm::vec3 houseSpawnSca = glm::vec3(houseScaleRange(random));

		//�Ƃ̐�������
		houseList[(size_t)i]->Initialize(1, texHouse.Get(), 1, houseSpawnPos, houseSpawnRot, houseSpawnSca * 0.1f);
		houseList[(size_t)i]->colLocal = { {-(houseSpawnSca.x * 0.5), 0, -(houseSpawnSca.x * 0.5) }, { houseSpawnSca} };
		houseList[(size_t)i]->Update(0);


		//========================�؂𐶐�����=====================
		for (int i = 0; i < 2; ++i)
		{

			//�x�[�X��transform���Z�b�g
			glm::vec3 treeSpawnPos = houseSpawnPos;
			glm::vec3 treeSpawnRot = glm::vec3(0, treeRotationRange(random), 0);
			glm::vec3 treeSpawnSca = glm::vec3(treeScaleRangeXZ(random));
			treeSpawnSca.y = treeScaleRangeY(random);
			//�����񐔂ɂ����position�̕ύX
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
			//��������
			treeList[(size_t)nowtree]->Initialize(0, texTree.Get(), 1, treeSpawnPos, treeSpawnRot, treeSpawnSca);
			treeList[(size_t)nowtree]->colLocal = { {-(treeSpawnSca.x * 0.5), 0, -(treeSpawnSca.z * 0.5) }, { treeSpawnSca } };
			treeList[(size_t)nowtree]->Update(0);
			nowtree += 1;
		};

		//if ((i % 2) == 0)
		//{
		//	//�x�[�X��transform���Z�b�g
		//	glm::vec3 rockSpawnPos = houseSpawnPos;
		//	glm::vec3 rockSpawnRot = glm::vec3(0, rockRotationRange(random), 0);
		//	glm::vec3 rockSpawnSca = glm::vec3(rockScaleRange(random), rockScaleRange(random), rockScaleRange(random));

		//	//�����񐔂ɂ����position�̕ύX
		//	if ((i % 2) == 0)
		//	{
		//		rockSpawnPos += rockBasePos1;
		//	}
		//	else if ((i % 2) == 1)
		//	{
		//		rockSpawnPos += rockBasePos2;
		//	}


		//	//��������
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

	//PlayMode��FPS�ɐݒ�
	playmode = PlayMode::eFPS;

	//Shot���A�T���g�ɃZ�b�g
	isShotGun = false;

	//�t�F�[�h�ϐ��̏�����
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
	return true;
}

/**
* ���͔��f.
*/
void MainGameScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();


	if (state == State::play && !fadein) {

		// �v���C���[���ړ�����.
		const float speed = DEFAULT_PLAYER_SPEED;
		player.velocity = glm::vec3(0);
		const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(CameraAngleX), glm::vec3(0, 1, 0));

		//FPS��TPS��؂�ւ��� **�e�X�g����**
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

		//Shot�̐؂�ւ�
		if (window.IsKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			//Shot�{�^����������Ă��Ȃ��Ƃ��̂ݐ؂�ւ�
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

		//=======================Camera�̓��͏���=================================
		//���L�[�̏ꍇ
		//���E�̈ړ�
		if (window.IsKeyPressed(GLFW_KEY_LEFT)) {
			CameraAngleX += CameraRotateSpeed * deltaTime;
		}
		else if (window.IsKeyPressed(GLFW_KEY_RIGHT)) {
			CameraAngleX -= CameraRotateSpeed * deltaTime;
		}
		//�㉺�̈ړ�
		if (window.IsKeyPressed(GLFW_KEY_UP)) {
			CameraAngleY += CameraRotateSpeed * 30 * deltaTime;
		}
		else if (window.IsKeyPressed(GLFW_KEY_DOWN)) {
			CameraAngleY -= CameraRotateSpeed * 30 * deltaTime;
		}


		//Mouse�̏ꍇ
		if (window.GetMouseButton(1))
		{
			mousePosX = window.GetMousePosition().x - 1440 / 2;
			//�E�ɓ��������ꍇ
			if (mousePosX > LastMousePosX)
			{
				CameraAngleX -= CameraRotateSpeed * (mousePosX - LastMousePosX) * deltaTime;
			}
			//���ɓ��������ꍇ
			else if (mousePosX < LastMousePosX)
			{
				CameraAngleX += CameraRotateSpeed * (LastMousePosX - mousePosX) * deltaTime;
			}

			LastMousePosX = mousePosX;
			//�㉺�̈ړ�
			posy = static_cast<int>(window.GetMousePosition().y - 380);
			if (lastposY < posy)
			{
				CameraAngleY -= (posy - lastposY) * CameraRotateSpeed * 10 * deltaTime;
			}
			else if (lastposY > posy)
			{
				CameraAngleY += (lastposY - posy) * CameraRotateSpeed * 10 * deltaTime;
			}
			lastposY = posy;
		}

		/*
		* �����]��
		* TPS���[�h�Ȃ�Player�̌����̓J�����Ɉˑ�����,���͂��ꂽ�����֌���.
		* FPS���[�h�Ȃ�Player�ƃJ�����̌����͓�������
		*/
		if (playmode == PlayMode::eFPS)
		{
			player.rotation.y = glm::radians(CameraAngleX);
		}

		//=======================�ړ��̓��͏���===================================
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
			// �V���b�g�{�^����������Ă��Ȃ����
			if (!window.IsKeyPressed(GLFW_KEY_SPACE) && !window.GetMouseButton(0)) {

				//���V�t�g�Ń_�b�V�� (�ړ����x��1.5�{)
				if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
				{
					player.velocity *= 1.5;
				}

				if (playmode == PlayMode::eTPS)
				{
					player.rotation.y = std::atan2(-player.velocity.z, player.velocity.x) - glm::radians(90.0f);
				}

			}
			//������Ă���Έړ����x�𔼌�����
			else
			{
				player.velocity *= 0.5f;
			}
		}


		//================================����̏���========================================

		//�����[�h����
		if (window.IsKeyDown(GLFW_KEY_R) && (reloadtimer <= 0))
		{
			if (!isShotGun)
			{
				sALReload.Play();
				al_Remaining = DEFAULT_AL_REMAINING;
				reloadtimer = 1.3f;
			}
		}

		//�A�T���g���C�t���̏���
		if (!isShotGun)
		{
			if ((window.IsKeyPressed(GLFW_KEY_SPACE) || window.GetMouseButton(0)) && (reloadtimer <= 0)) {
				const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(CameraAngleX), glm::vec3(0, 1, 0));
				//���˃^�C�}�[��0�b ���� �c�e����0�ȏ�
				if ((playerBulletTimer <= 0) && (al_Remaining > 0)) {

					//���ڂȂ�
					if (!isInput)
					{
						//�炵��
						sAssaultSE.Play();
						//�t���O���I�t�ɂ���
						isInput = true;
					}
					//�b��12���o����
					playerBulletTimer = 1.0f / 12.0f;
					//�c�e����1���炷
					al_Remaining -= 1;
					const glm::mat4 matRotY = glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
					Actor* bullet = FindAvailableActor(playerBulletList);

					//�e�̔��ˏ����{��
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
		//�V���b�g�K���̏���
		else
		{
			std::uniform_int_distribution<int> range(-3, 3);

			if (window.IsKeyPressed(GLFW_KEY_SPACE) || window.GetMouseButton(0) && (reloadtimer <= 0)) {
				//���˃^�C�}�[��0�b ���� �c�e����0�ȏ�
				if ((playerBulletTimer <= 0) && (sg_Remaining > 0))
				{
					//�~�߂�
					sShotGunSE.Stop();
					//�炵��
					sShotGunSE.Play();
					//1.22�b��1�񔭎˂ł���
					playerBulletTimer = 1.22f;
					//�c�e����1���炷
					sg_Remaining -= 1;
					const glm::mat4 matRotY = glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
					for (int i = 1; i < 10; ++i)
					{
						//�󂫃A�N�^�[���擾
						Actor* bullet = FindAvailableActor(playerBulletList);

						//�󂫃A�N�^�[�������
						if (bullet)
						{
							//������
							//bullet->Initialize(6, texBullet.Get(), 3, player.position + glm::vec3(matRotY * glm::vec4(0.25f, 1.0f, -0.0625f, 1.0f)), player.rotation, glm::vec3(0.2f));
							bullet->Initialize(6, texBullet.Get(), 3, player.position + glm::vec3(matRotY * glm::vec4(0.001f, 1.2f, -0.001f, 0.01f)), player.rotation, glm::vec3(0.2f));
							//�R���W�����̐ݒ�
							bullet->colLocal = { glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.5, 0.5, 0.5) };
							//velocity�̐ݒ�
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
			//==================-�t�F�[�h���� �V�[���ړ�==========================
			fadeOut = true;
			movetimer = 2;
			gameOverMove = true;
			StageFinishSE = true;
			sGameOverBGM.Stop(movetimer);

		}
	}


	//Debug�@�\
	if (DEBUGMODE)
	{

		//�ړ����x���㏸������
		if (window.IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
		{
			player.velocity *= 10;
		}
		//Ctrl & Alt �L�[�n
		if (window.IsKeyPressed(GLFW_KEY_LEFT_ALT) && window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			//�f�o�b�O���[�h��؂�
			if (window.IsKeyDown(GLFW_KEY_D))
			{
				DEBUGMODE = false;
			}
			//Ctrl + F [�t�F�[�h�@�\] �t�F�[�h���łȂ����
			if (window.IsKeyPressed(GLFW_KEY_F) && (!fadein && !fadeOut))
			{
				//Ctrl + F + I [�t�F�[�h�C��]
				if (window.IsKeyDown(GLFW_KEY_I))
				{
					fadein = true;
					faderate = 1;
				}
				//Ctrl + F + O [�t�F�[�h�A�E�g]
				else if (window.IsKeyDown(GLFW_KEY_O))
				{
					fadeOut = true;
				}
			}
			//�V�[���J��
			if (window.IsKeyPressed(GLFW_KEY_G))
			{
				//�Q�[���N���A
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
			//�e�X�g�p3D����
			if (window.IsKeyDown(GLFW_KEY_1))
			{
				SpecialBGM.Play(5.0f);
			}
			if (window.IsKeyDown(GLFW_KEY_2))
			{
				SpecialBGM.Stop(5.0f);
			}
			//���֌W
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
			//�S�Ă�Zombie��HP�����炷
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
		//��Alt + ��Ctrl �L�[�n
		if (window.IsKeyPressed(GLFW_KEY_LEFT_ALT) && window.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			//��Alt + ��Ctrl + D
			if (window.IsKeyDown(GLFW_KEY_D))
			{
				//�f�o�b�O���[�h���N������
				DEBUGMODE = true;
			}
		}
	}
}

/**
* ��ԍX�V.
*/
void MainGameScene::Update()
{

	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

	//�t�F�[�h���łȂ���΍X�V����
	if (!fadein && !fadeOut)
	{
		// �v���C���[�̍X�V.
		player.Update(deltaTime);
		// ���@�V���b�g�̍X�V.
		UpdateActorList(playerBulletList, deltaTime);
		// �]���r�̍X�V.
		UpdateActorList(enemyList, deltaTime);
	};

	//player�ƉƂ̓����蔻��
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
	//player�Ɩ؂̓����蔻��
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
	//player�Ɗ�̓����蔻��
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
	//Player�̍s���͈͐���
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

	//reloadtimer�����炷
	if (reloadtimer >= 0)
	{
		reloadtimer -= deltaTime;
	}

	//���X�i�[�̃|�W�V�������Z�b�g
	Sound::ListenerPostion(player.position);
	//�����x�N�g�����擾
	glm::vec3 dirNormal = glm::normalize(targetPos - vPos);
	//�����x�N�g�������X�i�[�փZ�b�g
	Sound::ListenerDirection(dirNormal.x, dirNormal.y, dirNormal.z);

	//�f�o�b�O���[�h�łȂ���΃]���r���o��
	if (!DEBUGMODE)
	{
		// �]���r�̔���.
		//Enemy�̎c�萔��0�ȏ�Ȃ�
		if (enemyLeft > 0 && !fadein) {

			//Enemy�̏o���^�C�}�[�����炷
			if (enemyPoppingTimer >= 0) {
				enemyPoppingTimer -= deltaTime;
				//Timer��0�ȉ��ɂȂ�����C���^�[�o���� �^�C�}�[�����Z����
			}
			else {
				enemyPoppingTimer += enemyPoppingInterval;


				const int maxPopCount = 10;
				const int popCount = std::min(enemyLeft, maxPopCount);
				enemyLeft -= popCount;

				//�����_���͈̔͂������Őݒ肷��
				std::uniform_int_distribution<int> rangeBaseX(player.position.x - 30, player.position.x + 30);
				std::uniform_int_distribution<int> rangeBaseZ(player.position.z - 30, player.position.z + 30);
				std::uniform_int_distribution<int> range(-15, 15);
				std::uniform_int_distribution<int> hpRange(10, 30);
				std::uniform_int_distribution<int> speedRange(4, 12);

				//���������x�[�X�|�W�V���������肷��
				glm::vec3 posBase(rangeBaseX(random), 0, rangeBaseZ(random));

				//�o���ʒu��m�点�邽�߂�SE��炷
				sZombiePopSE.SetPosition(posBase);
				sZombiePopSE.Play();

				for (int i = 0; i < popCount; ++i) {
					glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));


					//Enemy�̐����͈͐���
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
					if (zombie) {
						//===================�G�l�~�[��Model���������@�S�Ԃ����������X�gNumber��===========
						zombie->Initialize(4, texZombie.Get(), hpRange(random), pos, glm::vec3(0), glm::vec3(1.2));
						zombie->colLocal = { glm::vec3(-0.85f, 0, -0.8f), glm::vec3(1.7f, 1.8f, 1.6f) };
						zombie->target = &player;
						zombie->baseSpeed = (enemySpeed * speedRange(random)) * 0.1f;
					}
				}
			}
		}
	}



	//Enemy�̏�Ԃ��X�V
	for (auto& actor : enemyList) {
		//HP��0�ȉ��Ȃ玟�̃A�N�^�[������
		if (actor->Actor_HP <= 0) {
			continue;
		}
		//0�ȏ�̂Ƃ�
		ZombieActor* zombie = (ZombieActor*)actor;
		//�]���r�̃^�[�Q�b�g��HP0�Ȃ珈�����I������
		if (zombie->target->Actor_HP <= 0) {
			continue;
		}
		//�A�^�b�N�t���O��true�Ȃ�
		if (zombie->isAttacking) {
			const glm::vec3 vFront = glm::rotate(glm::mat4(1), zombie->rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 0);
			const glm::vec3 vTarget = zombie->target->position - zombie->position;
			const float angle = std::acos(glm::dot(vFront, vTarget));
			//�\���ɋ������߂��A���ʂɃ^�[�Q�b�g������΍U��.
			if (std::abs(angle) < glm::radians(45.0f) && glm::length(vTarget) < 1.5f) {
				--zombie->target->Actor_HP;
				sZombiePopSE.SetPosition(zombie->position);
				sZombiePopSE.Play();
			}
		}
		//Enemy�̍s���͈͐���
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

	//�}�b�v�O�Ȃ�HP0�ɂ���
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


	//===========================�J�����̍X�V.====================================--
	const glm::vec3 viewOffset = glm::vec3(0.4f, 1.8f, 2.0f);
	viewPos = player.position + viewOffset;



	// �v���C���[�̒e�̔��˃^�C�}�[�̍X�V.
	if (playerBulletTimer > 0) {
		playerBulletTimer -= deltaTime;
	}


	//=================================================================================================
	//=========================================�Փ˔���=================================================
	//=================================================================================================

	//===============================�ǉ������Փ˔���1======================================
	//�G���m�̏Փ˔���
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

	//===============================�ǉ������Փ˔���2======================================
	//�G�ƉƂ̏Փ˔���
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

	//===============================�ǉ������Փ˔���3======================================
	//�G�Ɩ؂̏Փ˔���
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

	//===============================�ǉ������Փ˔���4======================================
	//�G�Ɩ؂̏Փ˔���
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

	// �v���C���[�̒e�ƓG�̏Փ˔���.
	DetectCollision(playerBulletList, enemyList, [&](Actor& bullet, Actor& zombie) {
		const CollisionTime t = FindCollisionTime(bullet, zombie, deltaTime);
		if (t.time) {
			zombie.Actor_HP -= bullet.Actor_HP;
			bullet.Actor_HP = 0;
			//===================-���eSE��炷=======================
			sHitEnemy.SetPosition(zombie.position);
			sHitEnemy.Play();

			if (zombie.Actor_HP <= 0) {
				//==========================���SSe��炷============================
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

	//===============================�ǉ������Փ˔���5======================================
	//�e�ƃI�u�W�F�N�g�̏Փ˔���
	DetectCollision(playerBulletList, houseList, [&](Actor& bullet, Actor& object) {
		//�e�ƃI�u�W�F�N�g��������܂ł̎��Ԃ𒲂ׂ�
		const CollisionTime t = FindCollisionTime(bullet, object, deltaTime);
		//�Փ˂�����
		if (t.time)
		{
			//=========================���eSE================================
			sHitObject.SetPosition(bullet.position);
			sHitObject.Play();
			//�e����HP��0�ɂ���
			bullet.Actor_HP = 0;
		}
	});


	// �G�����ׂē|������X�e�[�W�N���A.
	if (enemyKilled == enemyTotal) {
		state = State::stageClear;
		sAssaultSE.Stop();
		if (StageFinishSE)
		{
			StageFinishSE = false;
			sClearSE.Play();
		}
	}


	// �v���C���[�̗̑͂�0�ȉ��ɂȂ�����Q�[���I�[�o�[.
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
	//===================================================================
	//=========================�t�F�[�h����===============================
	//===================================================================

	if (fadein || fadeOut)
	{
		//����faderate�̏��������܂��Ȃ�
		if (!fadeinit)
		{
			//in�̂Ƃ���DEFAULT��
			if (fadein)
			{
				faderate = DEFAULT_FADERATE;
				soundfade = 0.0f;
			}
			//out�̂Ƃ��͂O��
			else
			{
				faderate = 0.0f;
				soundfade = 1.0f;
			}
			fadeinit = true;
		}


		//Fade IN ����
		if (fadein)
		{
			if (faderate > 0.0f)
			{
				//2�b��in
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
				//2�b��out
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

	//StageClear����
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
	//GameOver����
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
* �`��.
*/
void MainGameScene::Render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.3f, 0.01f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//=======================================���W�ϊ��s����쐬����.=========================================
	/*
	* �p�[�X�y�N�e�B�u(3�������e)�̍s��
	* @param fovy(Field of View) ����p
	* @param aspect ��ʕ��䗦
	* @param near ���_�ƃX�N���[���̋���
	* @param far ����ʂ̋���(�����킩��Ȃ�)
	*/
	const glm::mat4x4 matProj = glm::perspective(glm::radians(55.0f), 16.0f / 9.0f, 0.1f, 500.0f);

	//���[�J�����W�̉�]��|�W�V�������擾���邽�߂̍s��v�Z
	const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(CameraAngleX), glm::vec3(0, 1, 0));
	//const glm::mat4x4 rotaPT = glm::rotate(glm::mat4(1), glm::radians(player.rotation.y), glm::vec3(0, 1, 0));
	//�J�����̏œ_���W
	targetPos = rotaPT * glm::vec4(0, glm::radians(CameraAngleY), -(P_T_Distance), 1);
	/*
	*	@param viewOffset �J�����̃��[�J�����W
	*	@mode TPS	�f�t�H���g��	DEFAULT_CAM_OFFSET_TPS
	*	@mode FPS	�f�t�H���g��	DEFAULT_CAM_OFFSET_FPS
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
	//�J�����̍��W�� ���[�J�����W ���� ���[���h���W �֕ϊ�����
	vPos = rotaPT * glm::vec4(viewOffset, 1);
	//View���W�v�Z�s��
	const glm::mat4x4 matView = glm::lookAt(player.position + vPos, player.position + targetPos, glm::vec3(0, 1, 0));




	progLighting.SetViewProjectionMatrix(matProj * matView);
	progSimple.SetViewProjectionMatrix(matProj * matView);

	progLighting.Use();

	meshList.BindVertexArray();

	// ������ݒ肷��.
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

		// �X�e�[�W���\��.
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
		//�c�e��
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

		// HP�\��.
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

		// �X�R�A�\��.
		progSimple.BindTexture(0, texScore.Get());

		progSimple.Draw(meshList[5], glm::vec3(-120, 350 - 24 - 4, 0), glm::vec3(0), glm::vec3(384 / 4, 128 / 4, 1.5));

		int tmpScore = score;

		if (tmpScore > 99999999)
		{
			tmpScore = 99999999;
		}

		//�������e�N�X�`������擾������ɓK�p����
		for (int i = 0; i < 8; ++i)
		{
			const int number = tmpScore % 10;
			progSimple.BindTexture(0, texNumber[number].Get());

			progSimple.Draw(meshList[5], glm::vec3(-32 + 32 * (8 - i), 350 - 24 - 4, 0), glm::vec3(0), glm::vec3(32, 32, 1.5));

			tmpScore /= 10;
		}

		// �X�e�[�W�N���A�E�Q�[���I�[�o�[�\��.
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
		// �t�F�[�h�A�E�g�p
		progSimple.BindTexture(0, texFadeSheet.Get());

		//�����t�F�[�h�t���O��True��������
		if ((fadein || fadeOut) || gameOverMove)
		{
			//�t�F�[�h����
			progSimple.Draw(meshList[5], glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1920, 1080, 10), backGroundAlpha * faderate);
		}

		//�f�o�b�O�p
		progSimple.BindTexture(0, texDebugImage.Get());
		//�����f�o�b�O���[�h�Ȃ�
		if (DEBUGMODE)
		{
			progSimple.Draw(meshList[5], glm::vec3(500, 200, 0), glm::vec3(0), glm::vec3(128, 128, 1), 0.8f);
		}

	}
}

/**
* �I��.
*/
void MainGameScene::Finalize()
{
	ClearActorList(playerBulletList);
	ClearActorList(enemyList);
	ClearActorList(objectList);
}

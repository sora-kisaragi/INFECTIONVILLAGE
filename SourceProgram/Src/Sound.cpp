// @file Sound.cpp


#include "Sound.h"
#include "SoundBuffer.h"
#include <al.h>
#include <alc.h>
#include <vector>
#include <iostream>

namespace Sound
{


	/* @Engine
	*============================================================
	* �S�̏���������̂ɕK�v�Ȋ֐��Q
	*
	* 
	*
	*
	*=============================================================
	*/

	/* Initialize()
	* �G���W���̏�����:
	*
	* @return	true ���� : false ���s.
	*
	* @note		�K���ŏ��ɏ��������Ă�������
	*			����������Ă��Ȃ��ꍇ.�S�Ă̊֐��͖����ł�.
	*/
	bool EngineInitialize(unsigned short sourceCapacity, unsigned short actorCapacity)
	{
		return Engine::Instance().Init(sourceCapacity, actorCapacity);
	}

	/* Update()
	* �G���W���̃A�b�v�f�[�g(���t���[��)
	*
	* @param deltaTime:	�f���^�^�C��(�O�񂩂�̍X�V����)
	*
	* @note		��Ƀo�b�t�@�[�̍X�V���s���܂�
	*			�Q�[�����[�v�Ńt���[�����ɓǂ�ł�������
	*/
	void EngineUpdate(float deltaTime)
	{
		Engine::Instance().Update(deltaTime);
	}



	/* @Listener
	*============================================================
	* �������m����ʒu���.
	* 3D�T�E���h�𗘗p����ꍇ�͕K�{���ڂɂȂ�܂�
	*
	*
	*
	*=============================================================
	*/

	/* @Listener
	*
	* ���X�i�[�̈ʒu��ύX����.
	*
	* @param x,y,z :���X�i�[�̋�ԍ��W(�ʒu�x�N�g��)
	*				�f�t�H���g�l{0.0f,0.0f,0.0f}
	*
	* @note 3D�I�u�W�F�N�g�𗘗p����ꍇ,���X�i�[�̈ʒu,����,
	*		3D�I�u�W�F�N�g�̍��W�Ȃǂ�,�������������傫�����v�Z�����
	*/
	void ListenerPostion(float x, float y, float z)
	{
		Engine::Instance().SetListenerPosition(x, y, z);
	}

	/* @Listener
	*
	* ���X�i�[�̑��x��ݒ肷��.
	*
	* @param x,y,z :���X�i�[�̑��x�x�N�g��
	*				�f�t�H���g�l{0.0f,0.0f,0.0f}
	*
	* @note ���ۂɓ����킯�ł͂Ȃ��h�b�v���[���ʂ̌v�Z�Ɏg���l(����킵��)
	*/
	void ListenerVelocity(float x, float y, float z) {
		Engine::Instance().SetListenerVelocity(x, y, z);
	}

	/* @Listener
	*
	* ���X�i�[�̌����x�N�g����ݒ肷��
	*
	* @param x,y,z :�����x�N�g��(�P�ʃx�N�g��)
	*				�f�t�H���g�l{0.0f,0.0f-1.0f}
	*
	* @note 3D�I�u�W�F�N�g�𗘗p����ꍇ,���X�i�[�̈ʒu,����,
	*		3D�I�u�W�F�N�g�̍��W�Ȃǂ�,�������������傫�����v�Z�����
	*/
	void ListenerDirection(float x, float y, float z) {
		Engine::Instance().SetListenerDirection(x, y, z);
	}

	/* @Listener
	*
	* ���X�i�[�̏�����x�N�g����ݒ肷��
	*
	* @param x,y,z :�����x�N�g��(�P�ʃx�N�g��)
	*				�f�t�H���g�l{0.0f,1.0f,0.0f}
	*
	* @note 3D�I�u�W�F�N�g�𗘗p����ꍇ,(��)...��Q��
	*/
	void ListenerUpVector(float x, float y, float z) {
		Engine::Instance().SetListenerUpVector(x, y, z);
	}
	void ListenerPrint() {
		float pos[3] = { 0.0f,0.0f,0.0f };
		alGetListenerfv(AL_POSITION, pos);
		std::cout << "x:" << pos[0] << " y:" << pos[1] << " z<:" << pos[2] << std::endl;
		float velo[3] = { 0.0f,0.0f,0.0f };
		alGetListenerfv(AL_VELOCITY, velo);
		std::cout << "x:" << velo[0] << " y:" << velo[1] << " z<:" << velo[2] << std::endl;

	}
	/* @Listener
	*
	* �}�X�^�[���ʂƎv���Ă悢,
	*
	* @param volume	:����[0.0 ~ 1.0f]
	*				 �f�t�H���g�l{1.0f}
	*
	* @note Player�N���X�ŌX�̉��ʂ𒲐߂��邱�Ƃ��\
	*/
	void ListenerVolume(float volume) {
		Engine::Instance().SetListenerGain(volume);
	}

	/* @World
	*
	* ������ݒ肷��.
	*
	* @param speedOfSound : ���̑���[0.0001~]
	*						�f�t�H���g�l{ 343.3}
	*
	* @note �h�b�v���[���ʂɉe��������l
	*/
	void WorldSpeedOfSound(float speedOfSound)
	{
		Engine::Instance().SetALSpeedOfSound(speedOfSound);
	}
	/* @World
	* �����S�ẴI�u�W�F�N�g��velocity�̃X�P�[�����Ǝv���(�\�z)
	* 
	* @param dopperFactor :�@[0.0f ~]
	*					   �f�t�H���g�l{1.0f}
	*
	* @note �悭�킩���ĂȂ�.�׋��s��
	* --�ȉ���������a�󂵂Ĉ��p--
	* �\�[�X�ƃ��X�i�[�̑��x�𒲐����܂��B
	*/
	void WorldDopplerFactor(float dopplerFactor)
	{
		Engine::Instance().SetALDopplerFactor(dopplerFactor);
	}
	/* @World
	* �����S�ẴI�u�W�F�N�g��velocity�̏搔���Ǝv���(�\�z)
	*
	* @param velocity:���t�@�����X�ɂ�Range������ĂȂ�...
	*				  �f�t�H���g�l{�s��}
	*
	* @note �悭�킩���ĂȂ�.�׋��s��
	* --�ȉ���������a�󂵂Ĉ��p--
	* �����ɓK�p�����搔�B
	*/
	void WorldDopplerVelocity(float velocity)
	{
		Engine::Instance().SetALDopplerVelocity(velocity);
	}

	/* @World
	* �f�B�X�^���X���f���Ȃ���̂�����炵��.
	* �ǂ����Ă����������������炨�₢���킹��������...
	*
	*
	*
	*/
	//void WorldDistanceModel(enum modle) {
	//	Engine::Instance().SetALDistanceModel()
	//}





	/* @Player
	*============================================================
	* �Œ艹��,
	* BGM��UI��SE����
	* �܂�2D�Q�[���Ŏg�p����Ȃ炱��ŏ\�����낤
	*
	* �Đ�,��~,�ꎞ��~��,��{�I�ȋ@�\������
	*=============================================================
	*/

	/* @Player
	* Init�֐�,�o�b�t�@�[�ɃA�N�^�[�I�u�W�F�N�g���쐬����.
	*
	* @param filename: wave�t�@�C��(.wav)
	*
	* @note	wave�t�@�C��(�X�e���Ior���m����)�̂ݎ󂯕t����
	*		Init���Ȃ��Ƒ��̃����o�֐����g�p�ł��Ȃ�.
	*/
	bool Player::Init(const char* filename)
	{
		Reset();
		Engine::Instance().CreateActor<ActorClasses::Actor>(actorId, filename);
		if (actorId < 0) {
			ErrorPrint("Init():Actor�̐����Ɏ��s���܂���");
			return false;
		}
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Init();


		return true;
	}

	/* @Player
	*
	* �����Đ�����
	*
	* @param timeToGrow : �������X�ɑ傫���Ȃ鎞��(�b)
	*
	* @note : ������ݒ肷��Ə��X�ɑ傫���Ȃ��Ď��Ԃ��o�Ɩ{���̉��ʂɂȂ�
	*/
	void Player::Play(float timeToGrow)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Play(actorId, timeToGrow);
	}
	/* @Player
	* �Đ����ł���Β�~����.
	*
	* @param timeToStop :�����~�܂�܂ł̎���(�b)
	*
	* @note ������ݒ肷��Ɖ����~�܂�܂ŏ��X�Ɍ������Ă���
	*/
	void Player::Stop(float timeToStop)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Stop(timeToStop);
	}
	/*@Player
	*
	* �����ꎞ��~����.����Đ��̍ۂ͓r������Đ������
	*
	* @note ���p����ƃ\�[�X������Ȃ��Ȃ�̂� �r���Ŏ~�߂�K�v�̂Ȃ����̂�Stop�Ŏ~�߂悤,
	*
	*/
	void Player::Pouse()const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Pouse();
	}

	/* @Player
	*
	* ���[�v��ݒ肷��
	*
	* @param io :  true < ���[�v�I�� : false < ���[�v�I�t
	*				�f�t�H���g�l false
	*/
	void Player::SetLooping(bool io)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->SetLooping(io);
	}
	/* @Player
	*
	* ���ʂ�ݒ肷��
	*
	* @param volume:�ݒ肷�鉹��(0.0f~0.1f)
	*				�f�t�H���g�l(0.8f)
	*
	* @note �{����oenal���̃f�t�H���g��1.0f�Ȃ̂���,�����ł���
	*/
	void Player::SetVolume(float volume)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr = Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId);
		actorPtr->SetVolume(volume);
	}
	/* @Player
	*
	* �s�b�`��ݒ肷��
	*
	* @param pitch :�ݒ肷��s�b�`(0.0f~)
	*				�f�t�H���g�l(1.0f);
	*
	* @note �����ɂ����1�I�N�^�[�u������,��{�ɂ����1�I�N�^�[�u�オ��,�ȏ�
	*/
	void Player::SetPitch(float pitch)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr = Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId);
		actorPtr->SetPitch(pitch);
	}

	/* @Plyaer(���N���X)
	* �f�X�g���N�^<<�o�b�t�@����Ώۂ̃A�N�^�[���폜����.
	*/
	Player::~Player()
	{
		Reset();
	}

	/* @Player
	*
	* @�����L�[��\��<�f�o�b�O�p>
	*/
	void Player::IdPrint()const
	{
		std::cout << actorId << std::endl;
	}
	/* @PlayerNormal
	* �G���[��\������<>
	*/
	void Player::ErrorPrint(const char* massage)
	{
		std::cerr << "ERROR>>Sound::PlayerNormal::" << massage << std::endl;
	}

	/* @Player
	*
	* �A�N�^�[�̔j�����蓮�ōs���܂�
	* ���s��Ȃ��ꍇ�͕ϐ����j�������Ƃ��Ƀf�X�g���N�^�Ŏ����ōs����.
	*/
	void Player::Reset()
	{
		if (actorId >= 0) {
			Engine::Instance().ActorDestroy(actorId);
		}
	}
	/*
	* �f�o�b�O�p�@al�\�[�X�̃X�e�[�g���擾����.
	*/
	void Player::DebugSourcePrint()const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->PrintSourceState();
	}


	/* @Player3D
	*============================================================
	* 3D�T�E���h�Ŏg�p����
	* �g�p����ꍇ�̓��X�i�[�̐ݒ���K�{�ɂȂ�̂�
	* ��������m�F���悤
	*
	* Player�̊֐��͑S�Čp�����Ă���+@�œƎ��֐�������
	*=============================================================
	*/


	/* @Player3D
	* Init�֐�,�o�b�t�@�[�ɃA�N�^�[�I�u�W�F�N�g���쐬����.
	*
	* @param filename: wave�t�@�C��(.wav)
	*
	* @note		wave�t�@�C��(�X�e���Ior���m����)�̂ݎ󂯕t����
	*/
	bool Player3D::Init(const char* filename)
	{
		Reset();
		Engine::Instance().CreateActor<ActorClasses::Actor3D>(actorId, filename);
		if (actorId < 0) {
			ErrorPrint("Init():Actor�̐����Ɏ��s���܂���");
			return false;
		}
		std::shared_ptr<ActorClasses::Actor3D>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId));
		actorPtr->Init();


		return true;
	}

	/*@Player3D
	* �|�W�V������ݒ肷��
	*
	* @param x,y,z :��ԏ�̈ʒu
	*				�f�t�H���g�l{0.0f,0.0f,0.0f}
	*
	* @note Listener�̍��W�ƌ����ƃI�u�W�F�N�g�̈ʒu����,
	*		�����������������A���̌����Ȃǂ����܂�.
	*/
	void Player3D::SetPosition(float x, float y, float z)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->SetPosition(Vector{ x,y,z });
	}

	/*@Player3D
	* ���x��ݒ肷��,�ʒu�������킯�ł͂Ȃ��h�b�v���[���ʂ̌v�Z�Ɏg�p����.
	*
	* @param x,y,z :���x�x�N�g��
	*				�f�t�H���g�l{0.0f,0.0f,0.0f}
	*
	* @note
	*/
	void Player3D::SetVelocity(float x, float y, float z)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->SetVelocity(Vector{ x,y,z });

	}

	/*@Player3D
	* �����ɂ�錸������ݒ肷��
	*
	* @param referenceDistance :���������������鋗��
	*							�f�t�H���g�l{1.0f}
	*
	* @note �����ɂ�鉹�ʂ̌����ɉe������l:�����قǌ������ɂ���,�Z���قǌ������₷��.
	*/
	void Player3D::SetReferenceDistance(float referenceDistance)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->SetReferenceDistance(referenceDistance);
	}
	/*@Player3D
	* �����ɂ�錸�����Ȃ��Ȃ鋫�E����ݒ肷��
	*
	* @param maxDistance	:�������Ȃ��Ȃ�܂ł̋���(���X�i�[����̑��΋���)
	*						 �f�t�H���g�l{100.0f}��openAL�ł�distanceModel�̐ݒ�ɂ�邽�ߏ���Ɍ��߂Ă�
	*
	* @note referenceDistance(��L)�Ƃ��܂��g�ݍ��킹�Ďg�����Č����������Ă�
	*/
	void Player3D::SetMaxDistance(float maxDistance)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->MaxDistance(maxDistance);
	}
	/*@Player3D
	* ���ʂ̍Œ�l�̐ݒ�(�v�Z�K�p��)
	*
	* @param minVolume	:min����(0.0f~)
	*					 �f�t�H���g�l{0.0f}
	*
	* @note �����ɂ���ĉ��������Ăق����Ȃ��ꍇ�͐ݒ肷��Ƃ悢
	*/
	void Player3D::SetMinVolume(float minVolume)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->MinVolume(minVolume);
	}
	/*@Player3D
	* ���ʂ̍ő�l�̐ݒ�(�v�Z�K�p��)
	*
	* @param maxVolume	 :max����(0.0f~1.0f)
	*					  �f�t�H���g�l{1.0f}
	*
	* @note ��肭�g���Ă�������(�Ă��Ɓ[)
	*/
	void Player3D::SetMaxVolume(float maxVolume)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->MaxVolume(maxVolume);
	}





	/* @Player3DCustom
	*============================================================
	* �J�X�^���Ƃ������Ƃł��܂�g�������ɂȂ��@�\��
	* ������ɉ񂳂��Ă������.
	* ���̒��̋@�\���g��Ȃ��̂ł���΃x�[�X��3D�̕����g�p���悤
	*
	*
	*=============================================================
	*/
	bool Player3DCustom::Init(const char* filename)
	{
		Reset();
		Engine::Instance().CreateActor<ActorClasses::Actor3DCustom>(actorId, filename);
		if (actorId < 0) {
			ErrorPrint("Init():Actor�̐����Ɏ��s���܂���");
			return false;
		}
		std::shared_ptr<ActorClasses::Actor3DCustom>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId));
		actorPtr->Init();
		return true;
	}


	/*@Player3DCustom
	* ������ύX����
	*
	* @param x,y,z :�����x�N�g��(�P�ʃx�N�g��)
	*				�f�t�H���g�l{0.0f,0.0f,0.0f}
	*
	* @note ��q�̉��̕����ɂ�錸���̐ݒ�Ɛ[���֌W����
	*/
	void Player3DCustom::SetDirection(float x, float y, float z)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetDirection(Vector{ x,y,z });
	}


	/*@Player3DCustom
	* �R�[���̃C���i�[�p�̐ݒ�
	*
	* @param coneInnerAngle	:�~���̒��_�̊p�x(0.0f~360.0f)
	*						 �f�t�H���g�l{360.0f}
	*
	* @note ConeOutorVolume�̌������K�p����Ȃ��p�x.�����̐ݒ��Distance��,
	*/
	void Player3DCustom::SetConeInnerAngle(float coneInnerAngle)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetConeInnerAngle(coneInnerAngle);
	}
	/*@Player3DCustom
	* �R�[���̃A�E�^�[�p�̐ݒ�
	*
	* @param ConeOuterAngle :�~���̒��_�̊p�x(0.0f~360.0f)
	*					     �f�t�H���g�l{360.0f}
	*
	* @note �C���i�[�p�ƃA�E�^�[�p�̊Ԃŏ��X�Ɍ�������Ă����C���[�W
	*/
	void Player3DCustom::SetConeOuterAngle(float coneOuterAngle)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetConeOuterAngle(coneOuterAngle);
	}
	/*@Player3DCustom
	* �A�E�^�[�p�̊O�̉��ʂ�ݒ肷��
	*
	* @param coneOuterVolume :max����(0.0f~1.0f)
	*						  �f�t�H���g�l{1.0f}
	*
	* @note �����̌����Ɠ����~��,�O���~��,�O�^�~���̊O�̉���...�l�l�̓Z�b�g�Ȃ�ł�.
	*/
	void Player3DCustom::SetConeOuterVolume(float coneOuterVolume)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetConeOuterVolume(coneOuterVolume);
	}
	/*@Player3DCustom
	* ���[���L���x�c�ł���(��R)
	*
	* @param rolloff :���[���I�t�̒l [0.0f - ]
	*				  �f�t�H���g�l{1.0f}
	*
	* @note �����������(�{�l���g�������킩��Ȃ�)
	* --�ȉ���������a�󂵂Ĉ��p--
	* �����������֒��܂��͌��������邽�߂̏搔
	* 0.0�ł́A�����̌����͔������܂���B
	*/
	void Player3DCustom::SetRollOffFactor(float rolloff)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetRollOffFactor(rolloff);
	}

}


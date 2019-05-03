/*@file SoundBuffer.h*/

#ifndef SOUNDBUFFER_H_INCLUDED
#define SOUNDBUFFER_H_INCLUDED

/*@include Lib*/
#include "SoundActor.h"
#include <al.h>
#include <alc.h>
#include <unordered_map>
#include <string>
#include <iostream>

namespace Sound
{
	class Engine;
	using EngineRef = Engine & ;
	class Engine :public ActorClasses
	{
	public:
		static EngineRef Instance();	//�V���O���g���C���X�^���X
		static ActorClasses& ActorInstance();
		bool Init(unsigned short sourceCapacity, unsigned short actorCapacity);					//������
		void Update(float delta);		//��ɌĂ�.

		/*OpenAL���X�i�[�̐ݒ�*/
		void SetListenerPosition(float x,float y,float z)const;
		void SetListenerGain(float gain)const;
		void SetListenerVelocity(float x,float y,float z)const;
		void SetListenerDirection(float x,float y,float z);
		void SetListenerUpVector(float x,float y,float z);

		/*OpenAL�S�̐ݒ�*/
		void SetALSpeedOfSound(float speedOfSound)const;
		void SetALDopplerFactor(float dopplerFactor)const;
		void SetALDopplerVelocity(float dopplerVelocity)const;
		//void SetALDistanceModel();
		
		/*actorList�֘A*/
		template<typename T>
		bool CreateActor(short& id, std::string filename);//retval:index;
		template<typename T>
		std::shared_ptr<T> GetActorPtr(short index)const;
		void ActorDestroy(short& id);



	private:
		//����.�R�s�[�h�~
		Engine() = default;
		~Engine();
		Engine(const EngineRef) = delete;
		EngineRef operator=(const EngineRef) = delete;

	private:
		/*sourceList�֘A*/
		virtual std::shared_ptr<ALSourceID> GetSourcePtr(int index)const override;

		virtual std::shared_ptr<ALBufferID> GetBufferPtr(std::string)const override;
		virtual std::shared_ptr<ALBufferID> GetBuffer3DPtr(std::string)const override;
		virtual int SetSourceToActor(int actorId)override;

		/*wavDataList�֘A*/
		virtual void ReadSoundData(std::string filename)override;
		virtual void ReadSoundData3D(std::string filename)override;


		/*�N���X���Ŏg���@�\*/
		void ErrorPrint(const char*)const;//�G���[�̏o��
		void BufferInitialize(unsigned short sourceCapacity,unsigned short actorCapacity);
		void BufferFinalize();


		//void DisconnectActorFromSource(int sourceId);//�\�[�X����A�N�^�[��؂藣��.
		/*�������t���b�O*/
		class{
		public:
			enum BOOLIAN {
				e_BOOL_INITIALIZED_ENGINE = 0x0001,
				e_BOOL_INITIALIZED_DEVICE = 0x0002,
				e_BOOL_INITIALIZED_CONTEXT = 0x0004,
				//e_BOOL_INITIALIZED_ = 0x0008,
				e_BOOL_CHANGED_LISTENER_DELECTION = 0x0010,
				//e_BOOL_INITIALIZED_ = 0x0020,
				//e_BOOL_INITIALIZED_ = 0x0040,
				//e_BOOL_INITIALIZED_ = 0x0080,
				//e_BOOL_INITIALIZED_ = 0x0100,
				//e_BOOL_INITIALIZED_ = 0x0200,
				//e_BOOL_INITIALIZED_ = 0x0400,
				//e_BOOL_INITIALIZED_ = 0x0800,
				//e_BOOL_INITIALIZED_ = 0x1000,
				//e_BOOL_INITIALIZED_ = 0x2000,
				//e_BOOL_INITIALIZED_ = 0x4000,
				//e_BOOL_INITIALIZED_ = 0x8000,
			};
			void Set(BOOLIAN b, bool io) { io ? boolian |= b : boolian &= ~b; };
			bool Get(BOOLIAN b)const { return boolian & b; };
		private:
			uint16_t boolian = 0;
		}flagList;
		ALCdevice* device;
		ALCcontext* context;

		/*Update()�Ń\�[�X����I����Ă邩�ǂ������m�F�������*/
		float checkSourcesStateTime = 1.0f;
		float checkSourcesStateTimer = 0.0f;

		/*Listener�֘A*/
		Vector listenerDirection = { 0.0, 0.0, -1.0};
		Vector listenerUpVector = { 0.0, 1.0, 0.0 };

		/*�S�̃{�����[��*/

		//�I�u�W�F�N�g�̃o�b�t�@�[
		std::unordered_map<std::string, std::shared_ptr<ALBufferID>> soundDataList;
		std::unordered_map<std::string, std::shared_ptr<ALBufferID>> soundData3DList;
		std::vector <std::shared_ptr<ALSourceID>> sourceList;
		std::vector<std::shared_ptr<EngineFunc>> actorList;

		//�T�E���h�f�[�^���X�g(soundDateList�̏��L�Ґ����Ǘ�����)
		std::unordered_map<std::string,int>soundDataCounter;
		std::unordered_map<std::string,int>soundData3DCounter;

		//�A�N�^�[�̃_�~�[�|�C���^��ԋp���邽�߂̃N���X
		class DummyActor;
	};



	/*GetActorPtr�ŕs����id�ŃA�N�Z�X�����ۂɕԂ��A�N�^�[�̃_�~�[�N���X
	* �|�C���^����A�N�Z�X�ł���ł��낤�p�u���b�N�֐����l�ׂ��ŋ�̊֐���
	* �I�[�o�[���C�h����.
	*/
	class Engine::DummyActor :/*public Actor, public Actor3D, */public Actor3DCustom
	{
	public:
		/*Actor(�e�N���X)*/
		bool Init() override { WarningCall(); return false; }
		void Play(short actorId,float)override { WarningCall(); }
		void Stop(float)const override { WarningCall(); };
		void Pouse()const override { WarningCall(); }
		void SetLooping(bool)override { WarningCall(); }
		void SetVolume(float)override { WarningCall(); }
		void SetPitch(float)override { WarningCall(); }
		void PrintSourceState()override { WarningCall(); }
		/*Actor3D*/
		void SetPosition(const Vector&)override { WarningCall(); }
		void SetVelocity(const Vector&)override { WarningCall(); }
		void SetReferenceDistance(float distance)override { WarningCall(); }
		void MaxDistance(float distance)override { WarningCall(); }
		void MinVolume(float volume)override { WarningCall(); }
		void MaxVolume(float volume)override { WarningCall(); }
		/*Actor3D*/
		void SetDirection(const Vector&)override { WarningCall(); }
		void SetConeInnerAngle(float angle)override { WarningCall(); }
		void SetConeOuterAngle(float angle)override { WarningCall(); }
		void SetConeOuterVolume(float volume)override { WarningCall(); }
		void SetRollOffFactor(float rolloff)override { WarningCall(); }

		/*�G���[��*/
		void WarningCall()const { std::cerr << "<�x��>DummyActor�|�C���^�̊֐����g�p���Ă��܂�" << std::endl; }

		DummyActor() {}
		~DummyActor() {}
	};


	/* @ObjectBuffer
	* �V����Actor���쐬����.
	*
	* @param id			�A�N�^�[�̃A�N�Z�X�L�[���󂯎��ϐ�
	* @param filename	�g�p����wav�t�@�C����
	*
	* @retval	:true ���� :false ���s
	*
	* @note �A�N�^�[ID�����ɏ������Ă���ꍇ�͎��s����.
	*/
	template<typename T>
	bool Engine::CreateActor(short& id,std::string filename)
	{
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE)) {
			ErrorPrint("CreateActor():�G���W���̏��������������Ă��܂���");
			return false;
		}
		//���ɃL�[���������Ă���ꍇ�͎��s
		if (id >= 0) {
			ErrorPrint("CreateActoro():���ɃA�N�^�[ID���������Ă���");
			return false;
		}
		//ActorList����󂢂Ă���v�f�����邩������.
		for (int i = 0; i < actorList.size(); ++i) {
			if (!actorList[i]) {//�󂫗v�f�����������ꍇ�͐V�����v�f���쐬���Y������Ԃ�
				struct Impl:public T{};
				actorList[i] = std::make_shared<Impl>();
				actorList[i]->SetFilename(filename);
				id = i;
				return true;
			}
		}
		//�J���Ă���v�f��������Ȃ������ꍇ�̓G���[��f����-1��Ԃ�.
		ErrorPrint("CreateActor():ActorList�̋󂫂�����܂���.");
		return false;
	}


	/* Actor�̃|�C���^���擾����.
	* 
	* @param index:�A�N�^�[�ɃA�N�Z�X���錮(�z��̓Y����)
	*
	* @note �s����index�������ꍇ�̓_�~�[�|�C���^��Ԃ�
	*		�_�~�[�N���X�̓A�N�Z�X���Ă��G���[�͏o�Ȃ����������Ȃ�
	*/
	template<typename T>
	std::shared_ptr<T> Engine::GetActorPtr(short index)const
	{
		//�G���W���̏��������������Ă��邩.
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE)) {
			ErrorPrint("GetActorPtr():�G���W���̏��������������Ă��܂���.�_�~�[��Ԃ��܂�");
			return std::make_shared<DummyActor>();
		}
		//�͈͊O�̓Y���������ł����ꍇ�̓G���[�ƈꏏ��nullptr��Ԃ�.
		if (index < 0 || index >= actorList.size()) {
			ErrorPrint("GetActorPtr():�s����index���󂯎��܂���.�_�~�[��Ԃ��܂�");
			if (index < 0) {
				std::cerr << "�I�u�W�F�N�g���쐬����Ă��܂���" << std::endl;
			}
			return std::make_shared<DummyActor>();
		}
		//�v�f�̃|�C���^��nullcheck
		if (!actorList[index]) {
			ErrorPrint("GetActorPtr():Actor�����݂��܂���.�_�~�[��Ԃ��܂�");
			return std::make_shared<DummyActor>();
		}

		std::shared_ptr<T>successCast = std::dynamic_pointer_cast<T>(actorList[index]);
		//�_�E���L���X�g�Ɏ��s�����ꍇ
		if (!successCast) {
			ErrorPrint("GetActorPtr():�_�E���L���X�g�Ɏ��s���܂���,��������Actorh�h���^�ƈ�v���Ă邩���m�F���Ă�������");
			return std::make_shared<DummyActor>();
		}

		return successCast;


	}

}

#endif // !SOUNDBUFFER_H_INCLUDED

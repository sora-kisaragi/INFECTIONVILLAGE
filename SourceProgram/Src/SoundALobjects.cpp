#include "SoundALobjects.h"
#include "FileReader_Wav.h"
#include <iostream>

namespace Sound {
	/*
	* �|�C���^�̍쐬
	*/
	ALBufferIDPtr ALBufferID::Create()
	{
		struct  Impl :public ALBufferID {};
		ALBufferIDPtr p = std::make_shared<Impl>();
		return p;
	}
	/*
	* �_�~�[���쐬����
	*/
	ALBufferIDPtr ALBufferID::CreateDummy()
	{
		struct Dummy :public ALBufferID
		{
			virtual bool Init(ALenum format, const ALvoid* data, ALsizei size, ALsizei freq)override { PrintWarning(); return false; };
			virtual bool Init(const char* filename, bool, std::string) override { PrintWarning(); return false; };
			virtual ALuint Id()const override { return id; }///<getter;


			void PrintWarning()const { std::cerr << "<�x��>ALBufferID�̃_�~�[�̊֐����g�p���Ă��܂�" << std::endl; }
		};
		ALBufferIDPtr p = std::make_shared<Dummy>();

		return p;
	}

	/*
	* �o�b�t�@�I�u�W�F�N�g�̍쐬.
	*
	* @param format		 //<ex AL_FORMAT_MONO8,���m���� bitrate 8bit
	* @param data		 //<���M����f�[�^�̃I�t�Z�b�g
	* @param size 		 //<�f�[�^�̃T�C�Y(byte�P��)
	* @param freq		 //<�T���v�����O���g�� (frequency=���g��)
	*
	* @retval true<���� false <���s.
	*/
	//ALenum format
	//const ALvoid *data
	//ALsizei size
	//ALsizei freq
	bool ALBufferID::Init(ALenum format, const ALvoid* data, ALsizei size, ALsizei freq)
	{

		Destroy();
		alGenBuffers(
			1,		//ALsizei n
			&id);	//ALuint *buffers

		alBufferData(
			id,					//ALuint buffer
			format,				//ALenum format
			data,				//const ALvoid *data
			size,				//ALsizei size
			freq);				//ALsizei freq
		return true;
	}

	/*
	* �t�@�C������쐬.
	*
	* @param filename :�t�@�C���̕�����
	*
	* @return true<���� : false<���s
	* @note wav�̂ݑΉ�.
	*/
	bool ALBufferID::Init(const char* filename, bool chengeMono, std::string warningMessage)
	{
		//�ꎞ�I�Ƀf�[�^��ێ�����N���X.
		FileReader::Wave waveReader;
		if (!waveReader.ReadFile(filename)) {
			std::cerr << "Sound::BufferObject::Init<<�t�@�C���̓ǂݍ��݂Ɏ��s���܂���" << std::endl;
			return false;
		}
		//���������m�����ϊ�����ꍇ
		if (chengeMono) {
			if (waveReader.ComvertToMono()) {
				//�ϊ������ꍇ�̓A�v���P�[�V�����̕ϊ����S���������Ă���̂Ōx������
				std::cerr << warningMessage << std::endl;
			}
		}
		waveReader.PrintData();
		//�`�����l���ƃr�b�g���[�g�̎擾
		ALenum format = 0;
		switch (waveReader.GetFormat())
		{
		case FileReader::Wave::e_FORMAT_MONO8:
			format = AL_FORMAT_MONO8;
			break;
		case FileReader::Wave::e_FORMAT_MONO16:
			format = AL_FORMAT_MONO16;
			break;
		case FileReader::Wave::e_FORMAT_STEREO8:
			format = AL_FORMAT_STEREO8;
			break;
		case FileReader::Wave::e_FORMAT_STEREO16:
			format = AL_FORMAT_STEREO16;
			break;

		default:
			std::cerr << "Sound::BufferObject::Init<<�t�H�[�}�b�g���擾�ł��܂���ł���" << std::endl;
			return false;
			break;
		}


		return Init(format, waveReader.GetData().data(), static_cast<ALsizei>(waveReader.GetSize()), waveReader.GetSampleRate());
	}

	/*
	* �o�b�t�@�I�u�W�F�N�g�̔j��
	*
	*/
	void ALBufferID::Destroy()
	{
		if (id) {
			alDeleteBuffers(1, &id);
			id = 0;
		}
	}


	/* @ class SourceObject
	*==================================================================================================
	* ��ɍ��̂Ƃ���̓\�[�X��id��ێ����邾��.
	* �Ƃ肠�����f���[�g�Y��������Ă��邾���Ȃ̂ō�����P�o�����炵����.
	*
	*
	*
	*====================================================================================================
	*/

	/*
	* �I�u�W�F�N�g�̂��쐬����֐�
	*/
	ALSourceIDPtr ALSourceID::Create()
	{
		struct Impl :public ALSourceID {};
		ALSourceIDPtr p = std::make_shared<Impl>();
		return p;
	}
	/*
	* �I�u�W�F�N�g�̃_�~�[���쐬����
	*/
	ALSourceIDPtr ALSourceID::CreateDummy()
	{
		struct Dummy :public ALSourceID
		{
			virtual void Init()override { PrintWarning(); }
			virtual void Play(float) override { PrintWarning(); }
			virtual void Stop(float) override { PrintWarning(); }
			virtual void Pouse() override { PrintWarning(); }
			virtual bool IsActive()override { PrintWarning(); return false; }
			virtual void SetVolume(float vol)override { PrintWarning(); }
			virtual void SetRelative(bool io)const override { PrintWarning(); }
			virtual void SetLooping(bool io)const override { PrintWarning(); }
			virtual void SetPitch(float pitch)const override { PrintWarning(); }
			virtual void SetPosition(float x, float y, float z)const override { PrintWarning(); }
			virtual void SetVelocity(float x, float y, float z)const override { PrintWarning(); }
			virtual void SetDirection(float x, float y, float z)const override { PrintWarning(); }
			virtual void SetReferenceDistance(float referenceDistance)const override { PrintWarning(); };
			virtual void SetMaxDistance(float maxDistance)const override { PrintWarning(); };
			virtual void SetConeOuterGain(float outerGain)const override { PrintWarning(); };
			virtual void SetConeInnerAngle(float innerAngle)const override { PrintWarning(); };
			virtual void SetConeOuterAngle(float outerAngle)const override { PrintWarning(); };
			virtual void SetMinGain(float minGain)const override { PrintWarning(); };
			virtual void SetMaxGain(float maxGain)const override { PrintWarning(); };
			virtual void SetRollOffFactor(float rolloff)const override { PrintWarning(); };
			virtual void ActorId(short id) override { PrintWarning(); }
			virtual short ActorId()const override { PrintWarning(); return actorId; }
			virtual void SetBuffer(ALuint bufferId)const override { PrintWarning(); }
			virtual void PrintState()const override { PrintWarning(); }

			virtual void Update(float) override { PrintWarning(); }

			virtual STATE GetState()const override { PrintWarning(); return e_STATE_MAX; }
			virtual void UnlinkActor()override { PrintWarning(); }
			virtual short ActorIdJustStopped()override { PrintWarning(); return -1; }

			void PrintWarning()const { std::cerr << "<�x��>ALSourceDummy�ɃA�N�Z�X���Ă��܂�" << std::endl; }
		};
		ALSourceIDPtr p = std::make_shared<Dummy>();
		return p;
	}

	/*SoundObject
	* �R���X�g���N�^��alGen���ǂ��Ȃ̂��Ǝv��������.
	*/
	ALSourceID::ALSourceID()
	{
		Init();
	}

	/*
	*
	*
	*/
	void ALSourceID::SetState(STATE newState)
	{
		state = (state & ~e_STATE_MAX) ^ newState;
	}

	ALSourceID::STATE ALSourceID::GetState()const
	{
		return static_cast<STATE>(state & e_STATE_MAX);
	}


	/*�A�N�^�[�ƃ����N���Ă���ꍇ,�A�N�^�[��id�����������ăf�t�H���g�ɖ߂�
	*
	* �֐��̐�����肷�����Y�ł�����ƍ�����,8)
	*/
	void ALSourceID::UnlinkActor()
	{
		//�S�Ă̒l��OpenAL�̃f�t�H���g�ɖ߂�
		Stop();
		SetBuffer();
		SetDirection();
		SetLooping();
		SetPitch();
		SetPosition();
		SetRelative();
		SetVelocity();
		SetGain();
		SetState();
		SetReferenceDistance();
		SetMaxDistance();
		SetConeOuterGain();
		SetConeInnerAngle();
		SetConeOuterAngle();
		SetMinGain();
		SetMaxGain();
		SetRollOffFactor();

		//�A�N�^�[��ID��j������.
		actorId = -1;
	}

	/*
	* OpenAL��Source()�֐����ʂɑS�Ċ֐��ɂ܂Ƃ߂�,
	*
	*
	*
	*/

	void ALSourceID::Init() { alGenSources(1, &id); }
	void ALSourceID::SetBuffer(ALuint bufferId)const { alSourcei(id, AL_BUFFER, bufferId); }
	void ALSourceID::SetGain(float vol)const { alSourcef(id, AL_GAIN, vol); }
	void ALSourceID::SetLooping(bool io)const { io ? alSourcei(id, AL_LOOPING, AL_TRUE) : alSourcei(id, AL_LOOPING, AL_FALSE); }
	void ALSourceID::SetPitch(float pitch)const { alSourcef(id, AL_PITCH, pitch); }
	void ALSourceID::SetPosition(float x, float y, float z)const { alSource3f(id, AL_POSITION, x, y, z); }
	void ALSourceID::SetVelocity(float x, float y, float z)const { alSource3f(id, AL_VELOCITY, x, y, z); }
	void ALSourceID::SetReferenceDistance(float referenceDistance)const { alSourcef(id, AL_REFERENCE_DISTANCE, referenceDistance); }
	void ALSourceID::SetMaxDistance(float maxDistance)const { alSourcef(id, AL_MAX_DISTANCE, maxDistance); }
	void ALSourceID::SetMinGain(float minGain)const { alSourcef(id, AL_MIN_GAIN, minGain); }
	void ALSourceID::SetMaxGain(float maxGain)const { alSourcef(id, AL_MAX_GAIN, maxGain); }
	void ALSourceID::SetDirection(float x, float y, float z)const { alSource3f(id, AL_DIRECTION, x, y, z); }
	void ALSourceID::SetConeInnerAngle(float innerAngle)const { alSourcef(id, AL_CONE_INNER_ANGLE, innerAngle); }
	void ALSourceID::SetConeOuterAngle(float outerAngle)const { alSourcef(id, AL_CONE_OUTER_ANGLE, outerAngle); }
	void ALSourceID::SetConeOuterGain(float outerGain)const { alSourcef(id, AL_CONE_OUTER_GAIN, outerGain); }
	void ALSourceID::SetRollOffFactor(float rolloff)const { alSourcef(id, AL_ROLLOFF_FACTOR, rolloff); }
	void ALSourceID::SetRelative(bool io)const { io ? alSourcei(id, AL_SOURCE_RELATIVE, AL_TRUE) : alSourcei(id, AL_SOURCE_RELATIVE, AL_FALSE); }

	/*ALSourceID
	* �Đ�����.
	*/
	void ALSourceID::Play(float easingTime)
	{
		if (easingTime > 0.0f) {
			easingRate = 1.0f / easingTime;
			easingGain = 0.0f;
			SetGain(easingGain);
			SetState(e_STATE_BEGANPLAY);
		}
		else {
			SetGain(gain);
			SetState(e_STATE_PLAY);
		}
		alSourcePlay(id);
	}
	/*ALSource ��~����
	*
	*/
	void ALSourceID::Stop(float easingTime)
	{
		if (easingTime > 0.0f) {

			SetState(e_STATE_BEGANSTOP);
		}
		else {
			//���X�g�b�v
			alSourceStop(id);
			SetState(e_STATE_STOP);
		}
	}
	void ALSourceID::SetVolume(float f)
	{
		switch (GetState())
		{
		case e_STATE_BEGANPLAY:
			easingGain *= f / gain;
			break;
		case e_STATE_BEGANSTOP:
			easingGain *= f / gain;
			break;

		case e_STATE_PLAY:
			SetGain(gain);
			break;
		default:

			break;
		}
		gain = f;

	}

	/*
	* �ꎞ��~
	*/
	void ALSourceID::Pouse()
	{
		SetState(e_STATE_POUSE);
		alSourcePause(id);
	}

	/*
	* �Đ����Ă����ԂȂ�true
	*/
	bool ALSourceID::IsActive()
	{
		return !((GetState() == e_STATE_STOP) || (GetState() == e_STATE_POUSE));
	}

	/*
	* �A�N�^�[��id��ݒ肷��
	*
	*/
	void ALSourceID::ActorId(short index) {
		//�A�N�^�[��id���������Ă����ꍇ�͒l�����Z�b�g
		if (actorId >= 0) {
			UnlinkActor();
		}
		actorId = index;

	}

	/*
	* �\�[�X�I�u�W�F�N�g��j������.
	*/
	void ALSourceID::Destroy()
	{
		if (id) {
			alDeleteSources(1, &id);
			id = 0;
		}
	}


	void ALSourceID::PrintState()const
	{
		//#define AL_INITIAL                               0x1011
		//#define AL_PLAYING                               0x1012
		//#define AL_PAUSED                                0x1013
		//#define AL_STOPPED                               0x1014
		ALenum state = 0;
		alGetSourcei(id, AL_SOURCE_STATE, &state);
		switch (state)
		{
		case AL_INITIAL:
			std::cout << "AL�X�e�[�g��\��initial" << std::endl;
			break;
		case AL_PLAYING:
			std::cout << "�X�e�[�g��\��:playng" << std::endl;

			break;
		case AL_PAUSED:
			std::cout << "�X�e�[�g��\��:�|�[�Y" << std::endl;
			break;
		case AL_STOPPED:
			std::cout << "�X�e�[�g��\��:�X�g�b�v" << std::endl;

			break;
		default:
			std::cout << "�X�e�[�g��\��:��������" << std::endl;
			break;

		}
	}

	/*
	* �A�b�v�f�[�g
	*
	*/
	void ALSourceID::Update(float delta)
	{
		switch (GetState())
		{
		case e_STATE_BEGANPLAY:
			easingGain += easingRate * delta;
			if (easingGain < gain) {
				SetGain(easingGain);
			}
			else {
				SetGain(gain);
				SetState(e_STATE_PLAY);
			}
			break;
		case e_STATE_BEGANSTOP:
			easingGain -= easingRate * delta;
			if (easingGain > 0.0f) {
				SetGain(easingGain);
			}
			else {
				Stop();
				SetState(e_STATE_STOP);
			}

			break;
		default:
			break;
		}
		//GetChangedStatus();
	}

	/*
	*
	*
	*/
	short ALSourceID::ActorIdJustStopped() {
		//�~�܂��Ă���Ȃ璲�ׂ�K�v���Ȃ�
		if (GetState() == e_STATE_STOP) {
			return -1;
		}
		//��Ԃ��擾����		
		ALenum alSourceState = 0;
		alGetSourcei(id, AL_SOURCE_STATE, &alSourceState);
		//�擾�������ɃX�g�b�v�������ꍇ.
		if (alSourceState == AL_STOPPED) {
			SetState(e_STATE_STOP);
			return actorId;
		}
		else {
			return -1;
		}

	}


	/*
	* ���S�f�o�b�O�p�֐�
	*
	* �O��ƃX�e�[�^�X���ω���������
	*
	*/
	void ALSourceID::GetChangedStatus()
	{
		if (!(teststate == GetState()))
		{
			std::cout << "�O��ƃX�e�[�g���ς��܂���:�O��̃X�e�[�g��\��" << std::endl;
			switch (teststate)
			{
			case e_STATE_STOP:
				std::cout << "stop" << std::endl;
				break;
			case e_STATE_PLAY:
				std::cout << "play" << std::endl;
				break;
			case e_STATE_BEGANPLAY:
				std::cout << "beginplay" << std::endl;
				break;
			case e_STATE_BEGANSTOP:
				std::cout << "beginstop" << std::endl;
				break;
			case e_STATE_POUSE:
				std::cout << "pouse" << std::endl;
				break;
			default:
				std::cout << "�S���ʕ�" << std::endl;
				break;
			}
			std::cout << "����̃X�e�[�g��\��" << std::endl;
			switch (GetState())
			{
			case e_STATE_STOP:
				std::cout << "stop" << std::endl;
				break;
			case e_STATE_PLAY:
				std::cout << "play" << std::endl;
				break;
			case e_STATE_BEGANPLAY:
				std::cout << "beginplay" << std::endl;
				break;
			case e_STATE_BEGANSTOP:
				std::cout << "beginstop" << std::endl;
				break;
			case e_STATE_POUSE:
				std::cout << "pouse" << std::endl;
				break;
			default:
				std::cout << "�S���ʕ�" << std::endl;
				break;
			}
			teststate = GetState();
		}
	}
}

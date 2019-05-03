// @file Sound.h

#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED
namespace Sound
{
	/* �I�[�f�B�I�̑S�̏����ɕK�v�Ȋ֐�.
	* @EngineInitialize :�������֐�.�K���n�߂Ɏg�p����ׂ� 
	* @EngineUpdate		:�X�V�֐�.���t���[���Ɉ��ĂԂׂ�
	*/
	bool EngineInitialize(unsigned short sourceCapacity=32,unsigned short actorCapacity=256);
	void EngineUpdate(float deltaTime);

	/* ���X�i�[�֘A(������̈ʒu��������̏��)
	* @Position		:�ʒu��ݒ肷��(�ʒu�x�N�g��)
	* @Direction	:�����Ă��������ݒ肷��(�P�ʃx�N�g��)
	* @UpVector		:������������Ă�����(�P�ʃx�N�g��)
	* @Velocity		:���x��ݒ肷��(���x�x�N�^�[)
	* @Volume		:���ʂ�ݒ肷��(�}�X�^�[�{�����[��)
	*
	*/
	void ListenerPostion(float x, float y, float z);
	void ListenerDirection(float x, float y, float z);
	void ListenerUpVector(float x,float y,float z);
	void ListenerVelocity(float x, float y, float z);	
	void ListenerVolume(float volume);
	void ListenerPrint();

	/*�S�̂̐ݒ�(�㋉����)
	* @SpeedOfSound			:������ݒ肷��
	* @DopplerFactor		:�悭�킩��Ȃ��V���[�Y�@
	* @DopperVelocity		:�悭�킩��Ȃ��V���[�Y�A
	* @WorldDistanceModel	:������<<�ǂ����Ă��g�����������炨�₢���킹��������<�}�C�P��
	*/
	void WorldSpeedOfSound(float speedObSound);
	void WorldDopplerFactor(float dopplerFactor);
	void WorldDopplerVelocity(float velocity);
	//void WorldDistanceModel(enum modle);//������

	/*
	* �T�E���h�v���C���[(���N���X).
	* bool Init		:������
	* void Play		:�����Đ�
	* void Stop		:�Đ����̉����~
	* void Pause	:�����~�߂�(�ꎞ��~:�~�߂��Ƃ��납�玟��Đ�)
	* void Looping	:���[�v�̐ݒ�on/off
	* void Volume	:���ʂ̒���(0.0f~0.1f)
	* void Reset	:��n��
	* void DebugSourcePrint	:�f�o�b�O�p��Ԃ̕\��
	*
	* @note �N���X�̐����� Engine�̃C���X�^���X����̂݉\
	* 
	*/
	class Player
	{
	public:
		virtual bool Init(const char*);
		virtual void Play(float timeToGrow=0.0f)const;
		virtual void Stop(float timeToStop=0.0f)const;
		virtual void Pouse()const;
		virtual void SetLooping(bool io)const;
		virtual void SetVolume(float vol)const;
		virtual void SetPitch(float pitch)const;

		virtual void Reset();
		virtual void DebugSourcePrint()const;

		virtual void IdPrint()const;
		static void ErrorPrint(const char* message);
		Player() = default;
		~Player();
	protected:
		/*�R�s�[�֎~*/
		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;

	protected:
		short actorId = -1;//�o�b�t�@�[�ɃA�N�Z�X���錮.
	};

	/*
	* 3DPlayer�N���X (Player�̊֐��͊���)
	*
	* void Position()			:3D��ԏ�̍��W
	* void Velocity()			:���x�̐ݒ�(�h�b�v���[���ʂɎg�p�����)
	* void ReferenceDistance()	:
	* void MaxDistance()		:
	* void MinVolume()			:
	* void MaxVolume()			:
	*
	*
	*/
	class Player3D :public Player
	{
	public:
		virtual bool Init(const char*)override;
		virtual void SetPosition(float x, float y, float z)const;
		virtual void SetVelocity(float x, float y, float z)const;

		virtual void SetReferenceDistance(float referenceDistance)const;
		virtual void SetMaxDistance(float maxDistance)const;
		virtual void SetMinVolume(float minVolume)const;
		virtual void SetMaxVolume(float maxVolume)const;

		/*glm::vec3�ɑΉ����Ă��:)*/
		template<typename T>
		void SetPosition(const T& vec3)const;
		template<typename T>
		void SetVelocity(const T& vec3)const;
	};


	/* 3D�T�E���h�J�X�^��(�㋉�Ҍ���)
	* ����l�̓Z�b�g���ڂ݂����Ȃ���
	* void Direction()		:����
	* void ConeInnerAngle()	:�����ɂ�鉹�̌������K������Ȃ��~��
	* void ConeOuterAngle()	:�C���i�[�Ƃ̊Ԃŏ��X�Ɍ������Ă���
	* void ConeOuterVolume():�A�E�^�[�~���̊O�ɏo���Ƃ��̉���
	* void RollOffFactor()	:�����ɂ�錸���̏搔
	*/
	class Player3DCustom :public Player3D
	{
	public:
		virtual bool Init(const char*)override;

		virtual void SetDirection(float x, float y, float z)const;
		virtual void SetConeInnerAngle(float coneInnerAngle)const;
		virtual void SetConeOuterAngle(float coneOuterAngle)const;
		virtual void SetConeOuterVolume(float coneOuterVolume)const;
		virtual void SetRollOffFactor(float rolloff)const;

		/*glm::vec3�ɑΉ����Ă��:)*/
		template<typename T>
		void SetDirection(const T& vec3)const;

	};


	/*
	* glm::vec3���ӎ������֐�
	*  x, y ,z �̏��Ԃ�float �z��Ɋi�[����.
	*/
	template<typename T>
	float* Vector3fv(const T& vec3)
	{
		float floatArray[3] = { vec3.x,vec3.y,vec3.z };
		return floatArray;
	}
	template<typename T>
	void ListenerPostion(const T& vec3) {
		ListenerPostion(vec3.x, vec3.y, vec3.z);
	}
	template<typename T>
	void ListenerVelocity(const T& vec3) {
		ListenerVelocity(vec3.x, vec3.y, vec3.z);
	}
	template<typename T>
	void ListenerDirection(const T& vec3) {
		ListenerDire(vec3.x, vec3.y, vec3.z);
	}
	template<typename T>
	void ListenerUpVector(const T& vec3) {
		ListenerUpVector(vec3.x, vec3.y, vec3.z);
	}
	template<typename T>
	void Player3D::SetPosition(const T& vec3)const {
		SetPosition(vec3.x, vec3.y, vec3.z);
	}
	template<typename T>
	void Player3D::SetVelocity(const T& vec3) const{
		SetVelocity(vec3.x, vec3.y, vec3.z);
	}

	template<typename T>
	void Player3DCustom::SetDirection(const T& vec3)const {
		SetDirection(vec3.x, vec3.y, vec3.z);
	}

}

#endif

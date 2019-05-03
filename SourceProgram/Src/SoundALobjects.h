//@file ALobjects.h

#ifndef SOUNDALOBJECTS_H_INCLUDED
#define SOUNDALOBJECTS_H_INCLUDED
#include <al.h>
#include <alc.h>
#include <memory>
#include <string>

/**
* OpenAL�̃o�b�t�@�[�I�u�W�F�N�g�ƃ\�[�X�I�u�W�F�N�g���Ǘ�����.
* �\�[�X�͋@�\���֐����Ƃɐݒ�ł���悤�ɕύX.
*
*
**/

namespace Sound
{
	/*
	* OpenAL ��BufferID���Ǘ�.�ݒ肷��N���X
	*/
	class ALBufferID;
	using ALBufferIDPtr = std::shared_ptr<ALBufferID>;
	class ALBufferID
	{
	public:
		static ALBufferIDPtr Create();			///�|�C���^���쐬����
		static ALBufferIDPtr CreateDummy();		///�_�~�[�̃|�C���^���쐬����
		virtual bool Init(ALenum format, const ALvoid* data, ALsizei size, ALsizei freq);
		virtual bool Init(const char* filename, bool changeMono=false ,std::string warningMessage="");
		virtual ALuint Id()const { return id; }///<getter;
	private:
		/*�R�s�[�h�~*/
		virtual void Destroy();
		ALBufferID() = default;
		~ALBufferID() { Destroy(); }
		ALBufferID(const ALBufferID&) = delete;
		ALBufferID& operator=(const ALBufferID&) = delete;


		ALuint id = 0;		///BufferId
	};
	/*
	* OpenAL ��SourceID���Ǘ�.�ݒ肷��N���X.
	*/
	class ALSourceID;
	using ALSourceIDPtr = std::shared_ptr<ALSourceID>;
	class ALSourceID
	{
	public:
		enum STATE{
			e_STATE_STOP		= 0x0000,
			e_STATE_PLAY		= 0x0001,
			e_STATE_POUSE		= 0x0002,
			e_STATE_BEGANPLAY	= 0x0003,
			e_STATE_BEGANSTOP	= 0x0004,
			e_STATE_MAX			= 0x000f,
		};
		static ALSourceIDPtr Create();
		static ALSourceIDPtr CreateDummy();
		virtual void Init();
		virtual void Play(float easingTime=0.0f);
		virtual void Stop(float easingTime=0.0f);
		virtual void SetVolume(float f);
		virtual void Pouse();
		virtual bool IsActive();
		virtual void ActorId(short id);
		virtual short ActorId()const { return actorId; }
		virtual void PrintState()const;

		/*OpenAL��Source��parameter�ݒ�*/
		virtual void SetBuffer(ALuint bufferId=0)const;
		virtual void SetRelative(bool io = false)const;
		virtual void SetLooping(bool io = false)const;
		virtual void SetPitch(float pitch =1.0f)const;
		virtual void SetPosition(float x=0.0f, float y=0.0f, float z=0.0f)const;
		virtual void SetVelocity(float x = 0.0f, float y = 0.0f, float z = 0.0f)const;
		virtual void SetDirection(float x = 0.0f, float y = 0.0f, float z = 0.0f)const;
		virtual void SetReferenceDistance(float referenceDistance=1.0f)const;
		virtual void SetMaxDistance(float maxDistance =100.0f)const;
		virtual void SetConeOuterGain(float outerGain=0.0f)const;
		virtual void SetConeInnerAngle(float innerAngle=360.0f)const;
		virtual void SetConeOuterAngle(float outerAngle=360.0f)const;
		virtual void SetMinGain(float minGain=0.0f)const;
		virtual void SetMaxGain(float maxGain=1.0f)const;
		virtual void SetRollOffFactor(float rolloff=1.0f)const;

		/*���t���[�����s����X�V����*/
		virtual void Update(float);

		virtual STATE GetState()const;
		
		virtual void UnlinkActor();
		/*�����Ԃ��ƂɎ��s����X�V����*/
		virtual short ActorIdJustStopped();
	private:
		/*�R�s�[�h�~*/
		ALSourceID();
		~ALSourceID() { Destroy(); }
		ALSourceID(const ALSourceID&) = delete;
		ALSourceID& operator=(const ALSourceID&) = delete;

		virtual void SetGain(float vol = 1.0f)const;
		virtual void Destroy();
		virtual void SetState(STATE=e_STATE_MAX);

		//�f�o�b�O�p
		void GetChangedStatus();
		uint16_t lastStats = 0;
		STATE teststate=e_STATE_STOP;

		ALuint id = 0;			//SourceId;

		uint16_t state = 0;		//�X�e�[�^�X

		/*�C�[�Y�C���A�E�g�Đ����g���ꍇ�̒l*/
		float easingGain = 0.0f;
		float easingRate = 0.0f;
		float gain=0.0f;

		short actorId = -1;		//�\�[�X���g�p���Ă���Actor��id.
	};
}


#endif // !ALOBJECTS_H_INCLUDED

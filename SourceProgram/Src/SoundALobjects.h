//@file ALobjects.h

#ifndef SOUNDALOBJECTS_H_INCLUDED
#define SOUNDALOBJECTS_H_INCLUDED
#include <al.h>
#include <alc.h>
#include <memory>
#include <string>

/**
* OpenALのバッファーオブジェクトとソースオブジェクトを管理する.
* ソースは機能を関数ごとに設定できるように変更.
*
*
**/

namespace Sound
{
	/*
	* OpenAL のBufferIDを管理.設定するクラス
	*/
	class ALBufferID;
	using ALBufferIDPtr = std::shared_ptr<ALBufferID>;
	class ALBufferID
	{
	public:
		static ALBufferIDPtr Create();			///ポインタを作成する
		static ALBufferIDPtr CreateDummy();		///ダミーのポインタを作成する
		virtual bool Init(ALenum format, const ALvoid* data, ALsizei size, ALsizei freq);
		virtual bool Init(const char* filename, bool changeMono=false ,std::string warningMessage="");
		virtual ALuint Id()const { return id; }///<getter;
	private:
		/*コピー防止*/
		virtual void Destroy();
		ALBufferID() = default;
		~ALBufferID() { Destroy(); }
		ALBufferID(const ALBufferID&) = delete;
		ALBufferID& operator=(const ALBufferID&) = delete;


		ALuint id = 0;		///BufferId
	};
	/*
	* OpenAL のSourceIDを管理.設定するクラス.
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

		/*OpenALのSourceのparameter設定*/
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

		/*毎フレーム実行する更新処理*/
		virtual void Update(float);

		virtual STATE GetState()const;
		
		virtual void UnlinkActor();
		/*一定期間ごとに実行する更新処理*/
		virtual short ActorIdJustStopped();
	private:
		/*コピー防止*/
		ALSourceID();
		~ALSourceID() { Destroy(); }
		ALSourceID(const ALSourceID&) = delete;
		ALSourceID& operator=(const ALSourceID&) = delete;

		virtual void SetGain(float vol = 1.0f)const;
		virtual void Destroy();
		virtual void SetState(STATE=e_STATE_MAX);

		//デバッグ用
		void GetChangedStatus();
		uint16_t lastStats = 0;
		STATE teststate=e_STATE_STOP;

		ALuint id = 0;			//SourceId;

		uint16_t state = 0;		//ステータス

		/*イーズインアウト再生を使う場合の値*/
		float easingGain = 0.0f;
		float easingRate = 0.0f;
		float gain=0.0f;

		short actorId = -1;		//ソースを使用しているActorのid.
	};
}


#endif // !ALOBJECTS_H_INCLUDED

//SoundActor.h

#ifndef SOUNDACTOR_H_INCLUDED
#define SOUNDACTOR_H_INCLUDED
#include <unordered_map>
#include <string>
#include <iostream>
namespace Sound {
	/*Sound内で使用する三軸を持つ構造体*/
	struct Vector {
		float x;
		float y;
		float z;
	};
	//他クラスの前方宣言
	class ALBufferID;
	class ALSourceID;
	/* アクター関連のクラス定義.実態は持てない
	* 継承してエンジンクラスの一部.
	*
	*
	*/
	class ActorClasses
	{
	public:
		class Actor;
		using ActorPtr = std::shared_ptr<Actor>;
		class Actor3D;
		class Actor3DCustom;

	protected:
		//エンジンクラスのみが使用できる関数群.
		class EngineFunc
		{
		public:
			enum STATE
			{
				////1byte使って状態の定義をする/
				//e_STATE_PLAY = 0x0000,
				//e_STATE_STOP = 0x0001,
				//e_STATE_POUSE = 0x0002,
				//e_STATE_NONE = 0x0003,

				//ステートが増える場合はここに追加する.
				//e_STATE_??? = 0x000?,

				e_STATE_MAX = 0x000f,
			};
			enum STATE_BOOLIAN
			{
				//e_BOOL_ACTIVE = 0x0010,
				e_BOOL_LOOPING = 0x0020,
				e_BOOL_3D = 0x0040,
				//e_BOOL_ = 0x0060,
				//e_BOOL_ = 0x0080,
				//e_BOOL_ = 0x0100,
				//e_BOOL_ = 0x0200,
				//e_BOOL_ = 0x0400,
				//e_BOOL_ = 0x0800,
				/*値が変化したかどうかを判断する*/
				//e_BOOL_CHANGED_POSITION  = 0x1000,
				//e_BOOL_CHANGED_VELOCITY  = 0x2000,
				//e_BOOL_CHANGED_DIRECTION = 0x4000,
				//e_BOOL_CHANGED           = 0x8000,

			};


			virtual void SetFilename(std::string str) = 0;
			virtual std::string GetFilename()const=0;
			virtual void SourceId(short i) = 0;
			virtual short SourceId()const = 0;
			virtual void ResetSourceId() = 0;
			virtual void SetState(STATE) = 0;
			virtual void SetState(STATE_BOOLIAN, bool) = 0;
			virtual STATE GetState()const = 0;
			virtual bool GetStateBoolian(STATE_BOOLIAN) const = 0;

		protected:
			EngineFunc() = default;
			~EngineFunc() {}
			EngineFunc(const EngineFunc&) = delete;
			EngineFunc& operator=(const EngineFunc&) = delete;

		};

	protected:
		/*
		* エンジンのバッファーにアクセスするための関数.
		*/
		virtual std::shared_ptr<ALBufferID> GetBufferPtr(std::string)const = 0;
		virtual std::shared_ptr<ALBufferID> GetBuffer3DPtr(std::string)const = 0;
		virtual std::shared_ptr<ALSourceID> GetSourcePtr(int index)const = 0;
		virtual int SetSourceToActor(int actorId) = 0;
		virtual void ReadSoundData(std::string filename)=0;
		virtual void ReadSoundData3D(std::string filename)=0;

	};

	class ActorClasses::Actor :public ActorClasses::EngineFunc
	{
	public:
		virtual bool Init();
		virtual void Play(short actorId,float easingTime=0.0f);
		virtual void Stop(float easingTime=0.0f)const;
		virtual void Pouse()const;
		virtual void SetLooping(bool io);
		virtual void SetVolume(float volume);
		virtual void SetPitch(float pitch);
		//アクターの状態などを管理する(想定しているアクタークラスは16bit)
		virtual void PrintSourceState();

	protected:
		virtual void SetSourceParameter();
		virtual void SetSourceParameterExCeption();
		virtual void SourceId(short i) { sourceId = i; }
		virtual short SourceId()const override { return sourceId; }
		virtual void ResetSourceId() override { sourceId = -1; }
		virtual void SetFilename(std::string str) override { buffername = str; }

		virtual void SetState(STATE) override;
		virtual void SetState(STATE_BOOLIAN, bool)override;
		virtual STATE GetState()const override;
		virtual bool GetStateBoolian(STATE_BOOLIAN)const override;
		virtual std::string GetFilename()const override { return buffername; };

		Actor() = default;
		~Actor() = default;
		Actor(const Actor&) = delete;
		Actor& operator=(const Actor&) = delete;
	protected:
		float pitch = 1.0f;			//ピッチ
		float gain = 0.8f;			//音量
		std::string buffername;		//バッファーに使用するファイル名.
		short sourceId = -1;		//sourceObjectsにアクセスするid.
		uint16_t state;				//ビットで管理してる状態.
	};



	class ActorClasses::Actor3D :public ActorClasses::Actor
	{
	public:
		virtual bool Init()override;
		virtual void SetPosition(const Vector&);
		virtual void SetVelocity(const Vector&);
		virtual void SetReferenceDistance(float distance);
		virtual void MaxDistance(float distance);
		virtual void MinVolume(float volume);
		virtual void MaxVolume(float volume);

	protected:
		virtual void SetSourceParameter()override;
		virtual void SetSourceParameterExCeption()override;

		Actor3D(){}
		~Actor3D() {}
		Actor3D(const Actor3D&) = delete;
		Actor3D& operator=(const Actor3D&) = delete;

	private:
		Vector position = {0.0f,0.0f,0.0f};
		Vector velocity= { 0.0f,0.0f,0.0f };
		float minVolume=0.0f;
		float maxVolume = 1.0f;
		float maxDistance = 100.0f;
		float referenceDistance=1.0f;
	};
	class ActorClasses::Actor3DCustom :public ActorClasses::Actor3D
	{
	public:
		//virtual bool Init()override;
		virtual void SetConeInnerAngle(float angle);
		virtual void SetConeOuterAngle(float angle);
		virtual void SetConeOuterVolume(float volume);
		virtual void SetRollOffFactor(float rolloff);
		virtual void SetDirection(const Vector&);

	protected:
		virtual void SetSourceParameter()override;
		virtual void SetSourceParameterExCeption()override;

	private:
		float coneInnerAngle = 360.0f;
		float coneOuterAngle = 360.0f;
		float coneOuterVolume=0.0f;
		float rolloffFactor = 1.0f;
		Vector direction = { 0.0f,0.0f,0.0f };
	};
}


#endif // !SOUNDACTOR_H_INCLUDED

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
		static EngineRef Instance();	//シングルトンインスタンス
		static ActorClasses& ActorInstance();
		bool Init(unsigned short sourceCapacity, unsigned short actorCapacity);					//初期化
		void Update(float delta);		//常に呼べ.

		/*OpenALリスナーの設定*/
		void SetListenerPosition(float x,float y,float z)const;
		void SetListenerGain(float gain)const;
		void SetListenerVelocity(float x,float y,float z)const;
		void SetListenerDirection(float x,float y,float z);
		void SetListenerUpVector(float x,float y,float z);

		/*OpenAL全体設定*/
		void SetALSpeedOfSound(float speedOfSound)const;
		void SetALDopplerFactor(float dopplerFactor)const;
		void SetALDopplerVelocity(float dopplerVelocity)const;
		//void SetALDistanceModel();
		
		/*actorList関連*/
		template<typename T>
		bool CreateActor(short& id, std::string filename);//retval:index;
		template<typename T>
		std::shared_ptr<T> GetActorPtr(short index)const;
		void ActorDestroy(short& id);



	private:
		//生成.コピー防止
		Engine() = default;
		~Engine();
		Engine(const EngineRef) = delete;
		EngineRef operator=(const EngineRef) = delete;

	private:
		/*sourceList関連*/
		virtual std::shared_ptr<ALSourceID> GetSourcePtr(int index)const override;

		virtual std::shared_ptr<ALBufferID> GetBufferPtr(std::string)const override;
		virtual std::shared_ptr<ALBufferID> GetBuffer3DPtr(std::string)const override;
		virtual int SetSourceToActor(int actorId)override;

		/*wavDataList関連*/
		virtual void ReadSoundData(std::string filename)override;
		virtual void ReadSoundData3D(std::string filename)override;


		/*クラス内で使う機能*/
		void ErrorPrint(const char*)const;//エラーの出力
		void BufferInitialize(unsigned short sourceCapacity,unsigned short actorCapacity);
		void BufferFinalize();


		//void DisconnectActorFromSource(int sourceId);//ソースからアクターを切り離す.
		/*初期化フラッグ*/
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

		/*Update()でソースが鳴り終わってるかどうかを確認する周期*/
		float checkSourcesStateTime = 1.0f;
		float checkSourcesStateTimer = 0.0f;

		/*Listener関連*/
		Vector listenerDirection = { 0.0, 0.0, -1.0};
		Vector listenerUpVector = { 0.0, 1.0, 0.0 };

		/*全体ボリューム*/

		//オブジェクトのバッファー
		std::unordered_map<std::string, std::shared_ptr<ALBufferID>> soundDataList;
		std::unordered_map<std::string, std::shared_ptr<ALBufferID>> soundData3DList;
		std::vector <std::shared_ptr<ALSourceID>> sourceList;
		std::vector<std::shared_ptr<EngineFunc>> actorList;

		//サウンドデータリスト(soundDateListの所有者数を管理する)
		std::unordered_map<std::string,int>soundDataCounter;
		std::unordered_map<std::string,int>soundData3DCounter;

		//アクターのダミーポインタを返却するためのクラス
		class DummyActor;
	};



	/*GetActorPtrで不正なidでアクセスした際に返すアクターのダミークラス
	* ポインタからアクセスできるであろうパブリック関数は虱潰しで空の関数に
	* オーバーライドする.
	*/
	class Engine::DummyActor :/*public Actor, public Actor3D, */public Actor3DCustom
	{
	public:
		/*Actor(親クラス)*/
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

		/*エラー文*/
		void WarningCall()const { std::cerr << "<警告>DummyActorポインタの関数を使用しています" << std::endl; }

		DummyActor() {}
		~DummyActor() {}
	};


	/* @ObjectBuffer
	* 新しいActorを作成する.
	*
	* @param id			アクターのアクセスキーを受け取る変数
	* @param filename	使用するwavファイル名
	*
	* @retval	:true 成功 :false 失敗
	*
	* @note アクターIDを既に所持している場合は失敗する.
	*/
	template<typename T>
	bool Engine::CreateActor(short& id,std::string filename)
	{
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE)) {
			ErrorPrint("CreateActor():エンジンの初期化が完了していません");
			return false;
		}
		//既にキーを所持している場合は失敗
		if (id >= 0) {
			ErrorPrint("CreateActoro():既にアクターIDを所持している");
			return false;
		}
		//ActorListから空いている要素があるかを検索.
		for (int i = 0; i < actorList.size(); ++i) {
			if (!actorList[i]) {//空き要素が見つかった場合は新しく要素を作成し添え字を返す
				struct Impl:public T{};
				actorList[i] = std::make_shared<Impl>();
				actorList[i]->SetFilename(filename);
				id = i;
				return true;
			}
		}
		//開いている要素が見つからなかった場合はエラーを吐いて-1を返す.
		ErrorPrint("CreateActor():ActorListの空きがありません.");
		return false;
	}


	/* Actorのポインタを取得する.
	* 
	* @param index:アクターにアクセスする鍵(配列の添え字)
	*
	* @note 不正なindexだった場合はダミーポインタを返す
	*		ダミークラスはアクセスしてもエラーは出ないが何もしない
	*/
	template<typename T>
	std::shared_ptr<T> Engine::GetActorPtr(short index)const
	{
		//エンジンの初期化が完了しているか.
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE)) {
			ErrorPrint("GetActorPtr():エンジンの初期化が完了していません.ダミーを返します");
			return std::make_shared<DummyActor>();
		}
		//範囲外の添え字が飛んできた場合はエラーと一緒にnullptrを返す.
		if (index < 0 || index >= actorList.size()) {
			ErrorPrint("GetActorPtr():不正なindexを受け取りました.ダミーを返します");
			if (index < 0) {
				std::cerr << "オブジェクトが作成されていません" << std::endl;
			}
			return std::make_shared<DummyActor>();
		}
		//要素のポインタのnullcheck
		if (!actorList[index]) {
			ErrorPrint("GetActorPtr():Actorが存在しません.ダミーを返します");
			return std::make_shared<DummyActor>();
		}

		std::shared_ptr<T>successCast = std::dynamic_pointer_cast<T>(actorList[index]);
		//ダウンキャストに失敗した場合
		if (!successCast) {
			ErrorPrint("GetActorPtr():ダウンキャストに失敗しました,生成したActorh派生型と一致してるかを確認してください");
			return std::make_shared<DummyActor>();
		}

		return successCast;


	}

}

#endif // !SOUNDBUFFER_H_INCLUDED

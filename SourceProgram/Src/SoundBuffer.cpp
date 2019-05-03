#include "SoundBuffer.h"
#include "SoundALobjects.h"
#include <iostream>


namespace Sound
{

	/* ObjectBuffer
	* 指定したIdのアクターをバッファから削除する.
	*
	*/
	void Engine::ActorDestroy(short& index)
	{
		//無効なIDでないかのチェック
		if (index < 0 || index >= actorList.size()) {
			ErrorPrint("ActorDestroy():不正なインデックスを受け取りました");
			return;
		}
		//アクセス先のActorが存在するか調べる.
		if (!actorList[index]) {
			ErrorPrint("ActorDestroy():存在しないActorを削除しようとしました");
			return;
		}
		//アクターがソースを所持している場合はソースとのリンクを切り離す.
		short sourceId = actorList[index]->SourceId();
		if (sourceId >= 0) {
			sourceList[sourceId]->UnlinkActor();
		}
		std::string destroyFilename = actorList[index]->GetFilename();
		//アクターの属性を確認.所持者が0になった音声データを捨てる
		//std::cout << destroyFilename << std::endl;
		if (actorList[index]->GetStateBoolian(EngineFunc::e_BOOL_3D)) {
			if (soundData3DCounter.count(destroyFilename)) {
				soundData3DCounter.at(destroyFilename) -= 1;
				//std::cout << "デバッグ;ファイル所有者を表示" << soundData3DCounter.at(destroyFilename)<< std::endl;
				if (soundData3DCounter.at(destroyFilename) <= 0) {
					soundData3DList.erase(destroyFilename);
				}
			}
		}
		else {
			if (soundDataCounter.count(destroyFilename)) {
				soundDataCounter.at(destroyFilename) -= 1;
				//std::cout << "デバッグ;ファイル所有者を表示" << soundDataCounter.at(destroyFilename) << std::endl;
				if (soundDataCounter.at(destroyFilename) <= 0) {
					soundDataList.erase(destroyFilename);
				}
			}
		}

		//アクターを削除する.
		actorList[index].reset();
		//インデックスも空に変更する.
		index = -1;
	}


	/*ObjectBuffer
	*
	* ファイルを読み込んでバッファーを作成する
	*/
	void Engine::ReadSoundData(std::string filename)
	{
		//既に読み込んでいる場合カウンターのみ増やす
		if (soundDataList.count(filename)) {
			++soundDataCounter.at(filename);
			return;
		}
		soundDataList.insert({ filename,ALBufferID::Create() });
		if (!soundDataList.at(filename)->Init(filename.c_str())) {
			//バッファーの作成に失敗した場合は要素を削除してエラーを吐く
			ErrorPrint("ReadWavData():バッファの作成に失敗しました.");
			soundDataList.erase(filename);
			return;
		}
		else {
			//成功した場合はカウンターも作成する.
			soundDataCounter.insert({ filename,1 });
			return;
		}

	}
	void Engine::ReadSoundData3D(std::string filename)
	{
		//既にファイルを読み込んでいる場合はカウンターだけ上げる
		if (soundData3DList.count(filename)) {
			++soundData3DCounter.at(filename);
			return;
		}
		soundData3DList.insert({ filename,ALBufferID::Create() });
		//std::string warningMessage = "<警告>Engine::ReadSoundData3D():読み込んだファイルがステレオだったためモノラルに変換しました\n\
			3Dサウンドとして使用するファイルは可能であればモノラルで用意してください";
		if (!soundData3DList.at(filename)->Init(filename.c_str(), true)) {
			ErrorPrint("ReadSoundData3D():バッファの作成に失敗しました");
			soundData3DList.erase(filename);
			return;
		}
		else {
			//成功した場合はカウンターも作成する.
			soundData3DCounter.insert({ filename,1 });
			return;
		}
	}


	/* @BufferObject
	* アクターにソースをセットする.
	*
	* @param actorId : アクター側の配列要素数
	*
	* @retrun : ソースのID.
	* @note : 所持者がいないソースを優先的に取得.全てのソースがセットされている場合は非アクティブなものを取得する.
	*			ソースが取得できなかった場合は -1が返ってくる.
	*/
	int Engine::SetSourceToActor(int actorId)
	{
		for (int i = 0; i < sourceList.size(); ++i) {
			if (sourceList[i]->ActorId() < 0) {
				sourceList[i]->ActorId(actorId);
				return i;
			}
		}
		for (int i = 0; i < sourceList.size(); ++i) {
			if (!sourceList[i]->IsActive()) {
				int sourceID = actorList[sourceList[i]->ActorId()]->SourceId();
				actorList[sourceList[i]->ActorId()]->ResetSourceId();
				actorList[actorId]->SourceId(sourceID);
				sourceList[i]->UnlinkActor();
				sourceList[i]->ActorId(actorId);
				return i;
			}
		}

		//ソースを取得できなかった場合はエラーをはいて.-1を返す
		ErrorPrint("ALsourceIdを取得できませんでした");
		return -1;
	}

	/*Buffer Object
	* ソースを取得する.
	*/
	std::shared_ptr<ALSourceID> Engine::GetSourcePtr(int index)const
	{
		//範囲外の添え字が飛んできた場合はエラーと一緒にnullptrを返す.
		if (index < 0 || index >= sourceList.size()) {
			ErrorPrint("GetSourcePtr():不正なindexを受け取りました.ダミーを返します");
			return ALSourceID::CreateDummy();
		}

		return sourceList[index];
	}
	/*Buffer Object
	* バッファーを取得する.
	*/
	std::shared_ptr<ALBufferID> Engine::GetBufferPtr(std::string filename)const
	{
		//要素が存在するかチェック
		if (!soundDataList.count(filename))
		{
			//見つからなかった場合はエラーと空のポインタを返す.
			ErrorPrint("GetBufferPtr():バッファーの取得に失敗しました.ダミーを返します");
			return ALBufferID::CreateDummy();
		}
		//連想配列からバッファポインタを返す.
		return soundDataList.at(filename);
	}



	/*Buffer Object
	* バッファーを取得する.
	*/
	std::shared_ptr<ALBufferID> Engine::GetBuffer3DPtr(std::string filename)const
	{
		//要素が存在するかチェック
		if (!soundData3DList.count(filename))
		{
			//見つからなかった場合はエラーと空のポインタを返す.
			ErrorPrint("GetBufferPtr():バッファーの取得に失敗しました.ダミーを返します");
			return ALBufferID::CreateDummy();
		}
		//連想配列からバッファポインタを返す.
		return soundData3DList.at(filename);
	}




	/* @Engine
	* エラーの定型文
	*
	* @param message :不具合の内容.
	*/
	void Engine::ErrorPrint(const char* message) const {
		std::cerr << "ERROR>>Sound::Engine::" << message << std::endl;
	}



	/*
	* シングルトンインスタンスの生成
	*/
	EngineRef Engine::Instance()
	{
		static Engine instance;
		return instance;
	}

	/*
	* インスタンスをアクターにキャストしただけ
	*/
	ActorClasses& Engine::ActorInstance()
	{
		return Instance();
	}

	/*
	* 初期化
	*
	* @param sourceCapacity : ソースの数(同時にいくつ再生できるか)
	* @param actorCapacity  : アクターの数
	*
	* @return true < success
	* @return false < failure
	*
	* @note :念のため 大きすぎる数を入れれないようにしている source:64 actor:512 まで
	*/
	bool Engine::Init(unsigned short sourceCapacity, unsigned short actorCapacity)
	{

		if (flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE)) {
			std::cout << "Sound::Engineはすでに初期化されています." << std::endl;
		}

		const unsigned short maxSourceCapacity = 64;
		const unsigned short maxActorCapacity = 512;
		if (sourceCapacity > maxSourceCapacity) {
			ErrorPrint("BufferInitialize():作成するSourceの数が多すぎます");
			std::cerr << "最大数:" << maxSourceCapacity << std::endl;
			return false;
		}
		if (actorCapacity > maxActorCapacity) {
			ErrorPrint("BufferInitialize():作成するSourceの数が多すぎます");
			std::cerr << "最大数:" << maxSourceCapacity << std::endl;
			return false;
		}


		/*
		* デバイスを取得
		*
		*/
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_DEVICE)) {
			device = alcOpenDevice(NULL);//const ALCchar *devicename
			if (device == NULL)
			{
				ErrorPrint("Init():デバイスの取得に失敗しました.");
				return false;
			}
			flagList.Set(flagList.e_BOOL_INITIALIZED_DEVICE, true);
		}

		/*
		* コンテキストの作成
		*/
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_CONTEXT)) {
			context = alcCreateContext(
				device,				   //ALCdevice *device, 
				NULL);				   //const ALCint* attrlist
			if (context == NULL) {
				ErrorPrint("Init():コンテキストの作成の失敗しました.");
				return false;
			}
			//コンテキストのセット
			alcMakeContextCurrent(context);//ALCcontext *context

			flagList.Set(flagList.e_BOOL_INITIALIZED_CONTEXT, true);
		}

		//バッファーの容量を確保する.
		BufferInitialize(sourceCapacity, actorCapacity);


		flagList.Set(flagList.e_BOOL_INITIALIZED_ENGINE, true);
		return true;
	}

	void Engine::Update(float delta) {
		//バッファーの更新
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE)) {
			ErrorPrint("Update():エンジンの初期化が完了していません");
		}

		//毎フレームのソースの更新
		for (size_t i = 0; i < sourceList.size(); ++i) {

			sourceList[i]->Update(delta);
			////ソースを所持しているアクターにも更新をかける
			//short actorid = sourceList[i]->ActorId();
			//if (actorid >= 0) {
			//	actorList[actorid]->Update(delta);
			//}
		}


		checkSourcesStateTimer -= delta;
		/*一定更新タイマーの処理*/
		if (checkSourcesStateTimer <= 0) {
			//もしタイマーが0になったら.ソースの定期更新
			for (size_t i = 0; i < sourceList.size(); ++i) {
				sourceList[i]->ActorIdJustStopped();
			}


			checkSourcesStateTimer = checkSourcesStateTime - checkSourcesStateTimer;
		}
		ALenum errortype = alGetError();
		if (errortype != AL_NO_ERROR) {
			//std::cout << "エラー発生OpenAL>>内容を下記に表示" << std::endl;
			switch (errortype)
			{
			case AL_INVALID_NAME:
				std::cerr << "name" << std::endl;
				break;
			case AL_INVALID_ENUM:
				std::cerr << "enum" << std::endl;
				break;
			case AL_INVALID_VALUE:
				//std::cerr << "value" << std::endl;
				break;
			case AL_INVALID_OPERATION:
				std::cerr << "operation" << std::endl;
				break;
			case AL_OUT_OF_MEMORY:
				std::cerr << "out of memory" << std::endl;
				break;
			default:
				std::cerr << "相当無し" << std::endl;
				break;
			}
		}
		if (alcGetError(device) != ALC_NO_ERROR) {
			//ALC_NO_ERROR	現在エラーはありません
			//ALC_INVALID_DEVICE	デバイスハンドラやその指定にはアクセス可能なドライバかサーバを指定してください。（訳注：原文では"The device handle or specifier names an accessible driver/server."『〜〜〜はアクセス可能なドライバまたはサーバを指定しています』という形になっていますが、エラーの意味からこのように訳しました。）
			//ALC_INVALID_CONTEXT	コンテキスト引数は有効なコンテキストを指していません。
			//ALC_INVALID_ENUM	使われているトークンは無効か、適していません。
			//ALC_OUT_OF_MEMORY	メモリ割り当てが出来ません。

			std::cout << "エラー発生ALC" << std::endl;
		}

		//リスナーのAL_OPERATIONに関してだけは同時に設定するので毎フレームbool型を確認,変化があれば更新
		if (flagList.Get(flagList.e_BOOL_CHANGED_LISTENER_DELECTION)) {
			const float vec[6] = { listenerDirection.x,listenerDirection.y,listenerDirection.z,
									listenerUpVector.x,listenerUpVector.y,listenerUpVector.z };
			alListenerfv(AL_ORIENTATION, vec);
			flagList.Set(flagList.e_BOOL_CHANGED_LISTENER_DELECTION, false);
		}
	}

	/*エンジンでやるのもどうかとおもったが,リスナー関係
	*
	*/
	void Engine::SetListenerPosition(float x, float y, float z)const {
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE))
		{
			ErrorPrint("SetListenerPosition():エンジンの初期化が完了していません");
			return;
		}
		alListener3f(AL_POSITION, x, y, z);
	}
	void Engine::SetListenerGain(float gain) const {
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE))
		{
			ErrorPrint("SetListenerGain():エンジンの初期化が完了していません");
			return;
		}
		alListenerf(AL_GAIN, gain);
	}
	void Engine::SetListenerVelocity(float x, float y, float z) const {
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE))
		{
			ErrorPrint("SetListenerVelocity():エンジンの初期化が完了していません");
			return;
		}
		alListener3f(AL_VELOCITY, x, y, z);
	}
	void Engine::SetListenerDirection(float x, float y, float z) {
		listenerDirection = { x, y, z };
		flagList.Set(flagList.e_BOOL_CHANGED_LISTENER_DELECTION, true);
	}
	void Engine::SetListenerUpVector(float x, float y, float z) {
		listenerUpVector = { x, y, z };
		flagList.Set(flagList.e_BOOL_CHANGED_LISTENER_DELECTION, true);
	}

	void Engine::SetALSpeedOfSound(float speedOfSound)const
	{
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE))
		{
			ErrorPrint("SetALSpeedOfSound():エンジンの初期化が完了していません");
			return;
		}
		alSpeedOfSound(speedOfSound);

	}
	void Engine::SetALDopplerFactor(float dopplerFactor)const
	{
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE))
		{
			ErrorPrint("SetDopplerFactor():エンジンの初期化が完了していません");
			return;
		}
		alDopplerFactor(dopplerFactor);
	}
	void Engine::SetALDopplerVelocity(float dopplerVelocity)const
	{
		if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE))
		{
			ErrorPrint("SetDopplerVelocity():エンジンの初期化が完了していません");
			return;
		}
		alDopplerVelocity(dopplerVelocity);
	}
	//void Engine::SetALDistanceModel()
	//{
	//	if (!flagList.Get(flagList.e_BOOL_INITIALIZED_ENGINE))
	//	{
	//		ErrorPrint("SetListenerPosition():エンジンの初期化が完了していません");
	//		return;
	//	}
	//	alDistanceModel();
	//}


	/*
	* Destractor
	*/
	Engine::~Engine()
	{
		//バッファーの後始末.
		BufferFinalize();

		//コンテキストの破棄
		if (flagList.Get(flagList.e_BOOL_INITIALIZED_CONTEXT)) {
			alcMakeContextCurrent(NULL);
			alcDestroyContext(context);
		}
		//デバイスの破棄
		if (flagList.Get(flagList.e_BOOL_INITIALIZED_DEVICE)) {
			alcCloseDevice(device);
		}

	}
	/* @Engine
	*
	* vector配列のサイズを設定する.
	*/
	void Engine::BufferInitialize(unsigned short sourceCapacity, unsigned short actorCapacity)
	{
		sourceList.resize(sourceCapacity);
		for (int i = 0; i < sourceList.size(); ++i) {
			sourceList[i] = ALSourceID::Create();
		}
		actorList.resize(actorCapacity);

	}
	/*
	* バッファーを削除する.
	*/
	void Engine::BufferFinalize()
	{
		actorList.clear();
		sourceList.clear();
		soundDataList.clear();
		soundData3DList.clear();
	}



}



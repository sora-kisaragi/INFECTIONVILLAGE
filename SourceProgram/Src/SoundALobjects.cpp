#include "SoundALobjects.h"
#include "FileReader_Wav.h"
#include <iostream>

namespace Sound {
	/*
	* ポインタの作成
	*/
	ALBufferIDPtr ALBufferID::Create()
	{
		struct  Impl :public ALBufferID {};
		ALBufferIDPtr p = std::make_shared<Impl>();
		return p;
	}
	/*
	* ダミーを作成する
	*/
	ALBufferIDPtr ALBufferID::CreateDummy()
	{
		struct Dummy :public ALBufferID
		{
			virtual bool Init(ALenum format, const ALvoid* data, ALsizei size, ALsizei freq)override { PrintWarning(); return false; };
			virtual bool Init(const char* filename, bool, std::string) override { PrintWarning(); return false; };
			virtual ALuint Id()const override { return id; }///<getter;


			void PrintWarning()const { std::cerr << "<警告>ALBufferIDのダミーの関数を使用しています" << std::endl; }
		};
		ALBufferIDPtr p = std::make_shared<Dummy>();

		return p;
	}

	/*
	* バッファオブジェクトの作成.
	*
	* @param format		 //<ex AL_FORMAT_MONO8,モノラル bitrate 8bit
	* @param data		 //<送信するデータのオフセット
	* @param size 		 //<データのサイズ(byte単位)
	* @param freq		 //<サンプリング周波数 (frequency=周波数)
	*
	* @retval true<成功 false <失敗.
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
	* ファイルから作成.
	*
	* @param filename :ファイルの文字列
	*
	* @return true<成功 : false<失敗
	* @note wavのみ対応.
	*/
	bool ALBufferID::Init(const char* filename, bool chengeMono, std::string warningMessage)
	{
		//一時的にデータを保持するクラス.
		FileReader::Wave waveReader;
		if (!waveReader.ReadFile(filename)) {
			std::cerr << "Sound::BufferObject::Init<<ファイルの読み込みに失敗しました" << std::endl;
			return false;
		}
		//もしもモノラル変換する場合
		if (chengeMono) {
			if (waveReader.ComvertToMono()) {
				//変換した場合はアプリケーションの変換負担がかかっているので警告する
				std::cerr << warningMessage << std::endl;
			}
		}
		waveReader.PrintData();
		//チャンネルとビットレートの取得
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
			std::cerr << "Sound::BufferObject::Init<<フォーマットが取得できませんでした" << std::endl;
			return false;
			break;
		}


		return Init(format, waveReader.GetData().data(), static_cast<ALsizei>(waveReader.GetSize()), waveReader.GetSampleRate());
	}

	/*
	* バッファオブジェクトの破棄
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
	* 主に今のところはソースのidを保持するだけ.
	* とりあえずデリート忘れを嫌っているだけなので今後改善出来たらしたい.
	*
	*
	*
	*====================================================================================================
	*/

	/*
	* オブジェクトのを作成する関数
	*/
	ALSourceIDPtr ALSourceID::Create()
	{
		struct Impl :public ALSourceID {};
		ALSourceIDPtr p = std::make_shared<Impl>();
		return p;
	}
	/*
	* オブジェクトのダミーを作成する
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

			void PrintWarning()const { std::cerr << "<警告>ALSourceDummyにアクセスしています" << std::endl; }
		};
		ALSourceIDPtr p = std::make_shared<Dummy>();
		return p;
	}

	/*SoundObject
	* コンストラクタでalGenもどうなのかと思ったけど.
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


	/*アクターとリンクしている場合,アクターのidを初期化してデフォルトに戻す
	*
	* 関数の数がやりすぎ太郎でちょっと困った,8)
	*/
	void ALSourceID::UnlinkActor()
	{
		//全ての値をOpenALのデフォルトに戻す
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

		//アクターのIDを破棄する.
		actorId = -1;
	}

	/*
	* OpenALのSource()関数を個別に全て関数にまとめた,
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
	* 再生する.
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
	/*ALSource 停止する
	*
	*/
	void ALSourceID::Stop(float easingTime)
	{
		if (easingTime > 0.0f) {

			SetState(e_STATE_BEGANSTOP);
		}
		else {
			//即ストップ
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
	* 一時停止
	*/
	void ALSourceID::Pouse()
	{
		SetState(e_STATE_POUSE);
		alSourcePause(id);
	}

	/*
	* 再生している状態ならtrue
	*/
	bool ALSourceID::IsActive()
	{
		return !((GetState() == e_STATE_STOP) || (GetState() == e_STATE_POUSE));
	}

	/*
	* アクターのidを設定する
	*
	*/
	void ALSourceID::ActorId(short index) {
		//アクターのidを所持していた場合は値をリセット
		if (actorId >= 0) {
			UnlinkActor();
		}
		actorId = index;

	}

	/*
	* ソースオブジェクトを破棄する.
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
			std::cout << "ALステートを表示initial" << std::endl;
			break;
		case AL_PLAYING:
			std::cout << "ステートを表示:playng" << std::endl;

			break;
		case AL_PAUSED:
			std::cout << "ステートを表示:ポーズ" << std::endl;
			break;
		case AL_STOPPED:
			std::cout << "ステートを表示:ストップ" << std::endl;

			break;
		default:
			std::cout << "ステートを表示:相当無し" << std::endl;
			break;

		}
	}

	/*
	* アップデート
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
		//止まっているなら調べる必要がない
		if (GetState() == e_STATE_STOP) {
			return -1;
		}
		//状態を取得する		
		ALenum alSourceState = 0;
		alGetSourcei(id, AL_SOURCE_STATE, &alSourceState);
		//取得した時にストップだった場合.
		if (alSourceState == AL_STOPPED) {
			SetState(e_STATE_STOP);
			return actorId;
		}
		else {
			return -1;
		}

	}


	/*
	* 完全デバッグ用関数
	*
	* 前回とステータスが変化した時に
	*
	*/
	void ALSourceID::GetChangedStatus()
	{
		if (!(teststate == GetState()))
		{
			std::cout << "前回とステートが変わりました:前回のステートを表示" << std::endl;
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
				std::cout << "全く別物" << std::endl;
				break;
			}
			std::cout << "今回のステートを表示" << std::endl;
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
				std::cout << "全く別物" << std::endl;
				break;
			}
			teststate = GetState();
		}
	}
}

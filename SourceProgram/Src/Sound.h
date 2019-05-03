// @file Sound.h

#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED
namespace Sound
{
	/* オーディオの全体処理に必要な関数.
	* @EngineInitialize :初期化関数.必ず始めに使用するべし 
	* @EngineUpdate		:更新関数.毎フレームに一回呼ぶべし
	*/
	bool EngineInitialize(unsigned short sourceCapacity=32,unsigned short actorCapacity=256);
	void EngineUpdate(float deltaTime);

	/* リスナー関連(聴き手の位置や向き等の情報)
	* @Position		:位置を設定する(位置ベクトル)
	* @Direction	:向いている方向を設定する(単位ベクトル)
	* @UpVector		:上方向をせっていする(単位ベクトル)
	* @Velocity		:速度を設定する(速度ベクター)
	* @Volume		:音量を設定する(マスターボリューム)
	*
	*/
	void ListenerPostion(float x, float y, float z);
	void ListenerDirection(float x, float y, float z);
	void ListenerUpVector(float x,float y,float z);
	void ListenerVelocity(float x, float y, float z);	
	void ListenerVolume(float volume);
	void ListenerPrint();

	/*全体の設定(上級向け)
	* @SpeedOfSound			:音速を設定する
	* @DopplerFactor		:よくわからないシリーズ①
	* @DopperVelocity		:よくわからないシリーズ②
	* @WorldDistanceModel	:未実装<<どうしても使いたかったらお問い合わせください<マイケル
	*/
	void WorldSpeedOfSound(float speedObSound);
	void WorldDopplerFactor(float dopplerFactor);
	void WorldDopplerVelocity(float velocity);
	//void WorldDistanceModel(enum modle);//未実装

	/*
	* サウンドプレイヤー(基底クラス).
	* bool Init		:初期化
	* void Play		:音を再生
	* void Stop		:再生中の音を停止
	* void Pause	:音を止める(一時停止:止めたところから次回再生)
	* void Looping	:ループの設定on/off
	* void Volume	:音量の調整(0.0f~0.1f)
	* void Reset	:後始末
	* void DebugSourcePrint	:デバッグ用状態の表示
	*
	* @note クラスの生成は Engineのインスタンスからのみ可能
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
		/*コピー禁止*/
		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;

	protected:
		short actorId = -1;//バッファーにアクセスする鍵.
	};

	/*
	* 3DPlayerクラス (Playerの関数は割愛)
	*
	* void Position()			:3D空間上の座標
	* void Velocity()			:速度の設定(ドップラー効果に使用される)
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

		/*glm::vec3に対応してるよ:)*/
		template<typename T>
		void SetPosition(const T& vec3)const;
		template<typename T>
		void SetVelocity(const T& vec3)const;
	};


	/* 3Dサウンドカスタム(上級者向け)
	* ※上四つはセット項目みたいなもの
	* void Direction()		:向き
	* void ConeInnerAngle()	:向きによる音の減衰が適応されない円錐
	* void ConeOuterAngle()	:インナーとの間で徐々に減衰していく
	* void ConeOuterVolume():アウター円錐の外に出たときの音量
	* void RollOffFactor()	:距離による減衰の乗数
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

		/*glm::vec3に対応してるよ:)*/
		template<typename T>
		void SetDirection(const T& vec3)const;

	};


	/*
	* glm::vec3を意識した関数
	*  x, y ,z の順番でfloat 配列に格納する.
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

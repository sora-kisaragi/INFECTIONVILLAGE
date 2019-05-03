// @file Sound.cpp


#include "Sound.h"
#include "SoundBuffer.h"
#include <al.h>
#include <alc.h>
#include <vector>
#include <iostream>

namespace Sound
{


	/* @Engine
	*============================================================
	* 全体処理をするのに必要な関数群
	*
	* 
	*
	*
	*=============================================================
	*/

	/* Initialize()
	* エンジンの初期化:
	*
	* @return	true 成功 : false 失敗.
	*
	* @note		必ず最初に初期化してください
	*			初期化されていない場合.全ての関数は無効です.
	*/
	bool EngineInitialize(unsigned short sourceCapacity, unsigned short actorCapacity)
	{
		return Engine::Instance().Init(sourceCapacity, actorCapacity);
	}

	/* Update()
	* エンジンのアップデート(毎フレーム)
	*
	* @param deltaTime:	デルタタイム(前回からの更新時間)
	*
	* @note		主にバッファーの更新を行います
	*			ゲームループでフレーム毎に読んでください
	*/
	void EngineUpdate(float deltaTime)
	{
		Engine::Instance().Update(deltaTime);
	}



	/* @Listener
	*============================================================
	* 音を感知する位置情報.
	* 3Dサウンドを利用する場合は必須項目になります
	*
	*
	*
	*=============================================================
	*/

	/* @Listener
	*
	* リスナーの位置を変更する.
	*
	* @param x,y,z :リスナーの空間座標(位置ベクトル)
	*				デフォルト値{0.0f,0.0f,0.0f}
	*
	* @note 3Dオブジェクトを利用する場合,リスナーの位置,向き,
	*		3Dオブジェクトの座標などで,聞こえる方向や大きさが計算される
	*/
	void ListenerPostion(float x, float y, float z)
	{
		Engine::Instance().SetListenerPosition(x, y, z);
	}

	/* @Listener
	*
	* リスナーの速度を設定する.
	*
	* @param x,y,z :リスナーの速度ベクトル
	*				デフォルト値{0.0f,0.0f,0.0f}
	*
	* @note 実際に動くわけではなくドップラー効果の計算に使わる値(紛らわしい)
	*/
	void ListenerVelocity(float x, float y, float z) {
		Engine::Instance().SetListenerVelocity(x, y, z);
	}

	/* @Listener
	*
	* リスナーの向きベクトルを設定する
	*
	* @param x,y,z :方向ベクトル(単位ベクトル)
	*				デフォルト値{0.0f,0.0f-1.0f}
	*
	* @note 3Dオブジェクトを利用する場合,リスナーの位置,向き,
	*		3Dオブジェクトの座標などで,聞こえる方向や大きさが計算される
	*/
	void ListenerDirection(float x, float y, float z) {
		Engine::Instance().SetListenerDirection(x, y, z);
	}

	/* @Listener
	*
	* リスナーの上方向ベクトルを設定する
	*
	* @param x,y,z :方向ベクトル(単位ベクトル)
	*				デフォルト値{0.0f,1.0f,0.0f}
	*
	* @note 3Dオブジェクトを利用する場合,(略)...上参照
	*/
	void ListenerUpVector(float x, float y, float z) {
		Engine::Instance().SetListenerUpVector(x, y, z);
	}
	void ListenerPrint() {
		float pos[3] = { 0.0f,0.0f,0.0f };
		alGetListenerfv(AL_POSITION, pos);
		std::cout << "x:" << pos[0] << " y:" << pos[1] << " z<:" << pos[2] << std::endl;
		float velo[3] = { 0.0f,0.0f,0.0f };
		alGetListenerfv(AL_VELOCITY, velo);
		std::cout << "x:" << velo[0] << " y:" << velo[1] << " z<:" << velo[2] << std::endl;

	}
	/* @Listener
	*
	* マスター音量と思ってよい,
	*
	* @param volume	:音量[0.0 ~ 1.0f]
	*				 デフォルト値{1.0f}
	*
	* @note Playerクラスで個々の音量を調節することも可能
	*/
	void ListenerVolume(float volume) {
		Engine::Instance().SetListenerGain(volume);
	}

	/* @World
	*
	* 音速を設定する.
	*
	* @param speedOfSound : 音の速さ[0.0001~]
	*						デフォルト値{ 343.3}
	*
	* @note ドップラー効果に影響がある値
	*/
	void WorldSpeedOfSound(float speedOfSound)
	{
		Engine::Instance().SetALSpeedOfSound(speedOfSound);
	}
	/* @World
	* 多分全てのオブジェクトのvelocityのスケールだと思われ(予想)
	* 
	* @param dopperFactor :　[0.0f ~]
	*					   デフォルト値{1.0f}
	*
	* @note よくわかってない.勉強不足
	* --以下公式から和訳して引用--
	* ソースとリスナーの速度を調整します。
	*/
	void WorldDopplerFactor(float dopplerFactor)
	{
		Engine::Instance().SetALDopplerFactor(dopplerFactor);
	}
	/* @World
	* 多分全てのオブジェクトのvelocityの乗数だと思われ(予想)
	*
	* @param velocity:リファレンスにもRangeが乗ってない...
	*				  デフォルト値{不明}
	*
	* @note よくわかってない.勉強不足
	* --以下公式から和訳して引用--
	* 音速に適用される乗数。
	*/
	void WorldDopplerVelocity(float velocity)
	{
		Engine::Instance().SetALDopplerVelocity(velocity);
	}

	/* @World
	* ディスタンスモデルなるものがあるらしい.
	* どうしても実装したかったらお問い合わせください...
	*
	*
	*
	*/
	//void WorldDistanceModel(enum modle) {
	//	Engine::Instance().SetALDistanceModel()
	//}





	/* @Player
	*============================================================
	* 固定音源,
	* BGMやUIのSE等や
	* また2Dゲームで使用するならこれで十分だろう
	*
	* 再生,停止,一時停止等,基本的な機能を持つ
	*=============================================================
	*/

	/* @Player
	* Init関数,バッファーにアクターオブジェクトを作成する.
	*
	* @param filename: waveファイル(.wav)
	*
	* @note	waveファイル(ステレオorモノラル)のみ受け付ける
	*		Initしないと他のメンバ関数が使用できない.
	*/
	bool Player::Init(const char* filename)
	{
		Reset();
		Engine::Instance().CreateActor<ActorClasses::Actor>(actorId, filename);
		if (actorId < 0) {
			ErrorPrint("Init():Actorの生成に失敗しました");
			return false;
		}
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Init();


		return true;
	}

	/* @Player
	*
	* 音を再生する
	*
	* @param timeToGrow : 音が徐々に大きくなる時間(秒)
	*
	* @note : 引数を設定すると徐々に大きくなって時間が経つと本来の音量になる
	*/
	void Player::Play(float timeToGrow)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Play(actorId, timeToGrow);
	}
	/* @Player
	* 再生中であれば停止する.
	*
	* @param timeToStop :音が止まるまでの時間(秒)
	*
	* @note 引数を設定すると音が止まるまで徐々に減衰していく
	*/
	void Player::Stop(float timeToStop)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Stop(timeToStop);
	}
	/*@Player
	*
	* 音を一時停止する.次回再生の際は途中から再生される
	*
	* @note 乱用するとソースが足りなくなるので 途中で止める必要のないものはStopで止めよう,
	*
	*/
	void Player::Pouse()const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->Pouse();
	}

	/* @Player
	*
	* ループを設定する
	*
	* @param io :  true < ループオン : false < ループオフ
	*				デフォルト値 false
	*/
	void Player::SetLooping(bool io)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->SetLooping(io);
	}
	/* @Player
	*
	* 音量を設定する
	*
	* @param volume:設定する音量(0.0f~0.1f)
	*				デフォルト値(0.8f)
	*
	* @note 本来のoenal側のデフォルトは1.0fなのだが,爆音である
	*/
	void Player::SetVolume(float volume)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr = Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId);
		actorPtr->SetVolume(volume);
	}
	/* @Player
	*
	* ピッチを設定する
	*
	* @param pitch :設定するピッチ(0.0f~)
	*				デフォルト値(1.0f);
	*
	* @note 半分にすれば1オクターブ下がる,二倍にすれば1オクターブ上がる,以上
	*/
	void Player::SetPitch(float pitch)const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr = Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId);
		actorPtr->SetPitch(pitch);
	}

	/* @Plyaer(基底クラス)
	* デストラクタ<<バッファから対象のアクターを削除する.
	*/
	Player::~Player()
	{
		Reset();
	}

	/* @Player
	*
	* @所持キーを表示<デバッグ用>
	*/
	void Player::IdPrint()const
	{
		std::cout << actorId << std::endl;
	}
	/* @PlayerNormal
	* エラーを表示する<>
	*/
	void Player::ErrorPrint(const char* massage)
	{
		std::cerr << "ERROR>>Sound::PlayerNormal::" << massage << std::endl;
	}

	/* @Player
	*
	* アクターの破棄を手動で行います
	* ※行わない場合は変数が破棄されるときにデストラクタで自動で行われる.
	*/
	void Player::Reset()
	{
		if (actorId >= 0) {
			Engine::Instance().ActorDestroy(actorId);
		}
	}
	/*
	* デバッグ用　alソースのステートを取得する.
	*/
	void Player::DebugSourcePrint()const
	{
		std::shared_ptr<ActorClasses::Actor>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor>(actorId));
		actorPtr->PrintSourceState();
	}


	/* @Player3D
	*============================================================
	* 3Dサウンドで使用する
	* 使用する場合はリスナーの設定も必須になるので
	* そちらも確認しよう
	*
	* Playerの関数は全て継承している+@で独自関数を持つ
	*=============================================================
	*/


	/* @Player3D
	* Init関数,バッファーにアクターオブジェクトを作成する.
	*
	* @param filename: waveファイル(.wav)
	*
	* @note		waveファイル(ステレオorモノラル)のみ受け付ける
	*/
	bool Player3D::Init(const char* filename)
	{
		Reset();
		Engine::Instance().CreateActor<ActorClasses::Actor3D>(actorId, filename);
		if (actorId < 0) {
			ErrorPrint("Init():Actorの生成に失敗しました");
			return false;
		}
		std::shared_ptr<ActorClasses::Actor3D>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId));
		actorPtr->Init();


		return true;
	}

	/*@Player3D
	* ポジションを設定する
	*
	* @param x,y,z :空間上の位置
	*				デフォルト値{0.0f,0.0f,0.0f}
	*
	* @note Listenerの座標と向きとオブジェクトの位置から,
	*		音が聞こえる方向や、音の減衰などが決まる.
	*/
	void Player3D::SetPosition(float x, float y, float z)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->SetPosition(Vector{ x,y,z });
	}

	/*@Player3D
	* 速度を設定する,位置が動くわけではなくドップラー効果の計算に使用する.
	*
	* @param x,y,z :速度ベクトル
	*				デフォルト値{0.0f,0.0f,0.0f}
	*
	* @note
	*/
	void Player3D::SetVelocity(float x, float y, float z)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->SetVelocity(Vector{ x,y,z });

	}

	/*@Player3D
	* 距離による減衰率を設定する
	*
	* @param referenceDistance :音が半分減衰する距離
	*							デフォルト値{1.0f}
	*
	* @note 距離による音量の減衰に影響する値:長いほど減衰しにくい,短いほど減衰しやすい.
	*/
	void Player3D::SetReferenceDistance(float referenceDistance)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->SetReferenceDistance(referenceDistance);
	}
	/*@Player3D
	* 距離による減衰がなくなる境界線を設定する
	*
	* @param maxDistance	:減衰がなくなるまでの距離(リスナーからの相対距離)
	*						 デフォルト値{100.0f}※openALではdistanceModelの設定によるため勝手に決めてる
	*
	* @note referenceDistance(上記)とうまく組み合わせて使えって公式が言ってた
	*/
	void Player3D::SetMaxDistance(float maxDistance)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->MaxDistance(maxDistance);
	}
	/*@Player3D
	* 音量の最低値の設定(計算適用語)
	*
	* @param minVolume	:min音量(0.0f~)
	*					 デフォルト値{0.0f}
	*
	* @note 減衰によって音が消えてほしくない場合は設定するとよい
	*/
	void Player3D::SetMinVolume(float minVolume)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->MinVolume(minVolume);
	}
	/*@Player3D
	* 音量の最大値の設定(計算適用後)
	*
	* @param maxVolume	 :max音量(0.0f~1.0f)
	*					  デフォルト値{1.0f}
	*
	* @note 上手く使ってください(てきとー)
	*/
	void Player3D::SetMaxVolume(float maxVolume)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3D>(actorId)->MaxVolume(maxVolume);
	}





	/* @Player3DCustom
	*============================================================
	* カスタムということであまり使いそうにない機能は
	* こちらに回させてもらった.
	* この中の機能を使わないのであればベースの3Dの方を使用しよう
	*
	*
	*=============================================================
	*/
	bool Player3DCustom::Init(const char* filename)
	{
		Reset();
		Engine::Instance().CreateActor<ActorClasses::Actor3DCustom>(actorId, filename);
		if (actorId < 0) {
			ErrorPrint("Init():Actorの生成に失敗しました");
			return false;
		}
		std::shared_ptr<ActorClasses::Actor3DCustom>actorPtr(Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId));
		actorPtr->Init();
		return true;
	}


	/*@Player3DCustom
	* 向きを変更する
	*
	* @param x,y,z :方向ベクトル(単位ベクトル)
	*				デフォルト値{0.0f,0.0f,0.0f}
	*
	* @note 後述の音の方向による減衰の設定と深く関係する
	*/
	void Player3DCustom::SetDirection(float x, float y, float z)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetDirection(Vector{ x,y,z });
	}


	/*@Player3DCustom
	* コーンのインナー角の設定
	*
	* @param coneInnerAngle	:円錐の頂点の角度(0.0f~360.0f)
	*						 デフォルト値{360.0f}
	*
	* @note ConeOutorVolumeの減衰が適用されない角度.向きの設定はDistanceで,
	*/
	void Player3DCustom::SetConeInnerAngle(float coneInnerAngle)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetConeInnerAngle(coneInnerAngle);
	}
	/*@Player3DCustom
	* コーンのアウター角の設定
	*
	* @param ConeOuterAngle :円錐の頂点の角度(0.0f~360.0f)
	*					     デフォルト値{360.0f}
	*
	* @note インナー角とアウター角の間で徐々に減衰されていくイメージ
	*/
	void Player3DCustom::SetConeOuterAngle(float coneOuterAngle)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetConeOuterAngle(coneOuterAngle);
	}
	/*@Player3DCustom
	* アウター角の外の音量を設定する
	*
	* @param coneOuterVolume :max音量(0.0f~1.0f)
	*						  デフォルト値{1.0f}
	*
	* @note 音源の向きと内側円錐,外側円錐,外型円錐の外の音量...四人はセットなんです.
	*/
	void Player3DCustom::SetConeOuterVolume(float coneOuterVolume)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetConeOuterVolume(coneOuterVolume);
	}
	/*@Player3DCustom
	* ロールキャベツである(大嘘)
	*
	* @param rolloff :ロールオフの値 [0.0f - ]
	*				  デフォルト値{1.0f}
	*
	* @note 美味しいよね(本人も使い方がわからない)
	* --以下公式から和訳して引用--
	* 距離減衰を誇張または減少させるための乗数
	* 0.0では、距離の減衰は発生しません。
	*/
	void Player3DCustom::SetRollOffFactor(float rolloff)const
	{
		Engine::Instance().GetActorPtr<ActorClasses::Actor3DCustom>(actorId)->SetRollOffFactor(rolloff);
	}

}


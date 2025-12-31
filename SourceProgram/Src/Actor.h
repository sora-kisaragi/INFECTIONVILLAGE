/**
* @file Actor.h
*/
#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include <GL/glew.h>
#include "Shader.h"
#include "MeshList.h"
#include <glm/vec3.hpp>
#include <vector>
#include <functional>
#include <memory>

/**
Actor* FindAvailableActor(std::vector<std::unique_ptr<Actor>>&);
void UpdateActorList(std::vector<std::unique_ptr<Actor>>&, float);
void RenderActorList(std::vector<std::unique_ptr<Actor>>&, Shader::Program&, MeshList&);

void DetectCollision(std::vector<std::unique_ptr<Actor>>&, std::vector<std::unique_ptr<Actor>>&, CollsionHandlerType);
	glm::vec3 size;
};

/**
* シーンに配置するオブジェクト.
*/
class Actor
{
public:
	Actor() = default;
	virtual ~Actor() = default;

	void Initialize(int mesh, GLuint tex, int hp, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	void Finalize();

	virtual void Update(float deltTIme);

public:
	int mesh = 0;
	GLuint texture = 0;

	//Tranceform変数
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	//速度変数
	glm::vec3 velocity = glm::vec3(0);
	//HP変数
	int Actor_HP = 0;

	//ローカルコリジョン
	Rect colLocal;
	//ワールドコリジョン
	Rect colWorld;
};

//利用可能なActorを取得する.
Actor* FindAvailableActor(std::vector<Actor*>&);
//アクターの状態を更新する.
void UpdateActorList(std::vector<Actor*>&, float);
//Actorを描画する.
void RenderActorList(std::vector<Actor*>&, Shader::Program&, MeshList&);
//アクターリストを空にする.
void ClearActorList(std::vector<Actor*>&);

//コリジョン判定のグループ.
using CollsionHandlerType = std::function<void(Actor&, Actor&)>;
//2つの長方形の衝突状態を調べる.
bool DetectCollision(const Actor&, const Actor&);
//2つのグループ間で衝突判定を行う.
void DetectCollision(std::vector<Actor*>&, std::vector<Actor*>&, CollsionHandlerType);

// 衝突した面(0=左, 1=右, 2=下, 3=上, 4=奥, 5=手前).
enum class CollisionPlane
{
  none = -1, // 衝突なし.
  negativeX = 0, // 左側.
  positiveX, // 右側.
  negativeY, // 下側.
  positiveY, // 上側.
  negativeZ, // 奥側.
  positiveZ, // 手前側.
};

//衝突するまでの時間を計算する為に必要な構造体
struct CollisionTime
{
  float time; // 衝突した時間.
  CollisionPlane plane; // 衝突した面.
};

//衝突するまでの経過時間を計算する
CollisionTime FindCollisionTime(const Actor&, const Actor&, float);

#endif // ACTOR_H_INCLUDED
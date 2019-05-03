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

/**
* ������.
*/
struct Rect
{
	glm::vec3 origin;
	glm::vec3 size;
};

/**
* �V�[���ɔz�u����I�u�W�F�N�g.
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

	//Tranceform�ϐ�
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	//���x�ϐ�
	glm::vec3 velocity = glm::vec3(0);
	//HP�ϐ�
	int Actor_HP = 0;

	//���[�J���R���W����
	Rect colLocal;
	//���[���h�R���W����
	Rect colWorld;
};

//���p�\��Actor���擾����.
Actor* FindAvailableActor(std::vector<Actor*>&);
//�A�N�^�[�̏�Ԃ��X�V����.
void UpdateActorList(std::vector<Actor*>&, float);
//Actor��`�悷��.
void RenderActorList(std::vector<Actor*>&, Shader::Program&, MeshList&);
//�A�N�^�[���X�g����ɂ���.
void ClearActorList(std::vector<Actor*>&);

//�R���W��������̃O���[�v.
using CollsionHandlerType = std::function<void(Actor&, Actor&)>;
//2�̒����`�̏Փˏ�Ԃ𒲂ׂ�.
bool DetectCollision(const Actor&, const Actor&);
//2�̃O���[�v�ԂŏՓ˔�����s��.
void DetectCollision(std::vector<Actor*>&, std::vector<Actor*>&, CollsionHandlerType);

// �Փ˂�����(0=��, 1=�E, 2=��, 3=��, 4=��, 5=��O).
enum class CollisionPlane
{
  none = -1, // �Փ˂Ȃ�.
  negativeX = 0, // ����.
  positiveX, // �E��.
  negativeY, // ����.
  positiveY, // �㑤.
  negativeZ, // ����.
  positiveZ, // ��O��.
};

//�Փ˂���܂ł̎��Ԃ��v�Z����ׂɕK�v�ȍ\����
struct CollisionTime
{
  float time; // �Փ˂�������.
  CollisionPlane plane; // �Փ˂�����.
};

//�Փ˂���܂ł̌o�ߎ��Ԃ��v�Z����
CollisionTime FindCollisionTime(const Actor&, const Actor&, float);

#endif // ACTOR_H_INCLUDED
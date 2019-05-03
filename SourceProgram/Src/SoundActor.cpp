#include "SoundActor.h"
#include "SoundALobjects.h"
#include "SoundBuffer.h"
namespace Sound {
	/* @Actor �h���N���X�̏���.
	*========================================================
	*
	*========================================================
	*/

	/* @Actor(���N���X)
	*
	*/
	void ActorClasses::Actor::Stop(float easingTime)const
	{
		//�\�[�X���������Ă��Ȃ��Ȃ�X���[
		if (sourceId < 0) {
			return;
		}
		Engine::ActorInstance().GetSourcePtr(sourceId)->Stop(easingTime);
	}
	/* @Actor(���N���X)
	*
	*/
	void ActorClasses::Actor::Pouse()const
	{
		//�\�[�X���������Ă��Ȃ��Ȃ�X���[
		if (sourceId < 0) {
			return;
		}
		Engine::ActorInstance().GetSourcePtr(sourceId)->Pouse();
	}
	/*
	*
	*/
	bool ActorClasses::Actor::Init()
	{

		if (GetStateBoolian(e_BOOL_3D)) {
			Engine::ActorInstance().ReadSoundData3D(buffername);
		}
		else {
			Engine::ActorInstance().ReadSoundData(buffername);
		}
		return true;
	}

	/*
	* ���[�v��ݒ肷��.
	*
	*/
	void ActorClasses::Actor::SetLooping(bool _io) {
		//�A�N�^�[�̃X�e�[�g��ݒ�
		SetState(e_BOOL_LOOPING, _io);

		//�\�[�X���������Ă���̂ł���ΐݒ肷��
		
		if (sourceId>=0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetLooping(GetStateBoolian(e_BOOL_LOOPING));
		}
	}
	/*
	* ���ʂ�ݒ肷��
	*/
	void ActorClasses::Actor::SetVolume(float volume)
	{
		gain = volume;
		//�\�[�X���������Ă���̂ł���ΐݒ肷��.
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetVolume(gain);
		}
	}
	/*
	* �s�b�`��ݒ肷��
	*/
	void ActorClasses::Actor::SetPitch(float pitch)
	{
		this->pitch=pitch;
		//�\�[�X���������Ă���̂ł���ΐݒ肷��.
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetPitch(pitch);
		}
	}


	/* @Actor <state>�֘A
	* �X�e�[�g����͂���.
	*
	*/
	void ActorClasses::Actor::SetState(ActorClasses::Actor::STATE _state)
	{
		//state��1byte�ڂ�0�ɂ������̂� ������_state ��XOR���Z���đ��
		state = (state & ~e_STATE_MAX) ^ _state;
	}

	/*
	* �X�e�[�g(bool)����͂���.
	*/
	void ActorClasses::Actor::SetState(ActorClasses::Actor::STATE_BOOLIAN _stateboolian, bool _io)
	{
		_io ? state |= _stateboolian : state &= (~_stateboolian);
	}

	/*
	*
	*
	*/
	ActorClasses::Actor::STATE ActorClasses::Actor::GetState()const
	{
		return static_cast<STATE>(state & e_STATE_MAX);
	}

	/*
	*
	*
	*/
	bool ActorClasses::Actor::GetStateBoolian(ActorClasses::Actor::STATE_BOOLIAN _stateboolian) const
	{
		return state & _stateboolian ? true : false;
	}

	void ActorClasses::Actor::PrintSourceState()
	{
		Engine::ActorInstance().GetSourcePtr(sourceId)->PrintState();
	}


	/* @ActorNormal
	*
	* Play()�����Đ�����.
	*
	*/
	void ActorClasses::Actor::Play(short actorId ,float easingTime)
	{
		//�\�[�X���������Ă��Ȃ��ꍇ�͎擾����.
		if (sourceId < 0) {
			sourceId = Engine::ActorInstance().SetSourceToActor(actorId);
			if (sourceId < 0) {
				std::cerr << "ERROR>>Source::ActorNormal::Play():�\�[�X���擾�ł��܂���ł���" << std::endl;
				return;
			}
			//�����Œl���Z�b�g����.
			SetSourceParameter();
		}
		//�ǂ����Ă������ŃZ�b�g�������ꍇ
		SetSourceParameterExCeption();

		//����炷.
		Engine::ActorInstance().GetSourcePtr(sourceId)->Play(easingTime);
		//�A�N�e�B�u�t���O��on�ɂ���.

	}


	void ActorClasses::Actor::SetSourceParameter()
	{
		ALSourceIDPtr sourcePtr = Engine::ActorInstance().GetSourcePtr(sourceId);
		if (GetStateBoolian(e_BOOL_3D)) {
			sourcePtr->SetBuffer(Engine::ActorInstance().GetBuffer3DPtr(buffername)->Id());
			sourcePtr->SetRelative(false);
		}
		else {
			sourcePtr->SetBuffer(Engine::ActorInstance().GetBufferPtr(buffername)->Id());
			sourcePtr->SetRelative(true);
		}
		sourcePtr->SetLooping(GetStateBoolian(e_BOOL_LOOPING));
		sourcePtr->SetVolume(gain);

	}
	void ActorClasses::Actor::SetSourceParameterExCeption()
	{

	}


	/*�ȉ� Actor3D
	*===============
	*
	*
	*
	*==================
	*/
	bool ActorClasses::Actor3D::Init()
	{

		SetState(e_BOOL_3D,true);
		Actor::Init();
		return true;
	}
	void ActorClasses::Actor3D::SetSourceParameter()
	{
		Actor::SetSourceParameter();
		ALSourceIDPtr sourcePtr = Engine::ActorInstance().GetSourcePtr(sourceId);
		sourcePtr->SetReferenceDistance(referenceDistance);
		sourcePtr->SetMaxDistance(maxDistance);
		sourcePtr->SetMinGain(minVolume);
		sourcePtr->SetMaxGain(maxVolume);
	}
	void ActorClasses::Actor3D::SetSourceParameterExCeption() {
		Actor::SetSourceParameterExCeption();
		ALSourceIDPtr sourcePtr = Engine::ActorInstance().GetSourcePtr(sourceId);
		sourcePtr->SetPosition(position.x, position.y, position.z);
		sourcePtr->SetVelocity(velocity.x, velocity.y, velocity.z);
	}

	void ActorClasses::Actor3D::SetPosition(const Vector& newPositon) 
	{
		position = newPositon;
		if (sourceId >= 0) {
			if (Engine::ActorInstance().GetSourcePtr(sourceId)->IsActive()) {
				//���t���[����񂪍X�V�����\��������̂Ń\�[�X���v���C���݂̂���AL�ɑ��M���Ȃ�
				Engine::ActorInstance().GetSourcePtr(sourceId)->SetPosition(position.x, position.y, position.z);
			}
		}
	}
	void ActorClasses::Actor3D::SetVelocity(const Vector& newVelocity)	
	{
		velocity = newVelocity;
		if (sourceId >= 0) {
			if (Engine::ActorInstance().GetSourcePtr(sourceId)->IsActive()) {
				//���t���[����񂪍X�V�����\��������̂Ń\�[�X���v���C���݂̂���AL�ɑ��M���Ȃ�
				Engine::ActorInstance().GetSourcePtr(sourceId)->SetVelocity(velocity.x, velocity.y, velocity.z);
			}
		}
	}
	void ActorClasses::Actor3D::SetReferenceDistance(float distance)
	{
		referenceDistance = distance;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetReferenceDistance(referenceDistance);
		}
	}
	void ActorClasses::Actor3D::MaxDistance(float distance)
	{
		maxDistance = distance;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetMaxDistance(maxDistance);
		}
	}
	void ActorClasses::Actor3D::MinVolume(float volume)
	{
		minVolume = volume;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetMinGain(minVolume);
		}
	}
	void ActorClasses::Actor3D::MaxVolume(float volume)
	{
		maxVolume = volume;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetMaxGain(maxVolume);
		}
	}


	//bool ActorClasses::Actor3DCustom::Init()
	//{
	//	Actor3D::Init();
	//}
	void ActorClasses::Actor3DCustom::SetSourceParameter()
	{
		Actor3D::SetSourceParameter();
		ALSourceIDPtr sourcePtr = Engine::ActorInstance().GetSourcePtr(sourceId);
		sourcePtr->SetConeInnerAngle(coneInnerAngle);
		sourcePtr->SetConeOuterAngle(coneOuterAngle);
		sourcePtr->SetConeOuterGain(coneOuterVolume);
		sourcePtr->SetRollOffFactor(rolloffFactor);

	}
	void ActorClasses::Actor3DCustom::SetSourceParameterExCeption() {
		Actor3D::SetSourceParameterExCeption();
		ALSourceIDPtr sourcePtr = Engine::ActorInstance().GetSourcePtr(sourceId);
		sourcePtr->SetDirection(direction.x, direction.y, direction.z);
	}
	void ActorClasses::Actor3DCustom::SetConeInnerAngle(float angle)
	{
		coneInnerAngle = angle;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetConeInnerAngle(coneInnerAngle);
		}
	}
	void ActorClasses::Actor3DCustom::SetConeOuterAngle(float angle)
	{
		coneOuterAngle = angle;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetConeOuterAngle(coneOuterAngle);
		}
	}
	void ActorClasses::Actor3DCustom::SetConeOuterVolume(float volume)
	{
		coneOuterVolume = volume;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetConeOuterGain(coneOuterVolume);
		}
	}
	void ActorClasses::Actor3DCustom::SetRollOffFactor(float rolloff)
	{
		rolloffFactor = rolloff;
		if (sourceId >= 0) {
			Engine::ActorInstance().GetSourcePtr(sourceId)->SetRollOffFactor(rolloffFactor);
		}
	}
	void ActorClasses::Actor3DCustom::SetDirection(const Vector& newDirection)
	{
		direction = newDirection;
		if (sourceId >= 0) {
			//���t���[����񂪍X�V�����\��������̂Ń\�[�X���v���C���݂̂���AL�ɑ��M���Ȃ�
			if (Engine::ActorInstance().GetSourcePtr(sourceId)->IsActive()) {
				Engine::ActorInstance().GetSourcePtr(sourceId)->SetDirection(direction.x, direction.y, direction.z);
			}
		}
	}

}
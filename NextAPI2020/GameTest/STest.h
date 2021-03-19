#pragma once

// Internal includes
#include "Scene.h"
#include "CBoxCollider.h"

class STest : public Scene
{
public:
	//--- Constructors and Destructor ---//
	STest();
	virtual ~STest();

	//--- Scene Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;
	virtual void Draw() override;

	//--- Game Logic Methods ---//
	void PrintCollision(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap);
	void SpinPlayer(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap);

private:
	//--- Private Data ---//
	Entity* m_player;
};
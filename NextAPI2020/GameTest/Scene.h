#pragma once

class Scene
{
public:
	//--- Constructors and Destructors ---//
	Scene() {}
	virtual ~Scene() {}

	//--- Methods ---//
	virtual void Load() = 0;
	virtual void Start() = 0;
	virtual void Update(float _deltaTime) = 0;
	virtual void Unload() = 0;
};
#pragma once

// Internal includes
#include "Component.h"

class CBank : public Component
{
public:
	//--- Constructors and Destructor ---//
	CBank();
	virtual ~CBank();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;
	
	//--- Methods ---//
	void AddMoney(float _money);
	void RemoveMoney(float _money);

	//--- Setters ---//
	void SetStartMoney(float _startMoney);

	//--- Getters ---//
	float GetStartMoney() const;
	float GetMoney() const;
	int GetMoneyRounded() const;

private:
	//--- Private Data ---//
	float m_startMoney;
	float m_currentMoney;
};

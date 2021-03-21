// Standard header
#include "stdafx.h"

// Standard includes
#include <math.h>

// Internal includes
#include "CBank.h"

//--- Constructors and Destructor ---//
CBank::CBank()
{
	m_startMoney = 0.0f;
	m_currentMoney = 0.0f;
}

CBank::~CBank()
{
}



//--- Component Interface ---//
void CBank::Init()
{
}

void CBank::Update(float _deltaTime)
{
}



//--- Methods ---//
void CBank::AddMoney(float _money)
{
	m_currentMoney += _money;
}

void CBank::RemoveMoney(float _money)
{
	m_currentMoney -= _money;
}



//--- Setters ---//
void CBank::SetStartMoney(float _startMoney)
{
	m_startMoney = _startMoney;
	m_currentMoney = m_startMoney;
}



//--- Getters ---//
float CBank::GetStartMoney() const
{
	return m_startMoney;
}

float CBank::GetMoney() const
{
	return m_currentMoney;
}

int CBank::GetMoneyRounded() const
{
	return (int)rint(m_currentMoney);
}
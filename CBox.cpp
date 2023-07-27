#include "pch.h"
#include "CBox.h"

#include "CCore.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Start.h"

CBox::CBox()
	: arrData{ 0, 0, 0 }
	, arrRGB{ 255, 255, 255 }
	, isBlock(false)
	, ptCursor{ 0 , 0 }
	, uPos{ 0, 0 }
{
}

CBox::~CBox()
{
}

void CBox::SetBlock()
{
	isBlock = true;
	arrRGB[0] = 0;
	arrRGB[1] = 0;
	arrRGB[2] = 0;
}

void CBox::update()
{
	if (!isBlock)
	{
		CScene_Start* pCurScene = dynamic_cast<CScene_Start*>
			(CSceneMgr::GetInst()->GetCurScene());

		if (KEY_TAP(KEY::MOUSELBUTTON))
		{
			HWND hWnd = CCore::GetInst()->GetMainHwnd();
			GetCursorPos(&ptCursor);
			ScreenToClient(hWnd, &ptCursor);

			if (pCurScene->GetBoxStart() == nullptr || 
				pCurScene->GetBoxEnd() == nullptr)
			{
				if (CheckInside(ptCursor))
				{
					if (pCurScene->GetBoxStart() == nullptr)
					{
						pCurScene->SetBoxStart(this);
					}
					else if (pCurScene->GetBoxEnd() == nullptr)
					{
						pCurScene->SetBoxEnd(this);
					}
					arrRGB[0] = 0;
					arrRGB[1] = 0;
					arrRGB[2] = 255;
				}
			}
		}

		if (pCurScene->GetBoxStart() == nullptr &&
			pCurScene->GetBoxEnd() == nullptr)
		{
			//arrData[0] = 0;
			//arrData[1] = 0;
			//arrData[2] = 0;
			//arrRGB[0] = 255;
			//arrRGB[1] = 255;
			//arrRGB[2] = 255;
		}
	}
}

void CBox::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	HBRUSH hBrush = CreateSolidBrush(RGB(arrRGB[0], arrRGB[1], arrRGB[2]));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_dc, hBrush);

	Rectangle(_dc, vPos.x - vScale.x / 2, vPos.y - vScale.y / 2, 
		vPos.x + vScale.x / 2, vPos.y + vScale.y / 2);

	wstring wstrBuff;
	wstrBuff = to_wstring(arrData[0]);
	TextOut(_dc, vPos.x - 24, vPos.y - 20, wstrBuff.c_str(), wstrBuff.length());
	wstrBuff = to_wstring(arrData[1]);
	TextOut(_dc, vPos.x + 16, vPos.y - 20, wstrBuff.c_str(), wstrBuff.length());
	wstrBuff = to_wstring(arrData[2]);
	TextOut(_dc, vPos.x - 4, vPos.y + 10, wstrBuff.c_str(), wstrBuff.length());

	SelectObject(_dc, oldBrush);
	DeleteObject(hBrush);
}

bool CBox::CheckInside(const POINT& _pt)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	
	int32 iLeft = vPos.x - vScale.x / 2;
	int32 iRight = vPos.x + vScale.x / 2;
	int32 iUp = vPos.y - vScale.y / 2;
	int32 iBottom = vPos.y + vScale.y / 2;

	if (_pt.x < iLeft)
		return false;
	if (_pt.x > iRight)
		return false;
	if (_pt.y < iUp)
		return false;
	if (_pt.y > iBottom)
		return false;
	
	return true;
}

void CBox::SetUnitPos(Vec2 _uPos)
{
	uPos = _uPos;
}

bool CBox::CheckInside(const Vec2& _pt)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	int32 iLeft = vPos.x - vScale.x / 2;
	int32 iRight = vPos.x + vScale.x / 2;
	int32 iUp = vPos.y - vScale.y / 2;
	int32 iBottom = vPos.y + vScale.y / 2;

	if (_pt.x < iLeft)
		return false;
	if (_pt.x > iRight)
		return false;
	if (_pt.y < iUp)
		return false;
	if (_pt.y > iBottom)
		return false;

	return true;
}

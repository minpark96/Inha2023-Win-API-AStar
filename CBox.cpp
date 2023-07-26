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
		if (KEY_TAP(KEY::MOUSELBUTTON))
		{
			HWND hWnd = CCore::GetInst()->GetMainHwnd();
			GetCursorPos(&ptCursor);
			ScreenToClient(hWnd, &ptCursor);

			CScene_Start* pCurScene = dynamic_cast<CScene_Start*>
				(CSceneMgr::GetInst()->GetCurScene());

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

	char buff[10];
	sprintf_s(buff, "%d", arrData[0]);
	TextOut(_dc, vPos.x - 24, vPos.y - 20, (LPCWSTR)buff, strlen(buff));
	sprintf_s(buff, "%d", arrData[1]);
	TextOut(_dc, vPos.x + 16, vPos.y - 20, (LPCWSTR)buff, strlen(buff));
	sprintf_s(buff, "%d", arrData[2]);
	TextOut(_dc, vPos.x - 4, vPos.y + 10, (LPCWSTR)buff, strlen(buff));

	/*wchar_t szBuffer[255] = {};
	swprintf_s(szBuffer, L"pt x : %d, pt y : %d", ptCursor.x, ptCursor.y);
	TextOut(_dc, 0, 700, (LPCWSTR)szBuffer, wcslen(szBuffer));
	if (CheckInside(ptCursor))
	{
		swprintf_s(szBuffer, L"Box x : %d, Box y : %d", (int)vPos.x, (int)vPos.y);
		TextOut(_dc, 0, 720, (LPCWSTR)szBuffer, wcslen(szBuffer));
	}*/

	SelectObject(_dc, oldBrush);
	DeleteObject(hBrush);
}

bool CBox::CheckInside(const POINT& _pt)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	
	int32 fLeft = vPos.x - vScale.x / 2;
	int32 fRight = vPos.x + vScale.x / 2;
	int32 fUp = vPos.y - vScale.y / 2;
	int32 fBottom = vPos.y + vScale.y / 2;

	if (_pt.x < fLeft)
		return false;
	if (_pt.x > fRight)
		return false;
	if (_pt.y < fUp)
		return false;
	if (_pt.y > fBottom)
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

	int32 fLeft = vPos.x - vScale.x / 2;
	int32 fRight = vPos.x + vScale.x / 2;
	int32 fUp = vPos.y - vScale.y / 2;
	int32 fBottom = vPos.y + vScale.y / 2;

	if (_pt.x < fLeft)
		return false;
	if (_pt.x > fRight)
		return false;
	if (_pt.y < fUp)
		return false;
	if (_pt.y > fBottom)
		return false;

	return true;
}

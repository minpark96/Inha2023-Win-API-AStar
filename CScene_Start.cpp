#include "pch.h"
#include "CScene_Start.h"

#include "CCore.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CBox.h"

CScene_Start::CScene_Start()
	: m_boxStart(nullptr)
	, m_boxEnd(nullptr)
	, m_boxCount(0)
	, arrBlock()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	// 박스 배치
	int iBoxCount = 16;
	int iMoveDist = 25;

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	int iObjScale = vResolution.x / iBoxCount;
	int iTerm = (vResolution.x - ((iMoveDist + iObjScale / 2) * 2)) / (iBoxCount - 1);

	CBox* pBoxObj = nullptr;

	for(int j = 0; j < iBoxCount / 2; ++j)
	{
		// Box Object 추가
		for (int i = 0; i < iBoxCount; ++i)
		{
			pBoxObj = new CBox;
			pBoxObj->SetPos(Vec2((iMoveDist + iObjScale / 2) + i * iTerm, 
				(iMoveDist + iObjScale / 2) + j * iTerm));
			pBoxObj->SetScale(Vec2(iObjScale, iObjScale));
			pBoxObj->SetUnitPos(Vec2(i, j));
			AddObject(pBoxObj, GROUP_TYPE::BOX);
		}
	}

	m_boxCount = iBoxCount * (iBoxCount / 2);

	// Block 설정
	arrBlock.push_back(19);
	arrBlock.push_back(33); 
	arrBlock.push_back(34); 
	arrBlock.push_back(35); 
	arrBlock.push_back(69); 
	arrBlock.push_back(85);
	arrBlock.push_back(101); 
	arrBlock.push_back(58); 
	arrBlock.push_back(74);
	arrBlock.push_back(90); 
	arrBlock.push_back(91); 
	arrBlock.push_back(92);

	for (int i = 0; i < arrBlock.size(); ++i)
	{
		CBox* pBlock = dynamic_cast<CBox*>(GetBox(arrBlock[i]));
		pBlock->SetBlock();
	}

	// Object 추가
	CObject* pObj = new CPlayer;
	pObj->SetPos(Vec2((iMoveDist + iObjScale / 2) + iTerm,
		(iMoveDist + iObjScale / 2) + iTerm));
	pObj->SetScale(Vec2(20, 20));
	AddObject(pObj, GROUP_TYPE::PLAYER);
}

void CScene_Start::Exit()
{
}


#include "pch.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Start.h"

#include "CBox.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CPlayer::CPlayer()
	: m_vStart{ 0, 0 }
	, m_vEnd{ 0, 0 }
	, m_path()
	, m_pathIndex()
{

}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	// 마우스 클릭 시 이동
	CScene_Start* pCurScene = dynamic_cast<CScene_Start*>
		(CSceneMgr::GetInst()->GetCurScene());

	if (pCurScene->GetBoxStart() != nullptr &&
		pCurScene->GetBoxEnd() != nullptr)
	{
		AStar();
		Vec2 vPos;
		vPos.x = (int32)(m_path[m_pathIndex].x * fDT);
		vPos.y = (int32)(m_path[m_pathIndex].y * fDT);
		++m_pathIndex;
		SetPos(vPos);

		if (m_pathIndex >= m_path.size())
		{
			pCurScene->SetBoxStart(nullptr);
			pCurScene->SetBoxEnd(nullptr);
		}
	}
}

void CPlayer::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_dc, hBrush);

	Ellipse(_dc, vPos.x - vScale.x / 2, vPos.y - vScale.y / 2
		, vPos.x + vScale.x / 2, vPos.y + vScale.y / 2);

	SelectObject(_dc, oldBrush);
	DeleteObject(hBrush);
}

void CPlayer::AStar()
{
	// F = G + H
	CScene_Start* pCurScene = dynamic_cast<CScene_Start*>
		(CSceneMgr::GetInst()->GetCurScene());
	CBox* pBoxStart = pCurScene->GetBoxStart();
	CBox* pBoxEnd = pCurScene->GetBoxEnd();

	Vec2 start = pBoxStart->GetUnitPos();
	Vec2 dest = pBoxEnd->GetUnitPos();

	enum
	{
		DIR_COUNT = 8
	};

	Vec2 front[] =
	{
		Vec2 { -1, 0},	// UP
		Vec2 { 0, -1},	// LEFT
		Vec2 { 1, 0},	// DOWN
		Vec2 { 0, 1},	// RIGHT
		Vec2 {-1, -1},	// UP_LEFT
		Vec2 {1, -1},	// DOWN_LEFT
		Vec2 {1, 1},		// DOWN_RIGHT
		Vec2 {-1, 0},	// UP_RIGHT
	};

	int32 cost[] =
	{
		10, // UP
		10, // LEFT
		10, // DOWN
		10, // RIGHT
		14,
		14,
		14,
		14
	};

	const int32 size = pCurScene->GetBoxCount();
	
	// ClosedList
	// close[y][x] -> (y, x)에 방문을 했는지 여부
	vector<vector<bool>> closed(size, vector<bool>(size, false));

	// best[y][x] -> 지금까지 (y, x)에 대한 가장 좋은 비용 (작을 수록 좋음)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// 부모 추적 용도	
	map<Vec2, Vec2> parent;

	// OpenList
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	// 1) 예약(발견) 시스템 구현
	// - 이미 더 좋은 경로를 찾았다면 스킵
	// 2) 뒤늦게 더 좋은 경로가 발견될 수 있음 -> 예외 처리 필수
	// - openList에서 찾아서 제거한다거나.
	// - pq에서 pop한 다음에 무시한다거나.
	// 
	// 초기값
	{
		int32 g = 0;
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다
		PQNode node = pq.top();
		pq.pop();

		// 동일한 좌표를 여러 경로로 찾아서\
		// 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵
		// [선택]
		if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// 방문
		closed[node.pos.y][node.pos.x] = true;

		// 목적지에 도착했으면 바로 종료
		if (node.pos == dest)
			break;

		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			Vec2 nextPos = node.pos + front[dir];
			// 갈 수 있는 지역은 맞는지 확인
			if (CanGo(nextPos) == false)
				continue;
			// [선택] 이미 방문한 곳이면 스킵
			if (closed[nextPos.y][nextPos.x])
				continue;

			// 비용 계산
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));
			// 다른 경로에서 더 빠른 길을 찾았으면 스킵
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// 예약 진행
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;
		}
	}

	// 거꾸로 거슬러 올라간다
	Vec2 pos = dest;
	CBox* pBox = dynamic_cast<CBox*>(pCurScene->GetBox(pos.x + pos.y * 16));

	m_path.clear();
	m_pathIndex = 0;

	while (true)
	{
		pBox = dynamic_cast<CBox*>(pCurScene->GetBox(pos.x + pos.y * 16));
		Vec2 realPos = pBox->GetPos();
		m_path.push_back(realPos);

		// 시작점은 자신이 곧 부모이다
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(m_path.begin(), m_path.end());
}

bool CPlayer::CanGo(Vec2 pos)
{
	CScene_Start* pCurScene = dynamic_cast<CScene_Start*>
		(CSceneMgr::GetInst()->GetCurScene());
	int iCount = pCurScene->GetBlockCount();

	for (int i = 0; i < iCount; ++i)
	{
		CBox* pBlock = dynamic_cast<CBox*>(pCurScene->GetBox(i));
		if (pBlock->CheckInside(pos))
			return false;
	}

	return true;
}



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
	, _sumTick(0)
	, _isMove(false)
{

}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	// ���콺 Ŭ�� �� �̵�
	CScene_Start* pCurScene = dynamic_cast<CScene_Start*>
		(CSceneMgr::GetInst()->GetCurScene());

	if (pCurScene->GetBoxStart() != nullptr &&
		pCurScene->GetBoxEnd() != nullptr)
	{
		Vec2 vPos;
		if (!_isMove)
		{
			vPos = pCurScene->GetBoxStart()->GetPos();
			SetPos(vPos);
			AStar();
			_isMove = true;
		}

		_sumTick += DeltaTick;
		if (_sumTick >= MOVE_TICK)
		{
			_sumTick = 0;
			vPos.x = m_path[m_pathIndex].x;
			vPos.y = m_path[m_pathIndex].y;
			++m_pathIndex;
			SetPos(vPos);
		}

		if (m_pathIndex >= m_path.size())
		{
			pCurScene->SetBoxStart(nullptr);
			pCurScene->SetBoxEnd(nullptr);
			_isMove = false;
		}
	}
}

void CPlayer::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
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
		Vec2 {1, 1},	// DOWN_RIGHT
		Vec2 {-1, 1},	// UP_RIGHT
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
	// close[y][x] -> (y, x)�� �湮�� �ߴ��� ����
	vector<vector<bool>> closed(size, vector<bool>(size, false));

	// best[y][x] -> ���ݱ��� (y, x)�� ���� ���� ���� ��� (���� ���� ����)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// �θ� ���� �뵵	
	map<Vec2, Vec2> parent;

	// OpenList
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	// 1) ����(�߰�) �ý��� ����
	// - �̹� �� ���� ��θ� ã�Ҵٸ� ��ŵ
	// 2) �ڴʰ� �� ���� ��ΰ� �߰ߵ� �� ���� -> ���� ó�� �ʼ�
	// - openList���� ã�Ƽ� �����Ѵٰų�.
	// - pq���� pop�� ������ �����Ѵٰų�.
	// 
	// �ʱⰪ
	{
		int32 g = 0;
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}

	while (pq.empty() == false)
	{
		// ���� ���� �ĺ��� ã�´�
		PQNode node = pq.top();
		pq.pop();

		// ������ ��ǥ�� ���� ��η� ã�Ƽ�\
		// �� ���� ��η� ���ؼ� �̹� �湮(closed)�� ��� ��ŵ
		// [����]
		if (closed[node.pos.y][node.pos.x])
			continue;
		//if (best[node.pos.y][node.pos.x] < node.f)
			//continue;

		// �湮
		closed[node.pos.y][node.pos.x] = true;

		// �������� ���������� �ٷ� ����
		if (node.pos == dest)
			break;

		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			Vec2 nextPos = node.pos + front[dir];

			// ���ڸ� ��� ��ġ���� Ȯ��
			if (nextPos.x < 0 || nextPos.y < 0)
				continue;
			if (nextPos.x > 15 || nextPos.y > 7)
				continue;

			// �� �� �ִ� ������ �´��� Ȯ�� (������� Ȯ��)
			if (CanGo(nextPos) == false)
				continue;

			// [����] �̹� �湮�� ���̸� ��ŵ
			if (closed[nextPos.y][nextPos.x])
				continue;

			// ��� ���
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));
			// �ٸ� ��ο��� �� ���� ���� ã������ ��ŵ
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// ���� ����
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;

			CBox* pBox = dynamic_cast<CBox*>(pCurScene->GetBox(nextPos.x + nextPos.y * 16));
			vector<int> data = { g, h, best[nextPos.y][nextPos.x] };
			pBox->SetData(data);
			pBox->SetColorRed();
		}
	}

	// �Ųٷ� �Ž��� �ö󰣴�
	Vec2 pos = dest;
	CBox* pBox = dynamic_cast<CBox*>(pCurScene->GetBox(pos.x + pos.y * 16));

	m_path.clear();
	m_pathIndex = 0;

	while (true)
	{
		pBox = dynamic_cast<CBox*>(pCurScene->GetBox(pos.x + pos.y * 16));
		Vec2 realPos = pBox->GetPos();
		pBox->SetColorGreen();
		m_path.push_back(realPos);

		// �������� �ڽ��� �� �θ��̴�
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
		CBox* pBlock = dynamic_cast<CBox*>(pCurScene->GetBox(pCurScene->GetBlock(i)));
		if (pBlock->GetUnitPos() == pos)
			return false;
	}

	return true;
}



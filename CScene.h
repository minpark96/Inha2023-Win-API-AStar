#pragma once

// ���漱��
class CObject;

class CScene
{
private:
	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END]; // ������Ʈ�� ���� �� ������ ����Ʈ�� �׷� ������ŭ ����
	wstring				m_strName; // Scene �̸�

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	void update();
	void render(HDC _dc);

	virtual void Enter() = 0; // �ش� Scene �� ���� �� ȣ�� 
	virtual void Exit() = 0;  // �ش� Scene �� Ż�� �� ȣ��

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	CObject* GetPlayer()
	{
		return m_arrObj[(UINT)GROUP_TYPE::PLAYER].back();
	}

	CObject* GetBox(int _idx)
	{
		return m_arrObj[(UINT)GROUP_TYPE::BOX][_idx];
	}

public:
	CScene();
	virtual ~CScene();
};

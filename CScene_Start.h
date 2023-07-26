#pragma once
#include "CScene.h"
#include "CBox.h"

class CScene_Start :
    public CScene
{
private:
    CBox* m_boxStart;
    CBox* m_boxEnd;
    int32 m_boxCount;
    vector<int> arrBlock;

public:
    virtual void Enter();
    virtual void Exit();

public:
    CBox* GetBoxStart() { return m_boxStart; }
    void SetBoxStart(CBox* _box) { m_boxStart = _box; }
    CBox* GetBoxEnd() { return m_boxEnd; }
    void SetBoxEnd(CBox* _box) { m_boxEnd = _box; }
    int32 GetBoxCount() { return m_boxCount; }
    int GetBlockCount() { return arrBlock.size(); }

public:
    CScene_Start();
    ~CScene_Start();
};


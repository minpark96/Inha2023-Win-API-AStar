#pragma once
#include "CObject.h"

enum class DIR
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
    END,
};

class CPlayer :
    public CObject
{
private:
    Vec2 m_vStart;
    Vec2 m_vEnd;
    vector<Vec2>	m_path;
    uint32		m_pathIndex;

public:
    virtual void update();
    virtual void render(HDC _dc);

private:
    bool CanGo(Vec2 pos);
    void AStar();

public:
    CPlayer();
    ~CPlayer();
};


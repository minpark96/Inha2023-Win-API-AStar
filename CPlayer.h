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
    enum
    {
        MOVE_TICK = 500
    };

private:
    Vec2 m_vStart;
    Vec2 m_vEnd;
    vector<Vec2> m_path;
    uint32 m_pathIndex;
    uint64 _sumTick;
    bool _isMove;

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


#pragma once
#include "CObject.h"
class CBox :
    public CObject
{
private:
    vector<int> arrData;
    vector<int> arrRGB;
    bool isBlock;
    POINT ptCursor;
    Vec2 uPos;

public:
    virtual void update();
    virtual void render(HDC _dc);

public:
    void SetBlock();
    bool CheckInside(const POINT& _pt);
    bool CheckInside(const Vec2& _pt);
    void SetUnitPos(Vec2 _uPos);
    Vec2 GetUnitPos() { return uPos; }

private:

public:
    CBox();
    ~CBox();
};


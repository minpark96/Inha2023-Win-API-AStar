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
    void SetColorRed() 
    {
        arrRGB[0] = 255;
        arrRGB[1] = 0;
        arrRGB[2] = 0;
    }
    void SetColorGreen()
    {
        arrRGB[0] = 0;
        arrRGB[1] = 255;
        arrRGB[2] = 0;
    }
    bool CheckInside(const POINT& _pt);
    bool CheckInside(const Vec2& _pt);
    void SetUnitPos(Vec2 _uPos);
    Vec2 GetUnitPos() { return uPos; }
    void SetData(const vector<int>& _arr)
    {
        arrData[0] = _arr[0];
        arrData[1] = _arr[1];
        arrData[2] = _arr[2];
    }

private:

public:
    CBox();
    ~CBox();
};


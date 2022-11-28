#include "Rect.h"

bool onion::object::math::Rect::BoundRect(const math::Vector2& position)
{
    if (!(leftTop.x <= position.x && rightBtm.x >= position.x))
        return false;
    if (!(leftTop.y >= position.y && rightBtm.y <= position.y))
        return false;
    return true;
}

bool onion::object::math::Rect::BoundRect(const Rect& rect)
{
    if (!(leftTop.x <= rect.leftTop.x && rightBtm.x >= rect.rightBtm.x))
        return false;
    if (!(leftTop.y >= rect.leftTop.y && rightBtm.y <= rect.rightBtm.y))
        return false;
    return true;
}

void onion::object::math::Rect::VecToRec(const math::Vector2& pos, int size)
{
    leftTop.x += pos.x - size;
    leftTop.y += pos.y + size;
    rightBtm.x += pos.x + size;
    rightBtm.y += pos.y - size;
}

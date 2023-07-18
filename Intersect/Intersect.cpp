#include "stdafx.h"
#include "Intersect.h"
#include "Scene/Actor.h"
#include "Scene/Component/MoveComponent/TransformComponent.h"
#include "Scene/Component/MoveComponent/CollisionComponent.h"

bool Intersect::IsIntersect(std::shared_ptr<Actor> const* const lhs, std::shared_ptr<Actor> const* const rhs)
{
    if (lhs == nullptr || rhs == nullptr)
    {
        assert(false && "ERROR (Intersect1) : Actor 또는 Trasnform의 정보가 존재하지 않음");
        return false;
    }

    if (!lhs->get()->IsActive() || !rhs->get()->IsActive())
    {
        assert(false && "Error (Intersect2) : 오른쪽 또는 왼쪽의 Actor가 비활성화");
        return false;
    }

    D3DXVECTOR3 lhs_min = lhs->get()->GETPOS - (lhs->get()->GETSCALE * 0.5);
    D3DXVECTOR3 lhs_max = lhs->get()->GETPOS + (lhs->get()->GETSCALE * 0.5);

    D3DXVECTOR3 rhs_min = rhs->get()->GETPOS - (rhs->get()->GETSCALE * 0.5);
    D3DXVECTOR3 rhs_max = rhs->get()->GETPOS + (rhs->get()->GETSCALE * 0.5);

    bool check = true;

    check &= lhs_max.x > rhs_min.x;
    check &= lhs_min.x < rhs_max.x;
    check &= lhs_max.y > rhs_min.y;
    check &= lhs_min.y < rhs_max.y;
    check &= lhs_max.z > rhs_min.z;
    check &= lhs_min.z < rhs_max.z;

    return check;
}

bool Intersect::IsIntersect(AbleArea const & const lhs, AbleArea const & const rhs)
{
    D3DXVECTOR3 lhs_min = lhs.pos - (lhs.size * 0.5);
    D3DXVECTOR3 lhs_max = lhs.pos + (lhs.size * 0.5);

    D3DXVECTOR3 rhs_min = rhs.pos - (rhs.size * 0.5);
    D3DXVECTOR3 rhs_max = rhs.pos + (rhs.size * 0.5);

    bool check = true;

    check &= lhs_max.x > rhs_min.x;
    check &= lhs_min.x < rhs_max.x;
    check &= lhs_max.y > rhs_min.y;
    check &= lhs_min.y < rhs_max.y;

    return check;
}
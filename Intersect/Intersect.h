#pragma once

#define GETPOS GetTransform()->GetPosition()
#define GETSCALE GetTransform()->GetScale()

class Intersect final
{
public:
    static bool IsIntersect(std::shared_ptr<class Actor> const* const lhs, std::shared_ptr<class Actor> const* const rhs);
    static bool IsIntersect(struct AbleArea const& const lhs, struct AbleArea const& const rhs);
};
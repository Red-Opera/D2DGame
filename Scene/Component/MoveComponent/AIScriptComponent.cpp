#include "stdafx.h"
#include "Scene/Actor.h"
#include "AIScriptComponent.h"
#include "TransformComponent.h"

#define DELAY 2     // ���� ���� ��� �ð�
#define SPEED 2     // ���� �̵� �ӵ�

#define FOLLOWINGMONSTER

void AIScriptComponent::Initialize()
{
}

void AIScriptComponent::Update()
{
    D3DXVECTOR3 position = transform->GetPosition();
    D3DXVECTOR3 scale = transform->GetScale();

#ifndef FOLLOWINGMONSTER
    // ===============================================================
    //				���� ���� ���� ���� �̵� ���� ���� ����
    // ===============================================================

    if (direction <= Direction::UpRight || direction == Direction::LeftUp)      // UpRight, Up, RightUp
    {
        float wall = (Settings::Get().GetHeight() - scale.y) * 0.5;                 // �ִ�� �� �� �ִ� ��ġ�� ��� ����

        if (position.y + SPEED <= wall)
            position.y += SPEED;
    }

    if (direction <= Direction::RightDown && direction >= Direction::UpRight)   // RightDown, Right, UpRight
    {
        float wall = (Settings::Get().GetWidth() - scale.x) * 0.5;

        if (position.x + SPEED <= wall)
            position.x += SPEED;
    }

    if (direction <= Direction::DownLeft && direction >= Direction::RightDown)  // DownLeft, Left, RightDown
    {
        float wall = (-Settings::Get().GetHeight() + scale.y) * 0.5;

        if (position.y - SPEED >= wall)
            position.y -= SPEED;
    }

    if (direction <= Direction::LeftUp && direction >= Direction::DownLeft)     // LeftUp, Left, DownLeft
    {
        float wall = (-Settings::Get().GetWidth() + scale.x) * 0.5;

        if (position.x - SPEED >= wall)
            position.x -= SPEED;
    }

    // ===============================================================
    //					 ���� �̵� ���� ���� ����
    // ===============================================================

    if (stopwatch.GetElapsedTimeMs() >= 2000.0)
    {
        Direction new_dir = static_cast<Direction>(Math::Random(0, 7));

        if (direction == new_dir)
        {
            int dir_num = static_cast<int>(new_dir);

            dir_num = (dir_num <= 3) ? dir_num + 4 : dir_num - 4;

            direction = static_cast<Direction>(dir_num);
        }

        else
            direction = new_dir;

        stopwatch.Start();
    }
#endif // !FOLLOWINGMONSTER

    transform->SetPosition(position);
}

void AIScriptComponent::Destroy()
{
}

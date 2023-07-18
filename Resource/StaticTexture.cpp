#include "stdafx.h"
#include "StaticTexture.h"
#include "Core/SubSystem/Graphics.h"

StaticTexture::StaticTexture(Context* context) : IResource(context, ResourceType::Texture2D) { }

bool StaticTexture::SaveToFile(const std::string& path) { return true; }

bool StaticTexture::LoadFromFile(const std::string& path)
{
    texturePath = path;                                                     // Animation ��Ʈ�� ����� TexturePath�� ������
    SetSpriteTexture(texturePath);

    return true;
}

void StaticTexture::SetSpriteTexture(const std::string& path)
{
    this->texturePath = path;

    texture = std::make_shared<D3D11_Texture>(context->GetSubSystem<Graphics>());
    texture->Create(path);
}

#include <algorithm>

#include "stdafx.h"
#include "Animation.h"

Animation::Animation(Context* const context) : IResource(context, ResourceType::Animation)
{
}

Animation::~Animation()
{
}

bool Animation::SaveToFile(const std::string& path)
{
    /*
        doc
        �� dec1
        �� Animation -> repeat_type, sprite_size...
            �� Ű������1 -> Ű������ ������
    */

    TiXmlDocument doc;   // ������ ���� ���� ��Ʈ
        
    TiXmlDeclaration* dec1 = doc.ToDeclaration();                                       // ���� ������ �޴� ������ ����
    doc.LinkEndChild(dec1);                                                             // ���� ���� �Է�
    
    TiXmlElement* root = new TiXmlElement("Animation");                                 // Animation ��Ʈ�� ����
    doc.LinkEndChild(root);

    for (int i = 0; i < resource_name.size(); i++)
    {
        TiXmlElement* spriteObject = new TiXmlElement("Sprite" + resource_name[i]);
        root->LinkEndChild(spriteObject);

        spriteObject->SetAttribute("Name", resource_name[i]);                          // Animation�� �̸��� ����
        spriteObject->SetAttribute("Type", static_cast<uint>(repeatType[i]));          // Animation�� �ݺ� ���θ� �˷���

        spriteObject->SetAttribute("TexturePath", texturePath[i].c_str());             // Animation�� �ؽ��� ��θ� �˷���

        spriteObject->SetAttribute("TextureSizeX", textureSize[i].x);                  // Animation�� �ؽ��� ����� �˷���
        spriteObject->SetAttribute("TextureSizeX", textureSize[i].y);

        for (const auto& keyframe : textureKeyFrames)
        {

            TiXmlElement* element = new TiXmlElement("Keyframe");   // keyframe�̶�� ��Ҹ� ����
            root->LinkEndChild(element);                            // keyframe�� root�� �ڽ� ��ҷ� ����

            element->SetAttribute("OffsetX", keyframe.second[i].offset.x);    // keyframe�� ������ ���� 
            element->SetAttribute("OffsetY", keyframe.second[i].offset.y);

            element->SetAttribute("SizeX", keyframe.second[i].size.x);        // keyframe�� ũ�� ����
            element->SetAttribute("SizeY", keyframe.second[i].size.y);

            element->SetAttribute("Time", keyframe.second[i].time);           // keyframe�� ���ð�
        }
    }

    return doc.SaveFile(path.c_str()); // ������ �����ϱ� ���� ���
}

bool Animation::LoadFromFile(const std::string& path)
{
    TiXmlDocument doc;
    bool fileOpen = doc.LoadFile(path.c_str());

    if (!fileOpen)
    {
        assert(false && "ERROR (Animation1) : �Է��� ��ο��� ������ ���� �� �� ����");
        return false;
    }

    TiXmlNode* data = doc.FirstChild("Animation");                        // Animation ��Ʈ�� ������, dec1�� �����÷��� FirstChild
    TiXmlNode* root = data->FirstChild();                                 // Animation �� �÷��̾��� ���¿� ���� ����Ǵ� �������̵尡 �޶����� �ؽ��� ��ü �޾ƿ�
    
    do
    {
        if (root == nullptr)
        {
            assert(false && "ERROR (Animation2) : XML���Ͽ��� �ִϸ��̼� ��ü�� ����");
            return false;
        }

        // �̸� (Name)
        animationName.push_back(root->ToElement()->Attribute("Name"));                              // Animation ��Ʈ�� ����� Name�� ������
        // �ݺ� Ÿ�� (Type)
        repeatType.push_back(static_cast<RepeatType>(atoi(root->ToElement()->Attribute("Type"))));  // Animation ��Ʈ�� ����� Type�� ������, ���ڿ� -> unsinged�� ��ȯ

        if (animationName.back() == "NW")
            std::cout << std::endl;
     
        // �ؽ��� ���丮 ��ġ (TexturePath)
        texturePath.push_back(root->ToElement()->Attribute("TexturePath"));                         // Animation ��Ʈ�� ����� TexturePath�� ������
        SetSpriteTexture(animationName.back(), texturePath.back());


     
        // Animation ��Ʈ�� ����� ������ ��ǥ�� ������, ���ڿ� -> float�� ��ȯ
        textureSize.push_back(D3DXVECTOR2(atof(root->ToElement()->Attribute("TextureSizeX")), atof(root->ToElement()->Attribute("TextureSizeY"))));
     
        for (auto element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            D3DXVECTOR2 offset;
            offset.x = atof(element->ToElement()->Attribute("OffsetX"));
            offset.y = atof(element->ToElement()->Attribute("OffsetY"));
     
            D3DXVECTOR2 size;
            size.x = atof(element->ToElement()->Attribute("SizeX"));
            size.y = atof(element->ToElement()->Attribute("SizeY"));
     
            float time = atof(element->ToElement()->Attribute("Time"));
     
            AddKeyframe(animationName.back(), offset, size, time);
        }

        // ���� Animation ��ü�� �̵�
        root = root->NextSibling();                                              
    } while (root != nullptr);

    return true;
}

const RepeatType& Animation::GetRepeatType(const std::string& animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return RepeatType::Loop;
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    // �ؽ��� ��ü�� ����Ǿ� �ִ� ��ġ�� �����Ͽ� �ε����� ��ȯ
    return repeatType[repeatIndex];
}

void Animation::SetRepeatType(const std::string animationName, const RepeatType& repeat_type)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return;
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    repeatType[repeatIndex] = repeat_type;
}

const D3DXVECTOR2& Animation::GetSpriteTextureSize(const std::string animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return D3DXVECTOR2();
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    return textureSize[repeatIndex];
} 

void Animation::SetSpriteTextureSize(const std::string animationName, const D3DXVECTOR2& size)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return;
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    textureSize[repeatIndex] = size;
}

void Animation::SetSpriteTexture(const std::string animationName, const std::string& path)
{
    unsigned int repeatIndex = textureKeyFrames.size();

    if (textureKeyFrames.find(animationName) != textureKeyFrames.end())
        repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));// �ش� �̸����� �ؽ��� ��ü�� ����Ǿ� �ִ� ��ġ�� �ε����� ��ȯ 

    texturePath[repeatIndex] = path;

    sprite_texture = std::make_shared<D3D11_Texture>(context->GetSubSystem<Graphics>());
    sprite_texture->Create(path);
}

const std::string& Animation::GetSpriteTexturePath(const std::string& animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return "";
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    return texturePath[repeatIndex];
} 

void Animation::SetSpriteTexturePath(const std::string& animationName, const std::string& path)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return;
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    texturePath[repeatIndex] = path;
}

void Animation::AddKeyframe(const std::string animationName, const Keyframe& keyframe)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        std::vector<Keyframe> newAnimation;
        newAnimation.push_back(keyframe);

        textureKeyFrames.insert(std::make_pair(animationName, newAnimation));
        return;
    }

    textureKeyFrames.at(animationName).emplace_back(keyframe);
}

void Animation::AddKeyframe(const std::string animationName, const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        std::vector<Keyframe> newAnimation;
        newAnimation.push_back(Keyframe(offset, size, time));

        textureKeyFrames.insert(std::make_pair(animationName, newAnimation));
        return;
    }

    textureKeyFrames.at(animationName).emplace_back(offset, size, time);
}

const Keyframe* const Animation::GetKeyframeFromIndex(const std::string animationName, const uint& index)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return nullptr;
    }

    if (index >= textureKeyFrames.at(animationName).size())
    {
        assert(false && "ERROR (Animation4) : �ش� �̸��� �ִϸ��̼� �� �ش� �ε����� �������� ����");
        return nullptr;
    }

    return &textureKeyFrames.at(animationName)[index];
}

const size_t Animation::GetKeyframeCount(const std::string animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : �ش� �̸��� �ִϸ��̼��� �������� ����");
        return 0;
    }

    return textureKeyFrames.at(animationName).size();
}
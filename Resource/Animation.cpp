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
        ㄴ dec1
        ㄴ Animation -> repeat_type, sprite_size...
            ㄴ 키프라임1 -> 키프라임 데이터
    */

    TiXmlDocument doc;   // 파일을 쓰기 위한 루트
        
    TiXmlDeclaration* dec1 = doc.ToDeclaration();                                       // 파일 정보를 받는 변수를 선언
    doc.LinkEndChild(dec1);                                                             // 파일 정보 입력
    
    TiXmlElement* root = new TiXmlElement("Animation");                                 // Animation 루트를 만듬
    doc.LinkEndChild(root);

    for (int i = 0; i < resource_name.size(); i++)
    {
        TiXmlElement* spriteObject = new TiXmlElement("Sprite" + resource_name[i]);
        root->LinkEndChild(spriteObject);

        spriteObject->SetAttribute("Name", resource_name[i]);                          // Animation의 이름을 정함
        spriteObject->SetAttribute("Type", static_cast<uint>(repeatType[i]));          // Animation의 반복 여부를 알려줌

        spriteObject->SetAttribute("TexturePath", texturePath[i].c_str());             // Animation의 텍스쳐 경로를 알려줌

        spriteObject->SetAttribute("TextureSizeX", textureSize[i].x);                  // Animation의 텍스쳐 사이즈를 알려줌
        spriteObject->SetAttribute("TextureSizeX", textureSize[i].y);

        for (const auto& keyframe : textureKeyFrames)
        {

            TiXmlElement* element = new TiXmlElement("Keyframe");   // keyframe이라는 요소를 만듬
            root->LinkEndChild(element);                            // keyframe을 root의 자식 요소로 만듬

            element->SetAttribute("OffsetX", keyframe.second[i].offset.x);    // keyframe의 오프셋 설정 
            element->SetAttribute("OffsetY", keyframe.second[i].offset.y);

            element->SetAttribute("SizeX", keyframe.second[i].size.x);        // keyframe의 크기 설정
            element->SetAttribute("SizeY", keyframe.second[i].size.y);

            element->SetAttribute("Time", keyframe.second[i].time);           // keyframe의 대기시간
        }
    }

    return doc.SaveFile(path.c_str()); // 파일을 저장하기 위한 경로
}

bool Animation::LoadFromFile(const std::string& path)
{
    TiXmlDocument doc;
    bool fileOpen = doc.LoadFile(path.c_str());

    if (!fileOpen)
    {
        assert(false && "ERROR (Animation1) : 입력한 경로에서 파일을 가져 올 수 없음");
        return false;
    }

    TiXmlNode* data = doc.FirstChild("Animation");                        // Animation 루트를 가져옴, dec1를 가져올려면 FirstChild
    TiXmlNode* root = data->FirstChild();                                 // Animation 중 플레이어의 상태에 따라 실행되는 스프라이드가 달라지는 텍스쳐 객체 받아옴
    
    do
    {
        if (root == nullptr)
        {
            assert(false && "ERROR (Animation2) : XML파일에서 애니메이션 객체가 없음");
            return false;
        }

        // 이름 (Name)
        animationName.push_back(root->ToElement()->Attribute("Name"));                              // Animation 루트에 저장된 Name을 가져옴
        // 반복 타입 (Type)
        repeatType.push_back(static_cast<RepeatType>(atoi(root->ToElement()->Attribute("Type"))));  // Animation 루트에 저장된 Type을 가져옴, 문자열 -> unsinged로 변환

        if (animationName.back() == "NW")
            std::cout << std::endl;
     
        // 텍스쳐 디렉토리 위치 (TexturePath)
        texturePath.push_back(root->ToElement()->Attribute("TexturePath"));                         // Animation 루트에 저장된 TexturePath를 가져옴
        SetSpriteTexture(animationName.back(), texturePath.back());


     
        // Animation 루트에 저장된 덱스쳐 좌표을 가져옴, 문자열 -> float로 변환
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

        // 다음 Animation 객체로 이동
        root = root->NextSibling();                                              
    } while (root != nullptr);

    return true;
}

const RepeatType& Animation::GetRepeatType(const std::string& animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
        return RepeatType::Loop;
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    // 텍스쳐 객체에 저장되어 있는 위치를 연계하여 인덱스를 반환
    return repeatType[repeatIndex];
}

void Animation::SetRepeatType(const std::string animationName, const RepeatType& repeat_type)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
        return;
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    repeatType[repeatIndex] = repeat_type;
}

const D3DXVECTOR2& Animation::GetSpriteTextureSize(const std::string animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
        return D3DXVECTOR2();
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    return textureSize[repeatIndex];
} 

void Animation::SetSpriteTextureSize(const std::string animationName, const D3DXVECTOR2& size)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
        return;
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    textureSize[repeatIndex] = size;
}

void Animation::SetSpriteTexture(const std::string animationName, const std::string& path)
{
    unsigned int repeatIndex = textureKeyFrames.size();

    if (textureKeyFrames.find(animationName) != textureKeyFrames.end())
        repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));// 해당 이름으로 텍스쳐 객체에 저장되어 있는 위치를 인덱스로 변환 

    texturePath[repeatIndex] = path;

    sprite_texture = std::make_shared<D3D11_Texture>(context->GetSubSystem<Graphics>());
    sprite_texture->Create(path);
}

const std::string& Animation::GetSpriteTexturePath(const std::string& animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
        return "";
    }

    unsigned int repeatIndex = std::distance(textureKeyFrames.begin(), textureKeyFrames.find(animationName));

    return texturePath[repeatIndex];
} 

void Animation::SetSpriteTexturePath(const std::string& animationName, const std::string& path)
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
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
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
        return nullptr;
    }

    if (index >= textureKeyFrames.at(animationName).size())
    {
        assert(false && "ERROR (Animation4) : 해당 이름의 애니메이션 중 해당 인덱스는 존재하지 않음");
        return nullptr;
    }

    return &textureKeyFrames.at(animationName)[index];
}

const size_t Animation::GetKeyframeCount(const std::string animationName) const
{
    if (textureKeyFrames.find(animationName) == textureKeyFrames.end())
    {
        assert(false && "ERROR (Animation3) : 해당 이름의 애니메이션이 존재하지 않음");
        return 0;
    }

    return textureKeyFrames.at(animationName).size();
}
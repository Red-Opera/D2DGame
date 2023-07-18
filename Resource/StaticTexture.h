// StaticTexture.h
// 정적 텍스쳐를 불러오는 파일

#pragma once
#include "IResource.h"
class StaticTexture : public IResource
{
public:
	StaticTexture(Context* const context);
	~StaticTexture() = default;

	// IResource을(를) 통해 상속됨
	virtual bool SaveToFile(const std::string& path) override;
	virtual bool LoadFromFile(const std::string& path) override;
	
	// ===============================================================
	// 텍스쳐 관련
	// ===============================================================

	// 텍스쳐 사이즈 관련 메소드
	const D3DXVECTOR2& GetSpriteTextureSize() const { return textureSize; }
	void SetSpriteTextureSize(const D3DXVECTOR2& size) { this->textureSize = size; }

	// 텍스쳐의 오프셋 관련 메소드
	const D3DXVECTOR2& GetSpriteTextureOffset() const { return textureOffset; }
	void SetSpriteTextureOffset(const D3DXVECTOR2& offset) { this->textureOffset = offset; }

	// 텍스쳐 데이터 관련 메소드
	const std::shared_ptr<class D3D11_Texture>& GetSpriteTexture() const { return texture; }
	void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& texture) { this->texture = texture; }

	// 화면 최대 설정 (기본 텍스트 크기 X : 50px, Y : 50px)
	const void FullSize(bool isFullSize = true, float sizeX = 50.0f, float sizeY = 50.0f)
	{
		this->isFullSize = isFullSize;

		isFullSize ? textureSize = D3DXVECTOR2(Settings::Get().GetWidth(), Settings::Get().GetHeight()) : D3DXVECTOR2(sizeX, sizeY);
	}
	const bool IsFullSize() const { return isFullSize; }

	// ===============================================================
	// 텍스쳐의 파일 관련
	// ===============================================================

	// 파일 경로 등록 및 텍스쳐 등록 메소드
	void SetSpriteTexture(const std::string& path);

	// 파일 경로 관련 메소드
	const std::string& GetSpriteTexturePath() const { return texturePath; }
	void SetSpriteTexturePath(const std::string& path) { this->texturePath = path; }

private:
	bool isFullSize = false;								// 텍스쳐 전체 화면 여부

	std::shared_ptr<class D3D11_Texture> texture;			// 텍스쳐 데이터
	D3DXVECTOR2 textureSize = D3DXVECTOR2(100.0f, 100.0f);	// 가져올 텍스쳐의 크기 (픽셀 단위)
	D3DXVECTOR2 textureOffset = D3DXVECTOR2(0.0f, 0.0f);	// 텍스쳐의 오프셋
	std::string texturePath = "";							// 스프라이트가 있는 경로
};


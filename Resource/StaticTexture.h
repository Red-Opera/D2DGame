// StaticTexture.h
// ���� �ؽ��ĸ� �ҷ����� ����

#pragma once
#include "IResource.h"
class StaticTexture : public IResource
{
public:
	StaticTexture(Context* const context);
	~StaticTexture() = default;

	// IResource��(��) ���� ��ӵ�
	virtual bool SaveToFile(const std::string& path) override;
	virtual bool LoadFromFile(const std::string& path) override;
	
	// ===============================================================
	// �ؽ��� ����
	// ===============================================================

	// �ؽ��� ������ ���� �޼ҵ�
	const D3DXVECTOR2& GetSpriteTextureSize() const { return textureSize; }
	void SetSpriteTextureSize(const D3DXVECTOR2& size) { this->textureSize = size; }

	// �ؽ����� ������ ���� �޼ҵ�
	const D3DXVECTOR2& GetSpriteTextureOffset() const { return textureOffset; }
	void SetSpriteTextureOffset(const D3DXVECTOR2& offset) { this->textureOffset = offset; }

	// �ؽ��� ������ ���� �޼ҵ�
	const std::shared_ptr<class D3D11_Texture>& GetSpriteTexture() const { return texture; }
	void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& texture) { this->texture = texture; }

	// ȭ�� �ִ� ���� (�⺻ �ؽ�Ʈ ũ�� X : 50px, Y : 50px)
	const void FullSize(bool isFullSize = true, float sizeX = 50.0f, float sizeY = 50.0f)
	{
		this->isFullSize = isFullSize;

		isFullSize ? textureSize = D3DXVECTOR2(Settings::Get().GetWidth(), Settings::Get().GetHeight()) : D3DXVECTOR2(sizeX, sizeY);
	}
	const bool IsFullSize() const { return isFullSize; }

	// ===============================================================
	// �ؽ����� ���� ����
	// ===============================================================

	// ���� ��� ��� �� �ؽ��� ��� �޼ҵ�
	void SetSpriteTexture(const std::string& path);

	// ���� ��� ���� �޼ҵ�
	const std::string& GetSpriteTexturePath() const { return texturePath; }
	void SetSpriteTexturePath(const std::string& path) { this->texturePath = path; }

private:
	bool isFullSize = false;								// �ؽ��� ��ü ȭ�� ����

	std::shared_ptr<class D3D11_Texture> texture;			// �ؽ��� ������
	D3DXVECTOR2 textureSize = D3DXVECTOR2(100.0f, 100.0f);	// ������ �ؽ����� ũ�� (�ȼ� ����)
	D3DXVECTOR2 textureOffset = D3DXVECTOR2(0.0f, 0.0f);	// �ؽ����� ������
	std::string texturePath = "";							// ��������Ʈ�� �ִ� ���
};


#pragma once

#include <map>
#include <string>

static class TempTexture
{
public:
	static void SetTexture(std::string name, struct ID3D11ShaderResourceView* texture) { tempTexture.emplace(name, texture); }
	static struct ID3D11ShaderResourceView* GetTexture(std::string name) { return tempTexture[name]; }

	static bool HasTexture(std::string name) { return tempTexture.find(name) != tempTexture.end(); }

private:
	static std::map<std::string, struct ID3D11ShaderResourceView*> tempTexture;
};
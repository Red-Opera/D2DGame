#pragma once

enum ShaderScope : uint
{
	ShaderScope_Unknown = 0U,
	ShaderScope_VS = 1U << 0,
	ShaderScope_PS = 1U << 1,
};

enum class CompilationState : uint
{
	Unknown,
	Compling,
	Succeeded,
	Failed
};

class D3D11_Shader final : public Object
{
public:
	D3D11_Shader(Graphics* graphics);
	~D3D11_Shader();

	void* GetResource() const { return resource; }
	bool HasResource() const { return resource != nullptr; }
	ID3DBlob* GetShaderBlob() const { return shader_blob; }

	const std::string& GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }

	const std::string& GetPath() const { return path;}
	const ShaderScope& GetShaderScope() const { return shader_scope; }
	const CompilationState& GetCompilationState() const { return compilation_state; }

	const char* GetEntryPoint() const;
	const char* GetTargetProfile() const;
	const char* GetShaderModel() const;

	bool IsCompiled() const { return compilation_state == CompilationState::Succeeded; }
	
	void Create(const ShaderScope& scope, const std::string& shader);
	void Clear();

private:
	void* Compile(const ShaderScope& scope, const std::string& shader);

private:
	ID3D11Device* device = nullptr;
	ID3DBlob* shader_blob = nullptr;

	void* resource = nullptr;

	std::string name = ""; 
	std::string path = "";

	ShaderScope shader_scope = ShaderScope_Unknown;
	CompilationState compilation_state = CompilationState::Unknown;
};
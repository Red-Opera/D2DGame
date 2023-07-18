#include "stdafx.h"
#include "IResource.h"

IResource::IResource(Context* const context, const ResourceType type) : context(context), resource_type(type)
{

}

template<typename T>
constexpr ResourceType IResource::DeduceResourceType()
{
	return ResourceType::Unknown;
}

#define REGISTER_RESOURCE_TYPE(T, enum_type) template <> ResourceType IResource::DeduceResourceType<T>() { return enum_type; }
REGISTER_RESOURCE_TYPE(Animation, ResourceType::Animation)
REGISTER_RESOURCE_TYPE(StaticTexture, ResourceType::Texture2D)
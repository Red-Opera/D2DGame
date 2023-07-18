// Object.h
#include "stdafx.h"

#pragma once

class Object
{
public:
	Object() { id = GUID_Generator::Generate(); }
	virtual ~Object() = default;

	uint GetID() const { return id; }
	void SetId(const uint& id) { this->id = id; }
private:
	uint id = 0;
};
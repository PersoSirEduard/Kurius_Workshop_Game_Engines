#pragma once

#include "GL/glew.h"
#include <vector>

struct VertexBufferAttribute
{
	unsigned int type;
	unsigned int count;
	bool normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
		}

		return 0;
	}
};

class VertexBufferLayout
{
private:
	unsigned int stride;
	std::vector<VertexBufferAttribute> attributes;
public:

	VertexBufferLayout()
		: stride(0) {};

	template<typename T>
	void Push(unsigned int count)
	{

	}

	template<>
	void Push<float>(unsigned int count)
	{
		attributes.push_back({ GL_FLOAT, count, false });
		stride += VertexBufferAttribute::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		attributes.push_back({ GL_UNSIGNED_INT, count, false });
		stride += VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	inline std::vector<VertexBufferAttribute> GetAttributes() const { return attributes;  }
	inline unsigned int GetStride() const { return stride; }
};
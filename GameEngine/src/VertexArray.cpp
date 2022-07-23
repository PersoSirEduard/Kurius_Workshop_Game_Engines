#include "VertexArray.h"
#include <vector>

VertexArray::VertexArray()
	:rendererID(0)
{
	glGenVertexArrays(1, &rendererID);
	Bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const std::vector<VertexBufferAttribute>& attribs = layout.GetAttributes();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < attribs.size(); i++)
	{
		const VertexBufferAttribute& attrib = attribs[i];

		glEnableVertexAttribArray(i); // Enable attrib index on vertex array
		glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset);
		offset += attrib.count * VertexBufferAttribute::GetSizeOfType(attrib.type);
	}
}
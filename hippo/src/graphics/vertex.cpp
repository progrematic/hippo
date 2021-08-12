#include "hippo/graphics/vertex.h"
#include "hippo/graphics/helpers.h"

#include "glad/glad.h"

namespace hippo::graphics
{
	const uint32_t RawVertexBuffer::GLTypeByte = GL_BYTE;
	const uint32_t RawVertexBuffer::GLTypeUByte = GL_UNSIGNED_BYTE;
	const uint32_t RawVertexBuffer::GLTypeShort = GL_SHORT;
	const uint32_t RawVertexBuffer::GLTypeUShort = GL_UNSIGNED_SHORT;
	const uint32_t RawVertexBuffer::GLTypeInt = GL_INT;
	const uint32_t RawVertexBuffer::GLTypeUInt = GL_UNSIGNED_INT;
	const uint32_t RawVertexBuffer::GLTypeFloat = GL_FLOAT;
	const uint32_t RawVertexBuffer::GLTypeDouble = GL_DOUBLE;

	RawVertexBuffer::RawVertexBuffer()
	{
		glGenBuffers(1, &mVbo); HIPPO_CHECK_GL_ERROR;
	}

	RawVertexBuffer::~RawVertexBuffer()
	{
		glDeleteBuffers(1, &mVbo); HIPPO_CHECK_GL_ERROR;
	}

	void RawVertexBuffer::SetLayout(const std::vector<uint32_t>& layout)
	{
		mLayout = layout;
		mStride = 0;
		for (auto& count : layout)
		{
			mStride += count;
		}
	}

	void RawVertexBuffer::Upload(bool dynamic /*= false*/)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVbo); HIPPO_CHECK_GL_ERROR;
		glBufferData(GL_ARRAY_BUFFER, mSize, mData, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); HIPPO_CHECK_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, 0); HIPPO_CHECK_GL_ERROR;
		mIsUploaded = true;
	}

	void RawVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVbo); HIPPO_CHECK_GL_ERROR;
	}

	void RawVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0); HIPPO_CHECK_GL_ERROR;
	}

	VertexArray::VertexArray()
		: mVao(0)
		, mEbo(0)
		, mAttributeCount(0)
		, mVertexCount(0)
		, mElementCount(0)
		, mIsValid(false)
	{
		glGenVertexArrays(1, &mVao); HIPPO_CHECK_GL_ERROR;
	}

	VertexArray::~VertexArray()
	{
		uint32_t id;
		for (auto& vbo : mVbos)
		{
			id = vbo->GetId();
			glDeleteBuffers(1, &id); HIPPO_CHECK_GL_ERROR;
			delete vbo;
		}
		glDeleteVertexArrays(1, &mVao); HIPPO_CHECK_GL_ERROR;
		mVbos.clear();
	}

	void VertexArray::PushBuffer(RawVertexBuffer* vbo)
	{
		if (mVbos.size() > 0)
		{
			HIPPO_ASSERT(mVbos[0]->GetVertexCount() == vbo->GetVertexCount(), "VertexArray::PushBuffer - Attempting to push a VertexBuffer with a different VertexCount");
		}
		HIPPO_ASSERT(vbo->GetLayout().size() > 0, "VertexArray::PushBuffer - VertexBuffer has no layout defined");
		if (vbo->GetLayout().size() > 0)
		{
			mVbos.push_back(vbo);
			mVertexCount = (uint32_t)mVbos[0]->GetVertexCount();
		}
	}

	void VertexArray::SetElements(const std::vector<uint32_t>& elements)
	{
		mElementCount = (uint32_t)elements.size();
		glBindVertexArray(mVao); HIPPO_CHECK_GL_ERROR;
		glGenBuffers(1, &mEbo); HIPPO_CHECK_GL_ERROR;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo); HIPPO_CHECK_GL_ERROR;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(uint32_t), &elements[0], GL_STATIC_DRAW); HIPPO_CHECK_GL_ERROR;
		glBindVertexArray(0); HIPPO_CHECK_GL_ERROR;
	}

	void VertexArray::Upload()
	{
		glBindVertexArray(mVao); HIPPO_CHECK_GL_ERROR;
		uint32_t attributeCount = 0;
		for (auto& vbo : mVbos)
		{
			if (!vbo->IsUploaded())
			{
				vbo->Upload(false);
			}
			vbo->Bind();
			uint32_t offset = 0;
			for (uint32_t count : vbo->GetLayout())
			{
				glEnableVertexAttribArray(attributeCount); HIPPO_CHECK_GL_ERROR;
				glVertexAttribPointer(attributeCount, count, static_cast<GLenum>(vbo->GetGLType()), GL_FALSE, vbo->GetStride(), (void*)(intptr_t)offset); HIPPO_CHECK_GL_ERROR;

				attributeCount++;
				offset += (count * vbo->GetTypeSize());
			}
			vbo->Unbind();
		}
		glBindVertexArray(0); HIPPO_CHECK_GL_ERROR;
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(mVao); HIPPO_CHECK_GL_ERROR;
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0); HIPPO_CHECK_GL_ERROR;
	}

}
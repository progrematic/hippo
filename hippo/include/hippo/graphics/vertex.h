#pragma once

#include <vector>
#include <type_traits>

namespace hippo::graphics
{
	class RawVertexBuffer
	{
	public:
		static const uint32_t GLTypeByte;
		static const uint32_t GLTypeUByte;
		static const uint32_t GLTypeShort;
		static const uint32_t GLTypeUShort;
		static const uint32_t GLTypeInt;
		static const uint32_t GLTypeUInt;
		static const uint32_t GLTypeFloat;
		static const uint32_t GLTypeDouble;

	public:
		RawVertexBuffer();
		virtual ~RawVertexBuffer();

		virtual uint32_t GetTypeSize() const = 0;

		inline bool IsUploaded() const { return mIsUploaded; }
		inline uint32_t GetId() const { return mVbo; }
		inline uint32_t GetVertexCount() const { return mVertexCount; }
		inline uint32_t GetStride() const { return mStride; }
		inline uint32_t GetGLType() const { return mGLType; }
		inline const std::vector<uint32_t>& GetLayout() const { return mLayout; }

		void SetLayout(const std::vector<uint32_t>& layout);

		virtual void Upload(bool dynamic = false);

		void Bind();
		void Unbind();

	protected:
		bool mIsUploaded = false;
		uint32_t mVbo = 0;
		uint32_t mVertexCount = 0;
		uint32_t mStride = 0;

		std::vector<uint32_t> mLayout;
		void* mData = nullptr;
		uint32_t mSize = 0;
		uint32_t mGLType = 0;
	};

	template<typename T>
	class VertexBuffer : public RawVertexBuffer
	{
		static_assert(
			std::is_same<T, char>() ||				// GL_BYTE
			std::is_same<T, unsigned char>() ||		// GL_UNSIGNED_BYTE
			std::is_same<T, short>() ||				// GL_SHORT
			std::is_same<T, unsigned short>() ||	// GL_UNSIGNED_SHORT
			std::is_same<T, int>() ||				// GL_INT
			std::is_same<T, unsigned int>() ||		// GL_UNSIGNED_INT
			std::is_same<T, float>() ||				// GL_FLOAT
			std::is_same<T, double>()				// GL_DOUBLE
			, "This type is not supported");
	public:
		VertexBuffer()
		{
			if constexpr (std::is_same<T, char>())			{ mGLType = RawVertexBuffer::GLTypeByte; }
			if constexpr (std::is_same<T, unsigned char>()) { mGLType = RawVertexBuffer::GLTypeUByte; }
			if constexpr (std::is_same<T, short>())			{ mGLType = RawVertexBuffer::GLTypeShort; }
			if constexpr (std::is_same<T, unsigned short>()){ mGLType = RawVertexBuffer::GLTypeUShort; }
			if constexpr (std::is_same<T, int>())			{ mGLType = RawVertexBuffer::GLTypeInt; }
			if constexpr (std::is_same<T, unsigned int>())	{ mGLType = RawVertexBuffer::GLTypeUInt; }
			if constexpr (std::is_same<T, float>())			{ mGLType = RawVertexBuffer::GLTypeFloat; }
			if constexpr (std::is_same<T, double>())		{ mGLType = RawVertexBuffer::GLTypeDouble; }
		}
		~VertexBuffer() {}

		uint32_t GetTypeSize() const override { return sizeof(T); }

		void PushVertex(const std::vector<T>& vert)
		{
			mVertexCount++;
			mDataVec.insert(mDataVec.end(), vert.begin(), vert.end());
		}

		void Upload(bool dynamic = false) override
		{
			mStride *= sizeof(T);
			mSize = sizeof(T) * (uint32_t)mDataVec.size();
			HIPPO_TRACE("VertexBuffer::Upload() - mSize: {}, mStride: {}", mSize, mStride);
			HIPPO_ASSERT(mSize > 0, "VertexBuffer::Upload() - mSize = 0");
			mData = &mDataVec[0];
			RawVertexBuffer::Upload(dynamic);
		}

	private:
		std::vector<T> mDataVec;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		inline bool IsValid() const { return mIsValid; }
		inline uint32_t GetVertexCount() const { return mVertexCount; }
		inline uint32_t GetElementCount() const { return mElementCount; }

		void PushBuffer(RawVertexBuffer* vbo);
		void SetElements(const std::vector<uint32_t>& elements);

		void Upload();

		void Bind();
		void Unbind();

	private:
		bool mIsValid;
		uint32_t mVertexCount, mElementCount;
		uint32_t mVao, mEbo;
		uint32_t mAttributeCount;
		std::vector<RawVertexBuffer*> mVbos;
	};
}
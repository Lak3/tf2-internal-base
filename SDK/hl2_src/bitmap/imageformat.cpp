//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#if defined( _WIN32 ) && !defined( _X360 ) && !defined( DX_TO_GL_ABSTRACTION )
#include <Windows.h>
#endif
#include "../public/bitmap/imageformat.h"
#include "../public/tier0/basetypes.h"
#include "../public/tier0/dbg.h"
#include "../public/mathlib/mathlib.h"
#include "../public/mathlib/vector.h"
#include "../public/tier1/utlmemory.h"
#include "../public/tier1/strtools.h"
#include "../public/mathlib/compressed_vector.h"

//-----------------------------------------------------------------------------
// Various important function types for each color format
//-----------------------------------------------------------------------------
static const ImageFormatInfo_t g_ImageFormatInfo[] =
{
	{ "UNKNOWN",					0, 0, 0, 0, 0, false },			// IMAGE_FORMAT_UNKNOWN,
	{ "RGBA8888",					4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_RGBA8888,
	{ "ABGR8888",					4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_ABGR8888, 
	{ "RGB888",						3, 8, 8, 8, 0, false },			// IMAGE_FORMAT_RGB888,
	{ "BGR888",						3, 8, 8, 8, 0, false },			// IMAGE_FORMAT_BGR888,
	{ "RGB565",						2, 5, 6, 5, 0, false },			// IMAGE_FORMAT_RGB565, 
	{ "I8",							1, 0, 0, 0, 0, false },			// IMAGE_FORMAT_I8,
	{ "IA88",						2, 0, 0, 0, 8, false },			// IMAGE_FORMAT_IA88
	{ "P8",							1, 0, 0, 0, 0, false },			// IMAGE_FORMAT_P8
	{ "A8",							1, 0, 0, 0, 8, false },			// IMAGE_FORMAT_A8
	{ "RGB888_BLUESCREEN",			3, 8, 8, 8, 0, false },			// IMAGE_FORMAT_RGB888_BLUESCREEN
	{ "BGR888_BLUESCREEN",			3, 8, 8, 8, 0, false },			// IMAGE_FORMAT_BGR888_BLUESCREEN
	{ "ARGB8888",					4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_ARGB8888
	{ "BGRA8888",					4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_BGRA8888
	{ "DXT1",						0, 0, 0, 0, 0, true },			// IMAGE_FORMAT_DXT1
	{ "DXT3",						0, 0, 0, 0, 8, true },			// IMAGE_FORMAT_DXT3
	{ "DXT5",						0, 0, 0, 0, 8, true },			// IMAGE_FORMAT_DXT5
	{ "BGRX8888",					4, 8, 8, 8, 0, false },			// IMAGE_FORMAT_BGRX8888
	{ "BGR565",						2, 5, 6, 5, 0, false },			// IMAGE_FORMAT_BGR565
	{ "BGRX5551",					2, 5, 5, 5, 0, false },			// IMAGE_FORMAT_BGRX5551
	{ "BGRA4444",					2, 4, 4, 4, 4, false },			// IMAGE_FORMAT_BGRA4444
	{ "DXT1_ONEBITALPHA",			0, 0, 0, 0, 0, true },			// IMAGE_FORMAT_DXT1_ONEBITALPHA
	{ "BGRA5551",					2, 5, 5, 5, 1, false },			// IMAGE_FORMAT_BGRA5551
	{ "UV88",						2, 8, 8, 0, 0, false },			// IMAGE_FORMAT_UV88
	{ "UVWQ8888",					4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_UVWQ8899
	{ "RGBA16161616F",				8, 16, 16, 16, 16, false },		// IMAGE_FORMAT_RGBA16161616F
	{ "RGBA16161616",				8, 16, 16, 16, 16, false },		// IMAGE_FORMAT_RGBA16161616
	{ "IMAGE_FORMAT_UVLX8888",	    4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_UVLX8899
	{ "IMAGE_FORMAT_R32F",			4, 32, 0, 0, 0, false },		// IMAGE_FORMAT_R32F
	{ "IMAGE_FORMAT_RGB323232F",	12, 32, 32, 32, 0, false },		// IMAGE_FORMAT_RGB323232F
	{ "IMAGE_FORMAT_RGBA32323232F",	16, 32, 32, 32, 32, false },	// IMAGE_FORMAT_RGBA32323232F

	// Vendor-dependent depth formats used for shadow depth mapping
	{ "NV_DST16",					2, 16, 0, 0, 0, false },		// IMAGE_FORMAT_NV_DST16
	{ "NV_DST24",					4, 24, 0, 0, 0, false },		// IMAGE_FORMAT_NV_DST24
	{ "NV_INTZ",					4,  8, 8, 8, 8, false },		// IMAGE_FORMAT_NV_INTZ
	{ "NV_RAWZ",					4, 24, 0, 0, 0, false },		// IMAGE_FORMAT_NV_RAWZ
	{ "ATI_DST16",					2, 16, 0, 0, 0, false },		// IMAGE_FORMAT_ATI_DST16
	{ "ATI_DST24",					4, 24, 0, 0, 0, false },		// IMAGE_FORMAT_ATI_DST24
	{ "NV_NULL",					4,  8, 8, 8, 8, false },		// IMAGE_FORMAT_NV_NULL

	// Vendor-dependent compressed formats typically used for normal map compression
	{ "ATI1N",						0, 0, 0, 0, 0, true },			// IMAGE_FORMAT_ATI1N
	{ "ATI2N",						0, 0, 0, 0, 0, true },			// IMAGE_FORMAT_ATI2N

#ifdef _X360
	{ "X360_DST16",					2, 16, 0, 0, 0, false },		// IMAGE_FORMAT_X360_DST16
	{ "X360_DST24",					4, 24, 0, 0, 0, false },		// IMAGE_FORMAT_X360_DST24
	{ "X360_DST24F",				4, 24, 0, 0, 0, false },		// IMAGE_FORMAT_X360_DST24F
	{ "LINEAR_BGRX8888",			4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_LINEAR_BGRX8888
	{ "LINEAR_RGBA8888",			4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_LINEAR_RGBA8888
	{ "LINEAR_ABGR8888",			4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_LINEAR_ABGR8888
	{ "LINEAR_ARGB8888",			4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_LINEAR_ARGB8888
	{ "LINEAR_BGRA8888",			4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_LINEAR_BGRA8888
	{ "LINEAR_RGB888",				3, 8, 8, 8, 0, false },			// IMAGE_FORMAT_LINEAR_RGB888
	{ "LINEAR_BGR888",				3, 8, 8, 8, 0, false },			// IMAGE_FORMAT_LINEAR_BGR888
	{ "LINEAR_BGRX5551",			2, 5, 5, 5, 0, false },			// IMAGE_FORMAT_LINEAR_BGRX5551
	{ "LINEAR_I8",					1, 0, 0, 0, 0, false },			// IMAGE_FORMAT_LINEAR_I8
	{ "LINEAR_RGBA16161616",		8, 16, 16, 16, 16, false },		// IMAGE_FORMAT_LINEAR_RGBA16161616

	{ "LE_BGRX8888",				4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_LE_BGRX8888
	{ "LE_BGRA8888",				4, 8, 8, 8, 8, false },			// IMAGE_FORMAT_LE_BGRA8888
#endif

	{ "DXT1_RUNTIME",				0, 0, 0, 0, 0, true, },			// IMAGE_FORMAT_DXT1_RUNTIME
	{ "DXT5_RUNTIME",				0, 0, 0, 0, 8, true, },			// IMAGE_FORMAT_DXT5_RUNTIME
};

namespace ImageLoader
{
	//-----------------------------------------------------------------------------
	// Returns info about each image format
	//-----------------------------------------------------------------------------
	const ImageFormatInfo_t& ImageFormatInfo(ImageFormat fmt)
	{
		assert((NUM_IMAGE_FORMATS + 1) == sizeof(g_ImageFormatInfo) / sizeof(g_ImageFormatInfo[0]));
		assert(unsigned(fmt + 1) <= (NUM_IMAGE_FORMATS));
		return g_ImageFormatInfo[fmt + 1];
	}

	int GetMemRequired(int width, int height, int depth, ImageFormat imageFormat, bool mipmap)
	{
		if (depth <= 0)
		{
			depth = 1;
		}

		if (!mipmap)
		{
			// Block compressed formats

			if (IsCompressed(imageFormat))
			{
				/*
							DDSURFACEDESC desc;
							memset( &desc, 0, sizeof(desc) );

							DWORD dwEncodeType;
							dwEncodeType = GetDXTCEncodeType( imageFormat );
							desc.dwSize = sizeof( desc );
							desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
							desc.dwWidth = width;
							desc.dwHeight = height;
							return S3TCgetEncodeSize( &desc, dwEncodeType );
				*/
				assert((width < 4) || !(width % 4));
				assert((height < 4) || !(height % 4));
				assert((depth < 4) || !(depth % 4));
				if (width < 4 && width > 0)
				{
					width = 4;
				}
				if (height < 4 && height > 0)
				{
					height = 4;
				}
				if (depth < 4 && depth > 1)
				{
					depth = 4;
				}
				int numBlocks = (width * height) >> 4;
				numBlocks *= depth;
				switch (imageFormat)
				{
				case IMAGE_FORMAT_DXT1:
				case IMAGE_FORMAT_DXT1_RUNTIME:
				case IMAGE_FORMAT_ATI1N:
					return numBlocks * 8;

				case IMAGE_FORMAT_DXT3:
				case IMAGE_FORMAT_DXT5:
				case IMAGE_FORMAT_DXT5_RUNTIME:
				case IMAGE_FORMAT_ATI2N:
					return numBlocks * 16;
				}

				assert(0);
				return 0;
			}

			return width * height * depth * SizeInBytes(imageFormat);
		}

		// Mipmap version
		int memSize = 0;
		while (1)
		{
			memSize += GetMemRequired(width, height, depth, imageFormat, false);
			if (width == 1 && height == 1 && depth == 1)
			{
				break;
			}
			width >>= 1;
			height >>= 1;
			depth >>= 1;
			if (width < 1)
			{
				width = 1;
			}
			if (height < 1)
			{
				height = 1;
			}
			if (depth < 1)
			{
				depth = 1;
			}
		}

		return memSize;
	}

	int GetMipMapLevelByteOffset(int width, int height, ImageFormat imageFormat, int skipMipLevels)
	{
		int offset = 0;

		while (skipMipLevels > 0)
		{
			offset += width * height * SizeInBytes(imageFormat);
			if (width == 1 && height == 1)
			{
				break;
			}
			width >>= 1;
			height >>= 1;
			if (width < 1)
			{
				width = 1;
			}
			if (height < 1)
			{
				height = 1;
			}
			skipMipLevels--;
		}
		return offset;
	}

	void GetMipMapLevelDimensions(int* width, int* height, int skipMipLevels)
	{
		while (skipMipLevels > 0)
		{
			if (*width == 1 && *height == 1)
			{
				break;
			}
			*width >>= 1;
			*height >>= 1;
			if (*width < 1)
			{
				*width = 1;
			}
			if (*height < 1)
			{
				*height = 1;
			}
			skipMipLevels--;
		}
	}

	int GetNumMipMapLevels(int width, int height, int depth)
	{
		if (depth <= 0)
		{
			depth = 1;
		}

		if (width < 1 || height < 1 || depth < 1)
			return 0;

		int numMipLevels = 1;
		while (1)
		{
			if (width == 1 && height == 1 && depth == 1)
				break;

			width >>= 1;
			height >>= 1;
			depth >>= 1;
			if (width < 1)
			{
				width = 1;
			}
			if (height < 1)
			{
				height = 1;
			}
			if (depth < 1)
			{
				depth = 1;
			}
			numMipLevels++;
		}
		return numMipLevels;
	}

	// Turn off warning about FOURCC formats below...
#pragma warning (disable:4063)

#ifdef DX_TO_GL_ABSTRACTION
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)
#endif	

#pragma warning (default : 4063)

} // ImageLoader namespace ends
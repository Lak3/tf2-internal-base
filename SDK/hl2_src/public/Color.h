//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef COLOR_H
#define COLOR_H
#ifdef _WIN32
#pragma once
#endif

class FColor
{
public:
	FColor() {
		_color[0] = _color[1] = _color[2] = _color[3] = 0.0f;
	}

	FColor(const float _r, const float _g, const float _b) {
		SetColor(_r, _g, _b);
	}

	FColor(const float _r, const float _g, const float _b, const float _a) {
		SetColor(_r, _g, _b, _a);
	}

public:
	void SetColor(const float _r, const float _g, const float _b, const float _a = 0.0f) 
	{
		_color[0] = _r;
		_color[1] = _g;
		_color[2] = _b;
		_color[3] = _a;
	}

	void GetColor(float& _r, float& _g, float& _b, float& _a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	void SetRawColor(float color32)
	{
		*((float*)this) = color32;
	}

	float GetRawColor() const
	{
		return *((float*)this);
	}

	inline float r() const { return _color[0]; }
	inline float g() const { return _color[1]; }
	inline float b() const { return _color[2]; }
	inline float a() const { return _color[3]; }

public:
	float& operator[](int index)
	{
		return _color[index];
	}

	const float& operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const FColor& rhs) const
	{
		return (*((float*)this) == *((float*)&rhs));
	}

	bool operator != (const FColor& rhs) const
	{
		return !(operator==(rhs));
	}

	FColor& operator=(const FColor& rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

private:
	float _color[4];
};

//-----------------------------------------------------------------------------
// Purpose: Basic handler for an rgb set of colors
//			This class is fully inline
//-----------------------------------------------------------------------------
class Color
{
public:
	// constructors
	Color()
	{
		*((int*)this) = 0;
	}
	Color(int _r, int _g, int _b)
	{
		SetColor(_r, _g, _b, 0);
	}
	Color(int _r, int _g, int _b, int _a)
	{
		SetColor(_r, _g, _b, _a);
	}

	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void SetColor(int _r, int _g, int _b, int _a = 0)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetColor(int& _r, int& _g, int& _b, int& _a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	void SetRawColor(int color32)
	{
		*((int*)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int*)this);
	}

	inline int r() const { return _color[0]; }
	inline int g() const { return _color[1]; }
	inline int b() const { return _color[2]; }
	inline int a() const { return _color[3]; }

	void AsFloatAlpha(FColor& clrOut)
	{
		clrOut[0] = (static_cast<float>(_color[0]) / 255.0f);
		clrOut[1] = (static_cast<float>(_color[1]) / 255.0f);
		clrOut[2] = (static_cast<float>(_color[2]) / 255.0f);
		clrOut[3] = (static_cast<float>(_color[3]) / 255.0f);
	}

	void AsFloat(float* clrOut)
	{
		clrOut[0] = (static_cast<float>(_color[0]) / 255.0f);
		clrOut[1] = (static_cast<float>(_color[1]) / 255.0f);
		clrOut[2] = (static_cast<float>(_color[2]) / 255.0f);
	}

	unsigned char& operator[](int index)
	{
		return _color[index];
	}

	const unsigned char& operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color& rhs) const
	{
		return (*((int*)this) == *((int*)&rhs));
	}

	bool operator != (const Color& rhs) const
	{
		return !(operator==(rhs));
	}

	Color& operator=(const Color& rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

private:
	unsigned char _color[4];
};

#endif // COLOR_H
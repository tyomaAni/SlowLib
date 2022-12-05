/*
BSD 2-Clause License

Copyright (c) 2022, tyomaAni
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#ifndef _SL_LIB_COLOR_H_
#define _SL_LIB_COLOR_H_

#include <math.h>

// I don't know how to call it
#define SL_ColorDivider 0.00392156862745f

class slColorBytes
{
public:
	slColorBytes()
		:
		r(0),
		g(0),
		b(0),
		a(255)
	{}
	slColorBytes(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
		:
		r(R),
		g(G),
		b(B),
		a(A)
	{}

	slColorBytes(uint32_t uint_data) // 0xFF112233 ARGB
	{
		r = static_cast<uint8_t>(uint_data >> 16u);
		g = static_cast<uint8_t>(uint_data >> 8u);
		b = static_cast<uint8_t>(uint_data);
		a = static_cast<uint8_t>(uint_data >> 24u);
	}

	uint8_t r, g, b, a;

	bool	operator==(const slColorBytes& v) const
	{
		if (r != v.r) return false;
		if (g != v.g) return false;
		if (b != v.b) return false;
		if (a != v.a) return false;
		return true;
	}

	bool	operator!=(const slColorBytes& v) const
	{
		if (r != v.r) return true;
		if (g != v.g) return true;
		if (b != v.b) return true;
		if (a != v.a) return true;
		return false;
	}
};

class slColor
{
public:
	float	m_data[4];

	slColor()
	{
		m_data[0] = m_data[1] = m_data[2] = 0.f;
		m_data[3] = 1.f;
	}

	slColor(float v)
	{
		m_data[0] = m_data[1] = m_data[2] = v;
		m_data[3] = 1.f;
	}

	slColor(float r, float g, float b, float a = 1.f)
	{
		m_data[0] = r;
		m_data[1] = g;
		m_data[2] = b;
		m_data[3] = a;
	}

	slColor(int32_t r, int32_t g, int32_t b, int32_t a = 255)
	{
		this->setAsByteAlpha(a);
		this->setAsByteRed(r);
		this->setAsByteGreen(g);
		this->setAsByteBlue(b);
	}

	slColor(uint32_t uint_data) // 0xFF112233 ARGB
	{
		setAsInteger(uint_data);
	}

	float x() { return m_data[0]; }
	float y() { return m_data[1]; }
	float z() { return m_data[2]; }
	float w() { return m_data[3]; }

	const float* data() const { return &m_data[0u]; }

	const float getRed() const { return m_data[0u]; }
	const float getGreen() const { return m_data[1u]; }
	const float getBlue() const { return m_data[2u]; }
	const float getAlpha() const { return m_data[3u]; }

	const uint8_t getAsByteRed() const { return static_cast<uint8_t>(m_data[0u] * 255.); }
	const uint8_t getAsByteGreen() const { return static_cast<uint8_t>(m_data[1u] * 255.); }
	const uint8_t getAsByteBlue() const { return static_cast<uint8_t>(m_data[2u] * 255.); }
	const uint8_t getAsByteAlpha() const { return static_cast<uint8_t>(m_data[3u] * 255.); }

	bool	operator==(const slColor& v) const
	{
		if (m_data[0] != v.m_data[0]) return false;
		if (m_data[1] != v.m_data[1]) return false;
		if (m_data[2] != v.m_data[2]) return false;
		if (m_data[3] != v.m_data[3]) return false;
		return true;
	}

	bool	operator!=(const slColor& v) const
	{
		if (m_data[0] != v.m_data[0]) return true;
		if (m_data[1] != v.m_data[1]) return true;
		if (m_data[2] != v.m_data[2]) return true;
		if (m_data[3] != v.m_data[3]) return true;
		return false;
	}

	void normalize()
	{
		float len = sqrtf((m_data[0] * m_data[0]) + (m_data[1] * m_data[1]) + (m_data[2] * m_data[2]));
		if (len > 0)
		{
			len = 1.0f / len;
		}
		m_data[0] *= len;
		m_data[1] *= len;
		m_data[2] *= len;
	}

	// 0xff112233
	uint32_t getAsInteger()
	{
		return SL_MAKEFOURCC(
			this->getAsByteBlue(),
			this->getAsByteGreen(),
			this->getAsByteRed(),
			this->getAsByteAlpha()
		);
	}

	void setAlpha(float v) { m_data[3] = v; }
	void setRed(float v) { m_data[0] = v; }
	void setGreen(float v) { m_data[1] = v; }
	void setBlue(float v) { m_data[2] = v; }

	void setAsByteAlpha(int32_t v) { m_data[3] = static_cast<float>(v) * SL_ColorDivider; }
	void setAsByteRed(int32_t v) { m_data[0] = static_cast<float>(v) * SL_ColorDivider; }
	void setAsByteGreen(int32_t v) { m_data[1] = static_cast<float>(v) * SL_ColorDivider; }
	void setAsByteBlue(int32_t v) { m_data[2] = static_cast<float>(v) * SL_ColorDivider; }

	void setAsInteger(uint32_t v)
	{
		this->setAsByteRed(static_cast<uint8_t>(v >> 16));
		this->setAsByteGreen(static_cast<uint8_t>(v >> 8));
		this->setAsByteBlue(static_cast<uint8_t>(v));
		this->setAsByteAlpha(static_cast<uint8_t>(v >> 24));
	}

	void set(float r, float g, float b, float a = 1.)
	{
		setAlpha(a);
		setRed(r);
		setGreen(g);
		setBlue(b);
	}

	void set(float v, float a = 1.)
	{
		setAlpha(a);
		setRed(v);
		setGreen(v);
		setBlue(v);
	}

	void set(const slColor& other)
	{
		*this = other;
	}
};

//	HTML colors
const slColor ColorAliceBlue = 0xffF0F8FF;
const slColor ColorAntiqueWhite = 0xffFAEBD7;
const slColor ColorAqua = 0xff00FFFF;
const slColor ColorAquamarine = 0xff7FFFD4;
const slColor ColorAzure = 0xffF0FFFF;
const slColor ColorBeige = 0xffF5F5DC;
const slColor ColorBisque = 0xffFFE4C4;
const slColor ColorBlack = 0xff000000;
const slColor ColorBlanchedAlmond = 0xffFFEBCD;
const slColor ColorBlue = 0xff0000FF;
const slColor ColorBlueViolet = 0xff8A2BE2;
const slColor ColorBrown = 0xffA52A2A;
const slColor ColorBurlyWood = 0xffDEB887;
const slColor ColorCadetBlue = 0xff5F9EA0;
const slColor ColorChartreuse = 0xff7FFF00;
const slColor ColorChocolate = 0xffD2691E;
const slColor ColorCoral = 0xffFF7F50;
const slColor ColorCornflowerBlue = 0xff6495ED;
const slColor ColorCornsilk = 0xffFFF8DC;
const slColor ColorCrimson = 0xffDC143C;
const slColor ColorCyan = 0xff00FFFF;
const slColor ColorDarkBlue = 0xff00008B;
const slColor ColorDarkCyan = 0xff008B8B;
const slColor ColorDarkGoldenRod = 0xffB8860B;
const slColor ColorDarkGray = 0xffA9A9A9;
const slColor ColorDarkGrey = 0xffA9A9A9;
const slColor ColorDarkGreen = 0xff006400;
const slColor ColorDarkKhaki = 0xffBDB76B;
const slColor ColorDarkMagenta = 0xff8B008B;
const slColor ColorDarkOliveGreen = 0xff556B2F;
const slColor ColorDarkOrange = 0xffFF8C00;
const slColor ColorDarkOrchid = 0xff9932CC;
const slColor ColorDarkRed = 0xff8B0000;
const slColor ColorDarkSalmon = 0xffE9967A;
const slColor ColorDarkSeaGreen = 0xff8FBC8F;
const slColor ColorDarkSlateBlue = 0xff483D8B;
const slColor ColorDarkSlateGray = 0xff2F4F4F;
const slColor ColorDarkSlateGrey = 0xff2F4F4F;
const slColor ColorDarkTurquoise = 0xff00CED1;
const slColor ColorDarkViolet = 0xff9400D3;
const slColor ColorDeepPink = 0xffFF1493;
const slColor ColorDeepSkyBlue = 0xff00BFFF;
const slColor ColorDimGray = 0xff696969;
const slColor ColorDimGrey = 0xff696969;
const slColor ColorDodgerBlue = 0xff1E90FF;
const slColor ColorFireBrick = 0xffB22222;
const slColor ColorFloralWhite = 0xffFFFAF0;
const slColor ColorForestGreen = 0xff228B22;
const slColor ColorFuchsia = 0xffFF00FF;
const slColor ColorGainsboro = 0xffDCDCDC;
const slColor ColorGhostWhite = 0xffF8F8FF;
const slColor ColorGold = 0xffFFD700;
const slColor ColorGoldenRod = 0xffDAA520;
const slColor ColorGray = 0xff808080;
const slColor ColorGrey = 0xff808080;
const slColor ColorGreen = 0xff008000;
const slColor ColorGreenYellow = 0xffADFF2F;
const slColor ColorHoneyDew = 0xffF0FFF0;
const slColor ColorHotPink = 0xffFF69B4;
const slColor ColorIndianRed = 0xffCD5C5C;
const slColor ColorIndigo = 0xff4B0082;
const slColor ColorIvory = 0xffFFFFF0;
const slColor ColorKhaki = 0xffF0E68C;
const slColor ColorLavender = 0xffE6E6FA;
const slColor ColorLavenderBlush = 0xffFFF0F5;
const slColor ColorLawnGreen = 0xff7CFC00;
const slColor ColorLemonChiffon = 0xffFFFACD;
const slColor ColorLightBlue = 0xffADD8E6;
const slColor ColorLightCoral = 0xffF08080;
const slColor ColorLightCyan = 0xffE0FFFF;
const slColor ColorLightGoldenRodYellow = 0xffFAFAD2;
const slColor ColorLightGray = 0xffD3D3D3;
const slColor ColorLightGrey = 0xffD3D3D3;
const slColor ColorLightGreen = 0xff90EE90;
const slColor ColorLightPink = 0xffFFB6C1;
const slColor ColorLightSalmon = 0xffFFA07A;
const slColor ColorLightSeaGreen = 0xff20B2AA;
const slColor ColorLightSkyBlue = 0xff87CEFA;
const slColor ColorLightSlateGray = 0xff778899;
const slColor ColorLightSlateGrey = 0xff778899;
const slColor ColorLightSteelBlue = 0xffB0C4DE;
const slColor ColorLightYellow = 0xffFFFFE0;
const slColor ColorLime = 0xff00FF00;
const slColor ColorLimeGreen = 0xff32CD32;
const slColor ColorLinen = 0xffFAF0E6;
const slColor ColorMagenta = 0xffFF00FF;
const slColor ColorMaroon = 0xff800000;
const slColor ColorMediumAquaMarine = 0xff66CDAA;
const slColor ColorMediumBlue = 0xff0000CD;
const slColor ColorMediumOrchid = 0xffBA55D3;
const slColor ColorMediumPurple = 0xff9370DB;
const slColor ColorMediumSeaGreen = 0xff3CB371;
const slColor ColorMediumSlateBlue = 0xff7B68EE;
const slColor ColorMediumSpringGreen = 0xff00FA9A;
const slColor ColorMediumTurquoise = 0xff48D1CC;
const slColor ColorMediumVioletRed = 0xffC71585;
const slColor ColorMidnightBlue = 0xff191970;
const slColor ColorMintCream = 0xffF5FFFA;
const slColor ColorMistyRose = 0xffFFE4E1;
const slColor ColorMoccasin = 0xffFFE4B5;
const slColor ColorNavajoWhite = 0xffFFDEAD;
const slColor ColorNavy = 0xff000080;
const slColor ColorOldLace = 0xffFDF5E6;
const slColor ColorOlive = 0xff808000;
const slColor ColorOliveDrab = 0xff6B8E23;
const slColor ColorOrange = 0xffFFA500;
const slColor ColorOrangeRed = 0xffFF4500;
const slColor ColorOrchid = 0xffDA70D6;
const slColor ColorPaleGoldenRod = 0xffEEE8AA;
const slColor ColorPaleGreen = 0xff98FB98;
const slColor ColorPaleTurquoise = 0xffAFEEEE;
const slColor ColorPaleVioletRed = 0xffDB7093;
const slColor ColorPapayaWhip = 0xffFFEFD5;
const slColor ColorPeachPuff = 0xffFFDAB9;
const slColor ColorPeru = 0xffCD853F;
const slColor ColorPink = 0xffFFC0CB;
const slColor ColorPlum = 0xffDDA0DD;
const slColor ColorPowderBlue = 0xffB0E0E6;
const slColor ColorPurple = 0xff800080;
const slColor ColorRebeccaPurple = 0xff663399;
const slColor ColorRed = 0xffFF0000;
const slColor ColorRosyBrown = 0xffBC8F8F;
const slColor ColorRoyalBlue = 0xff4169E1;
const slColor ColorSaddleBrown = 0xff8B4513;
const slColor ColorSalmon = 0xffFA8072;
const slColor ColorSandyBrown = 0xffF4A460;
const slColor ColorSeaGreen = 0xff2E8B57;
const slColor ColorSeaShell = 0xffFFF5EE;
const slColor ColorSienna = 0xffA0522D;
const slColor ColorSilver = 0xffC0C0C0;
const slColor ColorSkyBlue = 0xff87CEEB;
const slColor ColorSlateBlue = 0xff6A5ACD;
const slColor ColorSlateGray = 0xff708090;
const slColor ColorSlateGrey = 0xff708090;
const slColor ColorSnow = 0xffFFFAFA;
const slColor ColorSpringGreen = 0xff00FF7F;
const slColor ColorSteelBlue = 0xff4682B4;
const slColor ColorTan = 0xffD2B48C;
const slColor ColorTeal = 0xff008080;
const slColor ColorThistle = 0xffD8BFD8;
const slColor ColorTomato = 0xffFF6347;
const slColor ColorTurquoise = 0xff40E0D0;
const slColor ColorViolet = 0xffEE82EE;
const slColor ColorWheat = 0xffF5DEB3;
const slColor ColorWhite = 0xffffffff;
const slColor ColorWhiteSmoke = 0xffF5F5F5;
const slColor ColorYellow = 0xffFFFF00;
const slColor ColorYellowGreen = 0xff9ACD32;

#endif 
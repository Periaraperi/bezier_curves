#pragma once

struct Color {
	unsigned short r,g,b,a;
	Color()
		:r{0},g{0},b{0},a{0}
	{}
	Color(unsigned short rr,unsigned short gg,unsigned short bb,unsigned short aa)
		:r{rr},g{gg},b{bb},a{aa}
	{}
};

struct Color_gl {
	float r,g,b,a;
	Color_gl()
		:r{0.0f},g{0.0f},b{0.0f},a{0.0f}
	{}
	Color_gl(float rr,float gg,float bb,float aa)
		:r{rr},g{gg},b{bb},a{aa}
	{}
};

inline Color_gl to_gl_color(Color cl)
{
	return Color_gl(cl.r/255.0f,cl.g/255.0f,cl.b/255.0f,cl.a/255.0f);
}

#ifndef _GFXCONTROLS_H
#define _GFXCONTROLS_H

#include <Adafruit_GFX.h>

enum GFXControl_Align_t
{
	GFXControl_Align_Left,
	GFXControl_Align_Center,
	GFXControl_Align_Right,
};

class GFXControl
{
public:
	virtual void refresh();
	virtual void show();
	virtual void hide();
	virtual void setVisible(bool visible);
	
	GFXControl *_owner;
	GFXControl *_next;
	GFXControl *_child;
	
	virtual void setForeground(uint16_t foreground)
	{
		if (foreground != _foreground)
		{
			_foreground = foreground;
			_drawned = false;
		}
	}

protected:
	GFXControl(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y,  uint16_t width, uint16_t height, GFXControl_Align_t align, uint16_t foreground);
	GFXControl(Adafruit_GFX *gfx);
	
	virtual void redraw() { }
	
	Adafruit_GFX *_gfx;
	const GFXfont *_font;
	uint8_t _size;
	int16_t _x;
	int16_t _y;
	int16_t _width;
	int16_t _height;
	int16_t _mx;
	int16_t _my;
	int16_t _mw;
	int16_t _mh;
	GFXControl_Align_t _align;
	uint16_t _foreground;
	uint16_t _background;

	bool _drawned;
	bool _visible;
};

class GFXDisplay: public GFXControl
{
public:
	GFXDisplay(Adafruit_GFX *gfx)
	: GFXControl(gfx) { }
protected:
	void redraw() { }
};

class GFXStaticIcon: public GFXControl
{
public:
	GFXStaticIcon(GFXControl *owner, int16_t x, int16_t y,  uint16_t width, uint16_t height, uint16_t foreground, const uint8_t *bitmap)
	:  GFXControl(owner, NULL, 0, x, y, width, height, GFXControl_Align_Left, foreground)
	{
		_bitmap = bitmap;
	}
protected:
	void redraw();
private:
	const uint8_t *_bitmap;
};

class GFXStaticBitmap: public GFXControl
{
public:
	GFXStaticBitmap(GFXControl *owner, int16_t x, int16_t y,  uint16_t width, uint16_t height, const uint16_t *bitmap)
	:  GFXControl(owner, NULL, 0, x, y, width, height, GFXControl_Align_Left, 0)
	{
		_bitmap = bitmap;
	}
protected:
	void redraw();
private:
	const uint16_t *_bitmap;
};

class GFXStaticBox: public GFXControl
{
public:
	GFXStaticBox(GFXControl *owner, uint8_t size, int16_t x, int16_t y,  uint16_t width, uint16_t height, uint16_t foreground, uint16_t background)
	:  GFXControl(owner, NULL, size, x, y, width, height, GFXControl_Align_Left, foreground)
	{
		_background = background;
	}
protected:	
	void redraw();
};

class GFXLabel: public GFXControl
{
public:
	GFXLabel(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y, GFXControl_Align_t align, uint16_t foreground, const char *prefix, const char *suffix)
	:  GFXControl(owner, font, size, x, y, 0, 0, align, foreground)
	{
		_prefix = prefix;
		_suffix = suffix;
	}
protected:	
	void redraw();
	void print(const String& val, bool clear = false);
	
	const char *_prefix;
	const char *_suffix;
	int16_t _x_offset;
	int16_t _y_offset;
};

class GFXStaticLabel: public GFXLabel
{
public:
	GFXStaticLabel(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y,  GFXControl_Align_t align, uint16_t foreground, const char *s)
	:  GFXLabel(owner, font, size, x, y, align, foreground, NULL, NULL)
	{
		_s = s;
	}
protected:
	void redraw();
private:
	const char *_s;
};

class GFXBoundLabel: public GFXLabel
{
public:
	GFXBoundLabel(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y,  GFXControl_Align_t align, uint16_t foreground, String *s, const char *prefix = NULL, const char *suffix = NULL)
	:  GFXLabel(owner, font, size, x, y, align, foreground, prefix, suffix)
	{
		_s = s;
	}
protected:	
	void redraw();
private:
	String *_s;
	String _last;
};

class GFXBoundNumeric: public GFXLabel
{
public:
	GFXBoundNumeric(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y,  GFXControl_Align_t align, uint16_t foreground, int *val, const char *prefix = NULL, const char *suffix = NULL)
	:  GFXLabel(owner, font, size, x, y, align, foreground, prefix, suffix)
	{
		_val = val;
	}
protected:	
	void redraw();
private:
	int *_val;
	int _last;
};

class GFXBoundFloat: public GFXLabel
{
public:
	GFXBoundFloat(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y,  GFXControl_Align_t align, uint16_t foreground, float* val, int decimals, const char *prefix = NULL, const char *suffix = NULL)
	:  GFXLabel(owner, font, size, x, y, align, foreground, prefix, suffix)
	{
		_val = val;
		_decimals = decimals;
	}
protected:	
	void redraw();
private:
	int _decimals;
	float *_val;
	float _last;
};

class GFXBoundChar: public GFXLabel
{
public:
	GFXBoundChar(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y,  GFXControl_Align_t align, uint16_t foreground, char* val, const char *prefix = NULL, const char *suffix = NULL)
	:  GFXLabel(owner, font, size, x, y, align, foreground, prefix, suffix)
	{
		_val = val;
	}
protected:	
	void redraw();
private:
	char *_val;
	char _last;
};

class GFXBoundHBar: public GFXControl
{
public:
	GFXBoundHBar(GFXControl *owner, uint8_t size, int16_t x, int16_t y, uint16_t width, uint16_t height, GFXControl_Align_t align, uint16_t foreground, int *val, int min, int max)
	:  GFXControl(owner, NULL, size, x, y, width, height, align, foreground)
	{
		_val = val;
		_min = min;
		_max = max;
		_range = _max - _min;
		_barx = _x + _size;
		_bary = _y + _size;
		_barWidth = _width - (2 * _size);
		_barHeight = _height - (2 * _size);
	}
protected:  
	void redraw();
private:
	int _min;
	int _max;
	int _range;
	int _barx;
	int _bary;
	int _barWidth;
	int _barHeight;
	int *_val;
	int _lastVal;
	int _lastPos;	
};



#endif // _GFXCONTROLS_H

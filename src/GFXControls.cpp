#include "GFXControls.h"

void GFXControl::refresh()
{
	redraw();	
	
	if (_child != nullptr)
	{
		GFXControl *p = _child;
		do
		{
			p->refresh();
		}
		while ((p = p->_next) != nullptr);
	}
}

void GFXControl::show()
{
	//Serial.print(_name); Serial.println(".visible = true");
	
	_visible = true;

	if (_child != nullptr)
	{
		GFXControl *p = _child;
		do
		{
			p->show();
		}
		while ((p = p->_next) != nullptr);
	}
}

void GFXControl::hide()
{
	//Serial.print(_name); Serial.println(".visible = false");
	
	_drawned = false;
	_visible = false;
	
	if (_child != nullptr)
	{
		GFXControl *p = _child;
		do
		{
			p->hide();
		}
		while ((p = p->_next) != nullptr);
	}
}

void GFXControl::setVisible(bool visible)
{
	if (visible) show();
	else hide();
}


GFXControl::GFXControl(GFXControl *owner, const GFXfont *font, uint8_t size, int16_t x, int16_t y,  uint16_t width, uint16_t height, GFXControl_Align_t align, uint16_t foreground)
{	
	_owner = owner;
	_next = nullptr;
	_child = nullptr;
	_gfx = owner->_gfx;
	_font = font;
	_size = size;
	_x = x + owner->_x;
	_y = y + owner->_y;
	_width = width;
	_height = height;
	_align = align;
	_foreground = foreground;
	_background = owner->_background;
	
	_mx = _x;
	_my = _y;
	_mw = _width;
	_mh = _height;

	_drawned = false;
	_visible = true;	

	//Serial.print("Creation de "); Serial.println(_name);

	if (_owner != nullptr)
	{
		if (_owner->_child == nullptr)
		{
			_owner->_child = this;
		}
		else
		{
			GFXControl *last = _owner->_child;
			while (last->_next != nullptr)
			{
				last = last->_next;
			}
			last->_next = this;
		}
	}
}

GFXControl::GFXControl(Adafruit_GFX *gfx)
{
	_gfx = gfx;
	
	_drawned = false;
	_visible = true;

	_owner = nullptr;
	_next = nullptr;
	_child = nullptr;
}

void GFXStaticIcon::redraw()
{
	if (_bitmap == nullptr || !_visible || _drawned) return;
	
	_gfx->drawBitmap(_x, _y, _bitmap, _width, _height, _foreground);
	
	_drawned = true;
}

void GFXStaticBitmap::redraw()
{
	if (_bitmap == nullptr || !_visible || _drawned) return;
	
	_gfx->drawRGBBitmap(_x, _y, _bitmap, _width, _height);
	
	_drawned = true;
}

void GFXLabel::print(const String& val, bool clear)
{
	uint16_t w, h;
	int16_t x, y, align_offset;
	
	String s;
	if (_prefix != nullptr) s.concat(_prefix);
	s.concat(val);
	if (_suffix != nullptr) s.concat(_suffix);
	
	if (s.length() == 0)
	{
		if (clear)
		{
			_gfx->fillRect(_mx, _my, _mw, _mh, _background);
		}
		return;
	}
	
	_gfx->setFont(_font);
	_gfx->setTextSize(_size);
	_gfx->setTextColor(_foreground);
	
	_gfx->getTextBounds(s, 0, 0, &x, &y, &w, &h);

	if (_font != nullptr)
	{
		switch (_align)
		{
		case GFXControl_Align_Center:
			_x_offset = 3;
			break;
		case GFXControl_Align_Right:
			_x_offset = 6;
			break;
		default:
			_x_offset = 0;
			break;
		}
		_y_offset = h;
	}
	else
	{
		_x_offset = 0;
		_y_offset = 0;
	}	
	
	switch (_align)
	{
		case GFXControl_Align_Center:
			align_offset = (w / 2) + _x_offset;
			break;
		case GFXControl_Align_Right:
			align_offset = w + _x_offset;
			break;
		default:
			align_offset = _x_offset;
			break;
	}
	
	_gfx->setCursor(_x - align_offset, _y + _y_offset);

	_gfx->fillRect(_mx, _my, _mw, _mh, _background);
	
	_gfx->print(s);
	
	_mx = x + _x - align_offset;
	_my = y + _y + _y_offset;
	_mw = w;
	_mh = h;
}

void GFXStaticBox::redraw()
{
	if (!_visible || _drawned) return;
	
	for (int i = 0; i < _size; ++i)
	{
		_gfx->drawRect(_x + i, _y + i, _width - (2 * i), _height - (2 * i), _foreground);
	}
	_gfx->fillRect(_x + _size, _y + _size, _width - (2 * _size), _height - (2 * _size), _background);
	
	_drawned = true;
}

void GFXStaticLabel::redraw()
{
	if (!_visible || _drawned) return;
	
	print(String(_s));
	
	_drawned = true;
}

void GFXBoundLabel::redraw()
{
	if (!_visible || _s == nullptr) return;
	
	String s(*_s);
	
	if (_drawned && s.compareTo(_last) == 0) return;
	
	print(s, _drawned);
	
	_last = s;
	_drawned = true;
}

void GFXBoundNumeric::redraw()
{
	if (!_visible || _val == nullptr) return;
	
	int val = *_val;
		
	if (_drawned && val == _last) return;

	String s(val);
	print(s, _drawned);

	_last = val;
	_drawned = true;
}

void GFXBoundFloat::redraw()
{
	if (!_visible || _val == nullptr ) return;
	
	float val = *_val;
	
	if (_drawned && val == _last) return;

	print(String(val, _decimals), _drawned);

	_last = val;
	_drawned = true;
}

void GFXBoundChar::redraw()
{
	if (!_visible || _val == nullptr ) return;
	
	char val = *_val;
	
	if (_drawned && val == _last) return;

	print(String(val), _drawned);

	_last = val;
	_drawned = true;
}

void GFXBoundHBar::redraw()
{
	if (!_visible || _val == nullptr || _range == 0) return;
	
	int val = *_val;

	if (val > _max) val = _max;
	if (val < _min) val = _min;	
	
	if (_drawned && val == _lastVal) return;
	
	int16_t pos = (_barWidth * (val - _min)) / _range;
	
	if (!_drawned)
	{
		for (int i = 0; i < _size; ++i)
		{
			_gfx->drawRect(_x + i, _y + i, _width - (2 * i), _height - (2 * i), _foreground);
		}
		
		_lastPos = _align == GFXControl_Align_Right ? _barWidth : 0;
	}
	
	if (_align == GFXControl_Align_Right)
	{
		pos = _barWidth - pos;
		if (pos > _lastPos)
		{
			_gfx->fillRect(_barx + _lastPos, _bary, pos - _lastPos, _barHeight, _background);	
		}
		else
		{
			_gfx->fillRect(_barx + pos, _bary, _lastPos - pos, _barHeight, _foreground);
		}
	}
	else
	{
		if (pos > _lastPos)
		{
			_gfx->fillRect(_barx + _lastPos, _bary, pos - _lastPos, _barHeight, _foreground);	
		}
		else
		{
			_gfx->fillRect(_barx + pos, _bary, _lastPos - pos, _barHeight, _background);
		}
	}
	
	_lastPos = pos;
	_lastVal = val;
	_drawned = true;
}



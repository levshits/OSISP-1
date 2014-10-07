#pragma once
#include "Instrument.h"
class TextDrawing :
	public Instrument
{
private:
	TextDrawing(){};
public:
	static TCHAR text[100];
	static TextDrawing* GetInstance()
	{
		static TextDrawing _self;
		return &_self;
	}
	
	virtual void Draw(int x, int y);
	virtual void Display(int x, int y);
};


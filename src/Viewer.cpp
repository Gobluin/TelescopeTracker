/*
 * Viewer.cpp
 *
 *  Created on: 25 авг. 2015 г.
 *      Author: shrike
 */

#include "Viewer.h"
#include "f10x-pcd8544.h"
#include <cstdio>

Viewer::Viewer()
{


}

Viewer::~Viewer()
{

}




void Viewer::Init()
{
	lcd_init();
	lcd_putstr(0,0,(unsigned char*)"   X:",0);
	lcd_putstr(0,8,(unsigned char*)"   Y:",0);
	lcd_putstr(0,16,(unsigned char*)"   Z:",0);
	lcd_putstr(0,24,(unsigned char*)"TEMP:",0);
	lcd_refresh();
}

void Viewer::SetX(float& x)
{
	sprintf( floatBuf , "%3.10f\n", x);
	lcd_putstr(20,0, (unsigned char*)floatBuf,0);
	lcd_refresh();
}

void Viewer::SetY(float& y)
{
	snprintf( floatBuf,11 , "%3.10f", y);
	lcd_putstr(20,8, (unsigned char*)floatBuf,0);
	lcd_refresh();
}

void Viewer::SetZ(float& z)
{
	snprintf( floatBuf ,11, "%3.10f", z);
	lcd_putstr(20,16, (unsigned char*)floatBuf,0);
	lcd_refresh();
}

void Viewer::SetTemp(float& temp)
{
	snprintf(floatBuf,11 , "%3.10f", temp);
	lcd_putstr(24,24, (unsigned char*)floatBuf,0);
	lcd_refresh();
}

void Viewer::SetData(float x, float y ,float z ,float temp)
{
	snprintf( floatBuf,11 , "%3.6f", x);
	lcd_putstr(26,0,(unsigned char*) floatBuf,0);
	snprintf( floatBuf,11 , "%3.6f", y);
	lcd_putstr(26,8, (unsigned char*)floatBuf,0);
	snprintf( floatBuf,11 , "%3.6f", z);
	lcd_putstr(26,16, (unsigned char*)floatBuf,0);
	snprintf( floatBuf,11 , "%3.4f", temp);
	lcd_putstr(26,24, (unsigned char*)floatBuf,0);
	lcd_refresh();
}

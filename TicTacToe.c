/*
 * TicTacToe.c
 *
 *  Created on: 2017. dec. 5.
 *      Author: Peti
 */

#include <stm32f4xx.h>

#include "display.h"
#include "vcp.h"
#include "Common.h"
#include "drawing.h"
#include "touch.h"
#include "gyroscope.h"

#include "UartFifo.h"
#include "MessageHandler.h"
#include "CommHandler.h"

#include "WindowManager.h"
#include "MenuWindow.h"
#include "TicTacToe.h"
#include <stdio.h>
#include <math.h>

typedef struct {
	uint16_t x;
	uint16_t y;
} Vector;

/*Pálya kirajzolása*/
void DrawField()
{
	uint16_t i;
	Vector coord;
	Pixel p;
	p.x=0;
	p.y=BSP_LCD_GetYSize()/3;

	coord.x = BSP_LCD_GetXSize();
	coord.y = BSP_LCD_GetYSize();

	/*Függõleges és vízszintes vonalak*/
	for (i = 0; i < 2; i++)
	{

		DRV_DrawHorizontalLine(p, coord.x, blue);
		p.y=p.y+BSP_LCD_GetYSize()/3;

	}

	p.x=BSP_LCD_GetXSize()/3;
	p.y=0;
	for (i = 0; i < 2; i++)
	{
		DRV_DrawVerticalLine(p, coord.y, blue);
		p.x=p.x+BSP_LCD_GetXSize()/3;
	}
}
/*Visszatérési érték egy cella közepének a pozíciója, a pos adja meg a cella számát (0-8)*/
Pixel SymbolPos(int *pos)
{
	Pixel t;
	t.x = DRV_TS_GetX();
	t.y = DRV_TS_GetY();
	Pixel p;
	p.x = 0;
	p.y  =0;

	/*Hova érintettünk*/
	if ( t.x <= (BSP_LCD_GetXSize() / 3) && t.y <= (BSP_LCD_GetYSize()/3))
	{
		*pos = 0;
		p.x = BSP_LCD_GetXSize() / 6;
		p.y = BSP_LCD_GetYSize() / 6;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize() * 2 / 3) && t.x >= (BSP_LCD_GetXSize() / 3) && t.y <= (BSP_LCD_GetYSize()/3))
	{
		*pos = 1;
		p.x = BSP_LCD_GetXSize() / 2;
		p.y = BSP_LCD_GetYSize() / 6;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize()) && t.x >= (BSP_LCD_GetXSize() * 2/ 3) && t.y <= (BSP_LCD_GetYSize()/3))
	{
		*pos = 2;
		p.x = BSP_LCD_GetXSize() * 5 / 6;
		p.y = BSP_LCD_GetYSize() / 6;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize() / 3) && t.y <= (BSP_LCD_GetYSize()* 2 /3) && t.y >= (BSP_LCD_GetYSize()/3))
	{
		*pos = 3;
		p.x = BSP_LCD_GetXSize() / 6;
		p.y = BSP_LCD_GetYSize() / 2;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize() * 2 / 3) && t.x >= (BSP_LCD_GetXSize() / 3) && t.y <= (BSP_LCD_GetYSize()* 2 /3) && t.y >= (BSP_LCD_GetYSize()/3))
	{
		*pos = 4;
		p.x = BSP_LCD_GetXSize() / 2;
		p.y = BSP_LCD_GetYSize() / 2;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize()) && t.x >= (BSP_LCD_GetXSize() * 2/ 3) && t.y <= (BSP_LCD_GetYSize()* 2 / 3)  && t.y >= (BSP_LCD_GetYSize()/ 3))
	{
		*pos = 5;
		p.x = BSP_LCD_GetXSize() * 5 / 6;
		p.y = BSP_LCD_GetYSize() / 2;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize() / 3) && t.y <= (BSP_LCD_GetYSize()) && t.y >= (BSP_LCD_GetYSize()*2/3))
	{
		*pos = 6;
		p.x = BSP_LCD_GetXSize() / 6;
		p.y = BSP_LCD_GetYSize() * 5 / 6;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize() * 2 / 3) && t.x >= (BSP_LCD_GetXSize() / 3) && t.y <= (BSP_LCD_GetYSize()) && t.y >= (BSP_LCD_GetYSize()*2/3))
	{
		*pos = 7;
		p.x = BSP_LCD_GetXSize() / 2;
		p.y = BSP_LCD_GetYSize() * 5 / 6;
		return p;
	}

	if ( t.x <= (BSP_LCD_GetXSize()) && t.x >= (BSP_LCD_GetXSize() * 2/ 3) && t.y <= (BSP_LCD_GetYSize())  && t.y >= (BSP_LCD_GetYSize()* 2 / 3))
	{
		*pos = 8;
		p.x = BSP_LCD_GetXSize() * 5 / 6;
		p.y = BSP_LCD_GetYSize() * 5 / 6;
		return p;
	}
	return p;
}

void DrawSymbol(Pixel p, char* TicTac)
{
	DRV_Display_WriteStringAt(p, TicTac, ALIGN_Center);
}



int GameResult(int *p)
{
	int i,j,k,l;

	j = 0;
	k = 3;
	l = 6;

	for(i = 0; i < 3; i++) {
		if(p[j] ==  2 && p[k] == 2 && p[l] == 2) return 1;
		if(p[j] == 1 && p[k] == 1 && p[l] == 1) return 2;
		j++;
		k++;
		l++;
	}

	j = 0;
	k = 1;
	l = 2;

	for(i = 0; i < 3; i++) {
		if(p[j] == 2 && p[k] == 2 && p[l] == 2) return 1;
		if(p[j] == 1 && p[k] == 1 && p[l] == 1) return 2;
		j += 3;
		k += 3;
		l += 3;
	}

	if(p[0] == 2 && p[4] == 2 && p[8] == 2) return 1;
	if(p[0] == 1 && p[4] == 1 && p[8] == 1) return 2;

	if(p[2] == 2 && p[4] == 2 && p[6] == 2) return 1;
	if(p[2] == 1 && p[4] == 1 && p[6] == 1) return 2;

	return 0;
}

int Draw(int *TicTacs,int TicTac )
{	int i,j,k,l;

	j = 0;
	k = 3;
	l = 6;

	for(i = 0; i < 3; i++) {
		if((TicTacs[j] + TicTacs[k] + TicTacs[l]) == 4  && TicTac== 1) return 0;
		if((TicTacs[j] + TicTacs[k] + TicTacs[l]) == 2 && TicTac == 2) return 0;
		j++;
		k++;
		l++;
	}

	j = 0;
	k = 1;
	l = 2;

	for(i = 0; i < 3; i++) {
		if((TicTacs[j] + TicTacs[k] + TicTacs[l]) == 4 && TicTac == 1) return 0;
		if((TicTacs[j] + TicTacs[k] + TicTacs[l]) == 2 && TicTac == 2) return 0;
		j += 3;
		k += 3;
		l += 3;
	}

	if((TicTacs[0] + TicTacs[4] + TicTacs[8] ) == 4 && TicTac ==1) return 0;
	if((TicTacs[0] + TicTacs[4] + TicTacs[8]) == 2 && TicTac == 2) return 0;

	if((TicTacs[2] + TicTacs[4] + TicTacs[6]) == 4 && TicTac == 1) return 0;
	if((TicTacs[2] + TicTacs[4] + TicTacs[6]) == 2 && TicTac == 2) return 0;

	return -1;
	}
void GameOver(int winner){
	Pixel Center;
	Center.x=BSP_LCD_GetXSize() / 2;
	Center.y=BSP_LCD_GetYSize() / 2;
	if(winner == 1) //Játék vége
	{
		DRV_Display_WriteStringAt(Center, "The Winner is: X", ALIGN_Center);
	}
	else if(winner == 2)
	{
		DRV_Display_WriteStringAt(Center, "The Winner is: O", ALIGN_Center);
	}
	else
	{
		DRV_Display_WriteStringAt(Center, "Tic-Tac-Tie!", ALIGN_Center);
	}
}

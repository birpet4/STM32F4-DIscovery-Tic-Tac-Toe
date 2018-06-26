/*
 * TicTacToe.h
 *
 *  Created on: 2017. dec. 5.
 *      Author: Peti
 */

#ifndef TICTACTOE_H_
#define TICTACTOE_H_

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

#include <stdio.h>
#include <math.h>

void DrawField();

Pixel SymbolPos(int *pos);

void DrawSymbol(Pixel p, char* Marker);

int GameResult(int *p);

int IsEmpty(char* tictacs, int pos);

int Draw(int *TicTacs,int TicTac);

#endif /* TICTACTOE_H_ */

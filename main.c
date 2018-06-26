/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
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

static void SystemClock_Config(void);
char buff[20];
uint32_t frameTime;

int main(void)
{
	/*Rendszer inicializálása*/
	SystemClock_Config();
	HAL_Init();
	DRV_Display_Init();
	DRV_Display_Clear();
	DRV_TS_Init();

	/*Változók inicializálása*/
	int Pos = 1;
	int GameWinner = 0;
	int Space = 0;
	int TicTac = 1;
	int DrawResult=1;

	/*A játék*/
	while(1)
	{
		int TicTacs[] = {0,0,0,0,0,0,0,0,0};
		int empty[] = {0,0,0,0,0,0,0,0,0};
		DRV_Display_Clear();
		DrawField();
		DRV_Display_SwitchBuffer();

		while(Space < 9 && GameWinner == 0 && DrawResult == 1)	 //Amíg nem teltek be a helyek, vagy nem nyert valki
		{
			Pixel t;
			if(DRV_TS_IsTouchDetected()) //Érintés
			{
				t = SymbolPos(&Pos); //Pozíció és pixel
				if(!empty[Pos])
				{
					DRV_Display_SwitchBuffer();
					if(TicTac==1) //Ha a szimbólum X, akkor X-et, ha O, akkor O-t ír a cella közepébe
					{
							DRV_DrawCircle(t,25, green);
							TicTacs[Pos] = 1;

					}
					else
					{		Pixel p;
							p.x=t.x+20;
							p.y=t.y+20;
							t.x -= 20;
							t.y -= 20;

							DRV_DrawLine(t,p,red);
							t.x += 40;
							p.x=t.x-40;
							p.y=t.y+40;
							DRV_DrawLine(t,p,red);

							TicTacs[Pos] = 2;
					}
					//TicTacs[Pos] = TicTac;
					if(TicTac == 1) //Szimbólum váltás
					{
						TicTac = 2;
					}
					else
					{
						TicTac = 1;
					}
					DRV_Display_SwitchBuffer();

					if(Space == 7)
						DrawResult=Draw(TicTacs,TicTac);

					Space++;


					empty[Pos] = 1;
					HAL_Delay(500);
				}

			}
			GameWinner = GameResult(TicTacs); //X = 1, O = 2, döntetlen = 0
		}

		DRV_Display_Clear();
		GameOver(GameWinner);


		/* Új játék */
		DRV_Display_SwitchBuffer();
		Pos = 1;
		Space = 0;
		GameWinner = 0;
		TicTac = 1;
		DrawResult=1;
		HAL_Delay(3000);
	}


}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (HandleDraw())
		DRV_Display_SwitchBuffer();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	DRV_ErrorLoop();

}
#endif

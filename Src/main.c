/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/*rules
 * eliminate all enemy then go to next roundit,and roundit++
 * rank=(roundit-3)>=1?(roundit-3):1;speed=rank;enemynum=roundit<=5?(roundit+2):7;
 * enemy hit:red blink,score+1
 * you hit:green blink,roundit=1,life-1,restart
 * for each restart:
 * 		if life=0 then go to next stage and print score
 * 		else ...
 * 	for each roundit:
 * 		calculate rank,speed,enemynum
 * 		prepare enemy*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include <stdlib.h>
#include <stdio.h>
#include<time.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void startpage(void);
void endpage(void);
void play(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void init_eachround(void);
void paintall(void);
void paintscore(int x,int y,int size);
void updatebullets();
void movebullets();
void moveplanes();
void updateenemy();
void hitit();
void updateenemybullets();
void moveenemybullets();
void paintenemybullet(int x, int y);
void dieit();
int kz=0;
int score=0;
int cond=0;
int life=0;
int roundit=0;
int selectcolor=0;
int player[2];
int bullet[10][2];
int enemy[10][2];
int enemynum=0;
int speed=0;
int enemybullet[10][2];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  LCD_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
startpage();
play();
endpage();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//arm-none-eabi-objcopy "${ProjName}.elf" -O ihex "${ProjName}.hex"
//here key0 is gg,it should be right
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
HAL_Delay(100);
switch (GPIO_Pin) {
case KEY0_Pin:
if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET) {

if(cond==1){
	if(player[0]-10 > 5){
		player[0] = player[0]-5;
		paintall();
	}
}
}
break;
//left
case KEY1_Pin:
if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) {
	if (cond==0){cond=1;}
	else if(cond==1){
		if(player[0]+10 <220){
			player[0] = player[0]+5;
			paintall();
		}
	}
	else if(cond==2){cond=0;}
}
break;
//fire!
case KEYUP_Pin:
if (HAL_GPIO_ReadPin(KEYUP_GPIO_Port, KEYUP_Pin) == GPIO_PIN_SET) {

	if(cond==1){
		for(int i=0; i<10; i++){
			if(bullet[i][0] == 0){
					bullet[i][0] = player[0];bullet[i][1] = player[1]-22;
					paintall();
					break;
				}
		}
	}
}
break;
default:
break;
}}

//set enemy and plane
void init_eachround(void){
	player[0]=120;player[1]=280;
	if(roundit<=3)enemynum=1;else if(roundit>3&&roundit<=5)enemynum=roundit;else enemynum=6;
	if(roundit<=3)speed=1;else if(roundit>3&&roundit<=6)speed=roundit-2;else speed=4;
	//clear planes
	for(int i=0;i<10;i++){enemybullet[i][0]=0;enemybullet[i][1]=0;}
	for(int i=0;i<10;i++){enemy[i][0]=0;enemy[i][1]=0;}
	for(int i=0;i<10;i++){bullet[i][0]=0;bullet[i][1]=0;}

	LCD_Fill(1,41,239,318,BACK_COLOR);
	//clear bullets
	int startposx1=30;int startposy1=70;
	int startposx2=30;int startposy2=120;
	int rop1=0;int rop2=0;
	if (enemynum<=5){rop1=enemynum;}else if(enemynum>5&&enemynum<=10){rop2=enemynum-5;rop1=5;}
	int gg=rand()%16-8;
	for(int i=0; i<rop1; i++){
			enemy[i][0]=startposx1;enemybullet[i][0]=startposx1+gg;
			enemy[i][1]=startposy1;enemybullet[i][1]=startposy1;
			startposx1+=40;
		}
	if(rop2>0){
		for(int i=0; i<rop2; i++){
					enemy[i+5][0]=startposx2;
					enemy[i+5][1]=startposy2;
					startposx2+=40;
				}
	}
	paintall();
}
//if you don't die, then next roundit
void play(void){
	score=0;
	roundit=1;
	life=3;
	kz=0;
	BACK_COLOR=WHITE;
	LCD_Clear(BACK_COLOR);
		while(1){
			if(kz==0)BACK_COLOR=WHITE;
			if(kz==3)BACK_COLOR=GREEN;
			else if (kz==6)BACK_COLOR=RED;
			else if (kz==9)BACK_COLOR=BLUE;
			LCD_Clear(BACK_COLOR);
			LCD_ShowString(5,8,90,16,16,(uint8_t*)"score");
			LCD_ShowString(135,8,90,16,16,(uint8_t*)"life");
			init_eachround();
			while(1){
				updatebullets();
				updateenemy();
				updateenemybullets();
				dieit();
				hitit();
				paintall();
				if(life<=0){
					cond=2;break;
				}
				if(enemy[0][0]==0){break;}

			}
			kz++;
			kz%=12;
			roundit+=1;
			if(cond!=1)break;
			HAL_Delay(10);
	}
}
void endpage(void){
	LCD_Clear(BACK_COLOR);
	POINT_COLOR = BLACK;
	LCD_ShowString(30, 40, 200, 24, 24, (uint8_t*) "GAMEOVER");
	LCD_ShowString(30, 70, 200, 16, 16, (uint8_t*) "Your score is");
	paintscore(150, 70, 16);

	LCD_ShowString(30, 100, 200, 16, 16, (uint8_t*) "press KEY1 to back");
	while(1){
		if(cond!=2)break;
		}
}
void startpage(void){
	LCD_Clear(BACK_COLOR);
		POINT_COLOR = BLACK;
		LCD_ShowString(30, 40, 200, 24, 24, (uint8_t*) "STM32 SHUMP");
		LCD_ShowString(30, 100, 200, 16, 16, (uint8_t*) "press KEY1 to start");
		while(1){
			if(cond!=0)break;
			}

}
//bullet is 3x3
void updatebullets(){
	for(int i=0; i<10; i++){
		if(bullet[i][0] != 0){
			LCD_Fill(bullet[i][0]-1,bullet[i][1]-1,bullet[i][0]+1,bullet[i][1]+1,BACK_COLOR);
			bullet[i][1] = bullet[i][1] - speed;
			if(bullet[i][1] < 42){bullet[i][0] = 0;bullet[i][1] = 0;movebullets();}
		}
	}

}
void updateenemybullets(){
	for(int i=0; i<10; i++){
		if(enemybullet[i][0] != 0){
			LCD_Fill(enemybullet[i][0]-4,enemybullet[i][1]-4,enemybullet[i][0]+4,enemybullet[i][1]+4,BACK_COLOR);
			enemybullet[i][1] = enemybullet[i][1] + speed*2;
			if(bullet[i][1] > 290){enemybullet[i][0] = 0;enemybullet[i][1] = 0;moveenemybullets();}
		}
	}

}

void updateenemy(){
	for(int i=0; i<10; i++){
		if(enemy[i][0] != 0){
			LCD_Fill(enemy[i][0]-10,enemy[i][1]-20,enemy[i][0]+10,enemy[i][1]+20,BACK_COLOR);
			enemy[i][1] = enemy[i][1] + speed;
			if(enemy[i][1] > 298){enemy[i][0] = 0;enemy[i][1] = 0;moveplanes();}
		}
	}
}

void dieit(){
	for(int i=0; i<10; i++){
		if(enemy[i][0]!=0&&abs(enemy[i][0] - player[0]) <= 20&&abs(enemy[i][1] - player[1]) <= 40){
					LCD_Fill(player[0]-10,player[1]-20,	player[0]+10,player[1]+20,BACK_COLOR);
					LCD_Fill(enemy[i][0]-10,enemy[i][1]-20,enemy[i][0]+10,enemy[i][1]+20,BACK_COLOR);
					player[0] = 120;player[1] = 280;
					enemy[i][0] = 0;enemy[i][1] = 0;
					life-=1;
					HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
					HAL_Delay(50);
					HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
					HAL_Delay(50);
					HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
					HAL_Delay(50);
					HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
					moveplanes();
					break;
		}
	}
	for(int i=0; i<10; i++){
			if(enemybullet[i][0]!=0&&abs(enemybullet[i][0] - player[0]) <= 10&&abs(enemybullet[i][1] - player[1]) <= 20){
						LCD_Fill(player[0]-10,player[1]-20,	player[0]+10,player[1]+20,BACK_COLOR);
						LCD_Fill(enemybullet[i][0]-4,enemybullet[i][1]-4,enemybullet[i][0]+4,enemybullet[i][1]+4,BACK_COLOR);
						player[0] = 120;player[1] = 280;
						enemybullet[i][0] = 0;enemybullet[i][1] = 0;
						life-=1;
						HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
						HAL_Delay(50);
						HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
						HAL_Delay(50);
						HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
						HAL_Delay(50);
						HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
						moveenemybullets();
						break;
			}
		}
}

void hitit(){
	int hitit=0;
	for(int i=0; i<10; i++){
		if (hitit==1)break;
		if(enemy[i][0] != 0){
			for(int j=0; j<10; j++){
				if(abs(bullet[j][0] - enemy[i][0]) <= 12&&abs(bullet[j][1] - enemy[i][1]) <= 22){
						LCD_Fill(enemy[i][0]-10,enemy[i][1]-20,enemy[i][0]+10,enemy[i][1]+20,BACK_COLOR);
						bullet[j][0] = 0;bullet[j][1] = 0;
						enemy[i][0] = 0;enemy[i][1] = 0;
						score+=1;
						HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
						HAL_Delay(50);
						HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
						HAL_Delay(50);
						HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
						HAL_Delay(50);
						HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
						moveplanes();
						movebullets();
						hitit=1;
						break;
				}
			}
		}
	}

}
void moveenemybullets(){
	if(enemybullet[0][0] == 0){
		for(int i=0; i<9; i++){
			enemybullet[i][0] = enemybullet[i+1][0];
			enemybullet[i][1] = enemybullet[i+1][1];
		}
	}
}


void movebullets(){
	if(bullet[0][0] == 0){
		for(int i=0; i<9; i++){
			bullet[i][0] = bullet[i+1][0];
			bullet[i][1] = bullet[i+1][1];
		}
	}
}

void moveplanes(){
	if(enemy[0][0] == 0){
		for(int i=0; i<9; i++){
			enemy[i][0] = enemy[i+1][0];
			enemy[i][1] = enemy[i+1][1];
		}
	}
}

void paintbullet(int x, int y){
	LCD_DrawLine(x-1,y,x+1,y);
	LCD_DrawLine(x,y-1,x,y+1);
}
void paintenemybullet(int x, int y){
	LCD_DrawLine(x-2,y,x+2,y);
	LCD_DrawLine(x,y-2,x,y+2);
}
void paintplayer(){
	int x=player[0];
	int y=player[1];
	if(x!=0&&y!=0){
		LCD_Fill(x-20,y-25,x+20,y+25,BACK_COLOR);
		LCD_DrawLine(x-11,y+4,x+11,y+4);
		LCD_DrawLine(x,y-20,x,y+20);
		LCD_DrawLine(x-5,y+20,x+5,y+20);
		LCD_DrawLine(x-11,y+4,x,y-20);
		LCD_DrawLine(x+11,y+4,x,y-20);
		LCD_DrawLine(x-5,y+20,x,y-20);
		LCD_DrawLine(x+5,y+20,x,y-20);
	}
}
void paintenemy(int x,int y){
	if(x!=0&&y!=0){
			LCD_Fill(x-20,y-25,x+20,y+25,BACK_COLOR);
			LCD_DrawLine(x-11,y+4,x+11,y+4);
			LCD_DrawLine(x,y-20,x,y+20);
			LCD_DrawLine(x-5,y-20,x+5,y-20);
		}
}

void paintscore(int x, int y, int size){
	char sc[10];
	itoa(score,sc,10);
	LCD_ShowString(x, y, 200, 16, size, (uint8_t*) sc);
}


void paintall(){
	paintscore(75,8,24);
	LCD_Fill(190,15,226,25,BACK_COLOR);
	for(int i=0; i<life; i++){
		LCD_DrawLine(181+i*14,15, 189+i*14,25);
		LCD_DrawLine(181+i*14,25, 189+i*14,15);
	}
	paintplayer();
	for(int i=0; i<10; i++){
			if(bullet[i][0] == 0)continue;
			else paintbullet(bullet[i][0],bullet[i][1]);
		}
	for(int i=0; i<10; i++){
				if(enemybullet[i][0] == 0)continue;
				else paintenemybullet(enemybullet[i][0],enemybullet[i][1]);
			}
	for(int i=0; i<10; i++){
		if(enemy[i][0] == 0)continue;
		else paintenemy(enemy[i][0],enemy[i][1]);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

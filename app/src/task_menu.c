/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_menu.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_menu_attribute.h"
#include "task_menu_interface.h"
#include "display.h"

/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI			0ul
#define G_TASK_MEN_TICK_CNT_INI		0ul

#define DEL_MEN_XX_MIN				0ul
#define DEL_MEN_XX_MED				50ul
#define DEL_MEN_XX_MAX				500ul

/********************** internal data declaration ****************************/
task_menu_dta_t task_menu_dta =
	{DEL_MEN_XX_MIN, ST_MENU_MAIN, EV_MEN_MEN_IDLE, false, 1,1,1,0,1};

task_motor_dta_t task_motor_dta_list[] = {
	{1, OFF, 0, LEFT},
	{2, OFF, 0, RIGHT},
};


#define MENU_DTA_QTY	(sizeof(task_menu_dta)/sizeof(task_menu_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_menu 		= "Task Menu (Interactive Menu)";
const char *p_task_menu_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_menu_cnt;
volatile uint32_t g_task_menu_tick_cnt;

/********************** external functions definition ************************/
void task_menu_init(void *parameters)
{
	task_menu_dta_t *p_task_menu_dta;
	task_menu_st_t	state;
	task_menu_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_menu_init), p_task_menu);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_menu), p_task_menu_);

	g_task_menu_cnt = G_TASK_MEN_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_menu_cnt), g_task_menu_cnt);

	init_queue_event_task_menu();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_menu_dta = &task_menu_dta;

	/* Print out: Task execution FSM */
	state = p_task_menu_dta->state;
	LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

	event = p_task_menu_dta->event;
	LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

	b_event = p_task_menu_dta->flag;
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

    displayInit( DISPLAY_CONNECTION_GPIO_4BITS );

    displayCharPositionWrite(0, 0);
	displayStringWrite("TdSE Bienvenidos");

	displayCharPositionWrite(0, 1);
	displayStringWrite("Test Nro: ");

	HAL_GPIO_WritePin(LED_A_PORT, LED_A_PIN, LED_A_ON);

	g_task_menu_tick_cnt = G_TASK_MEN_TICK_CNT_INI;
}

void task_menu_update(void *parameters)
{
	int i;
	task_menu_dta_t *p_task_menu_dta;
	task_motor_dta_t *p_task_motor_1_dta;
	task_motor_dta_t *p_task_motor_2_dta;

	bool b_time_update_required = false;

	char menu_str[30];


	/* Update Task Menu Counter */
	g_task_menu_cnt++;

	/* Protect shared resource (g_task_menu_tick) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
    {
    	g_task_menu_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_menu_tick) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
		{
			g_task_menu_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

    	/* Update Task Menu Data Pointer */
		p_task_menu_dta = &task_menu_dta;
		p_task_motor_1_dta = &task_motor_dta_list[0];
		p_task_motor_2_dta = &task_motor_dta_list[1];


    	if (DEL_MEN_XX_MIN < p_task_menu_dta->tick)
		{
			p_task_menu_dta->tick--;
		}
		else
		{


			p_task_menu_dta->tick = DEL_MEN_XX_MAX;

			if (true == any_event_task_menu())
			{
				p_task_menu_dta->flag = true;
				p_task_menu_dta->event = get_event_task_menu();
			}

			switch (p_task_menu_dta->state)
			{
				case ST_MEN_XX_IDLE:

					if ((true == p_task_menu_dta->flag) && (EV_MEN_MEN_ACTIVE == p_task_menu_dta->event))
					{
						p_task_menu_dta->flag = false;
						p_task_menu_dta->state = ST_MEN_XX_ACTIVE;
					}

					break;

				case ST_MEN_XX_ACTIVE:

					if ((true == p_task_menu_dta->flag) && (EV_MEN_MEN_IDLE == p_task_menu_dta->event))
					{
						p_task_menu_dta->flag = false;
						p_task_menu_dta->state = ST_MEN_XX_IDLE;
					}

					break;

				case ST_MENU_MAIN:

					displayCharPositionWrite(0, 0);
					sprintf(menu_str, "Motor %d: %s, %d, %s", p_task_motor_1_dta->motor_id, p_task_motor_1_dta->power == OFF ? "OFF" : "ON",
					p_task_motor_1_dta->speed, p_task_motor_1_dta->spin == LEFT ? "L" : "R");
					displayStringWrite(menu_str);

					displayCharPositionWrite(0, 1);
					sprintf(menu_str, "Motor %d: %s, %d, %s", p_task_motor_2_dta->motor_id, p_task_motor_2_dta->power == OFF ? "OFF" : "ON",
					p_task_motor_2_dta->speed, p_task_motor_2_dta->spin == LEFT ? "L" : "R");
					displayStringWrite(menu_str);


					if ((true == p_task_menu_dta->flag) && (EV_MEN_MEN_ACTIVE == p_task_menu_dta->event))
					{
						p_task_menu_dta->flag = false;
						p_task_menu_dta->state = ST_MENU_1;
						displayClear();

					}

					//Display Menu Main:



					break;

				case ST_MENU_1:
					//Display Menu #1:
					displayCharPositionWrite(0, 0);
					displayStringWrite("Enter/Next/Escape");
					displayCharPositionWrite(2, 1);
					displayStringWrite("Motor 1");
					displayCharPositionWrite(2, 2);
					displayStringWrite("Motor 2");
					displayCharPositionWrite(0, p_task_menu_dta->cursor_motor);
					displayStringWrite("> ");


					if ((true == p_task_menu_dta->flag) && (EV_MEN_ESC_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->state = ST_MENU_MAIN;

					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_ENT_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->state = ST_MENU_2;
						//displayClear();
					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_NEX_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->cursor_motor = (p_task_menu_dta->cursor_motor % 2) + 1;
						//displayClear();
					}


					//Display Menu #1:

					break;
				case ST_MENU_2:
					//Display Menu #1:
					displayCharPositionWrite(0, 0);
					displayStringWrite("Enter/Next/Escape");
					displayCharPositionWrite(2, 1);
					displayStringWrite("Power");
					displayCharPositionWrite(2, 2);
					displayStringWrite("Speed");
					displayCharPositionWrite(2, 3);
					displayStringWrite("Spin");
					displayCharPositionWrite(0, p_task_menu_dta->cursor_task);
					displayStringWrite("> ");

					if ((true == p_task_menu_dta->flag) && (EV_MEN_ESC_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->state = ST_MENU_1;

					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_ENT_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;

						switch (p_task_menu_dta->cursor_task){
							case 1:
								p_task_menu_dta->state = ST_MENU_3_POWER;
								break;
							case 2:
								p_task_menu_dta->state = ST_MENU_3_SPEED;
								break;
							case 3:
								p_task_menu_dta->state = ST_MENU_3_SPIN;
								break;
							default:
								break;
						}
						//displayClear();
					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_NEX_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->cursor_task = (p_task_menu_dta->cursor_task % 3) + 1;
						//displayClear();
					}

					break;

				case ST_MENU_3_POWER:
									//Display Menu #1:
						displayCharPositionWrite(0, 0);
						displayStringWrite("Enter/Next/Escape");
						displayCharPositionWrite(2, 1);
						displayStringWrite("ON");
						displayCharPositionWrite(2, 2);
						displayStringWrite("OFF");
						displayCharPositionWrite(0, p_task_menu_dta->cursor_power);
						displayStringWrite("> ");

						if ((true == p_task_menu_dta->flag) && (EV_MEN_ESC_ACTIVE == p_task_menu_dta->event))
						{
							displayClear();
							p_task_menu_dta->flag = false;
							p_task_menu_dta->cursor_power = 1;
							p_task_menu_dta->state = ST_MENU_2;

						}

						if ((true == p_task_menu_dta->flag) && (EV_MEN_ENT_ACTIVE == p_task_menu_dta->event))
						{
							displayClear();
							p_task_menu_dta->flag = false;

							if(p_task_menu_dta->cursor_motor == 1)
								p_task_motor_1_dta->power = p_task_menu_dta->cursor_power;

							if(p_task_menu_dta->cursor_motor == 2)
								p_task_motor_2_dta->power = p_task_menu_dta->cursor_power;
							//displayClear();
						}

						if ((true == p_task_menu_dta->flag) && (EV_MEN_NEX_ACTIVE == p_task_menu_dta->event))
						{
							displayClear();
							p_task_menu_dta->flag = false;
							p_task_menu_dta->cursor_power = (p_task_menu_dta->cursor_power % 2) + 1;
										//displayClear();
						}

						break;

				case ST_MENU_3_SPEED:
																	//Display Menu #1:
					displayCharPositionWrite(0, 0);
					displayStringWrite("Enter/Next/Escape");

					if(p_task_menu_dta->cursor_speed < 8){
						for(i = p_task_menu_dta->cursor_speed + 1 ; i < p_task_menu_dta->cursor_speed + 4; i++){

							displayCharPositionWrite(2, i - p_task_menu_dta->cursor_speed);
							snprintf(menu_str, sizeof(menu_str), "%d", (i-1));
							displayStringWrite(menu_str);

						}
						displayCharPositionWrite(0, 1); //p_task_menu_dta->cursor_speed

					}
					else{
						displayCharPositionWrite(2, 1);
						displayStringWrite("7");
						displayCharPositionWrite(2, 2);
						displayStringWrite("8");
						displayCharPositionWrite(2, 3);
						displayStringWrite("9");
						displayCharPositionWrite(0, p_task_menu_dta->cursor_speed - 6);
					}



					displayStringWrite("> ");


					if ((true == p_task_menu_dta->flag) && (EV_MEN_ESC_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->cursor_speed = 0;
						p_task_menu_dta->state = ST_MENU_2;

					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_ENT_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;

						if(p_task_menu_dta->cursor_motor == 1)
							p_task_motor_1_dta->speed = p_task_menu_dta->cursor_speed;

						if(p_task_menu_dta->cursor_motor == 2)
							p_task_motor_2_dta->speed = p_task_menu_dta->cursor_speed;
															//displayClear();
					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_NEX_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;



						if(p_task_menu_dta->cursor_speed == 9)
							p_task_menu_dta->cursor_speed = 0;
						else
							p_task_menu_dta->cursor_speed++;//displayClear();
					}

				break;


				case ST_MENU_3_SPIN:
													//Display Menu #1:
					displayCharPositionWrite(0, 0);
					displayStringWrite("Enter/Next/Escape");
					displayCharPositionWrite(2, 1);
					displayStringWrite("LEFT");
					displayCharPositionWrite(2, 2);
					displayStringWrite("RIGHT");
					displayCharPositionWrite(0, p_task_menu_dta->cursor_spin);
					displayStringWrite("> ");

					if ((true == p_task_menu_dta->flag) && (EV_MEN_ESC_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->cursor_spin = 1;
						p_task_menu_dta->state = ST_MENU_2;

					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_ENT_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;

						if(p_task_menu_dta->cursor_motor == 1)
							p_task_motor_1_dta->spin = p_task_menu_dta->cursor_spin;

						if(p_task_menu_dta->cursor_motor == 2)
							p_task_motor_2_dta->spin = p_task_menu_dta->cursor_spin;
											//displayClear();
					}

					if ((true == p_task_menu_dta->flag) && (EV_MEN_NEX_ACTIVE == p_task_menu_dta->event))
					{
						displayClear();
						p_task_menu_dta->flag = false;
						p_task_menu_dta->cursor_spin= (p_task_menu_dta->cursor_spin % 2) + 1;
														//displayClear();
					}

				break;









				default:

					p_task_menu_dta->tick  = DEL_MEN_XX_MIN;
					p_task_menu_dta->state = ST_MEN_XX_IDLE;
					p_task_menu_dta->event = EV_MEN_MEN_IDLE;
					p_task_menu_dta->flag  = false;

					break;
			}
		}
	}
}

/********************** end of file ******************************************/

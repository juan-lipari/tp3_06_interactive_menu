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
 * @file   : task_menu_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_MENU_ATTRIBUTE_H_
#define TASK_INC_TASK_MENU_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
/* Menu Statechart - State Transition Table */
/*State Transition Table - Menu.c

------------------------+-----------------------+-----------------------+-----------------------+--------------------------------------+
| Current               | Event                 |                       | Next                  |                                      |
| State                 | (Parameters)          | [Guard]               | State                 | Actions                              |
|=======================+=======================+=======================+=======================+======================================|
| ST_SYS_MENU_MAIN      | EV_SYS_MENU         	|                       | ST_SYS_MENU_1         |                                      |
|-----------------------+-----------------------+-----------------------+-----------------------+--------------------------------------|
| ST_SYS_MENU_1    		| EV_SYS_ESCAPE         |                       | ST_SYS_MENU_MAIN      | id_motor = 1 (REVISAR)               |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_NEXT           |                       | ST_SYS_MENU_1         | id_motor = (id_motor % 2) + 1        |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_ENTER          |                       | ST_SYS_MENU_2         |                                      |
|-----------------------+-----------------------+-----------------------+-----------------------+--------------------------------------|
| ST_SYS_MENU_2        	| EV_SYS_ESCAPE         |                       | ST_SYS_MENU_1         | task_motor = 1 (REVISAR)             |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_NEXT           |                       | ST_SYS_MENU_2         | task_motor = (task_motor % 3) + 1    |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_ENTER          | [task_motor == 1]     | ST_SYS_MENU_3_POWER   |                                      |
|                       |                       +-----------------------+-----------------------+--------------------------------------|
|                       |                       | [task_motor == 2]     | ST_SYS_MENU_3_SPEED   |                                      |
|                       |                       +-----------------------+-----------------------+--------------------------------------|
|						|						| [task_motor == 3]     | ST_SYS_MENU_3_SPIN    |                                      |
|-----------------------+-----------------------+-----------------------+-----------------------+--------------------------------------|
| ST_SYS_MENU_3_POWER	| EV_SYS_ESCAPE         |                       | ST_SYS_MENU_2         | cursor_power = 1 (REVISAR)           |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_NEXT           |                       | ST_SYS_MENU_3         | cursor_power = (cursor_power % 2) + 1|
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_ENTER          |                       | ST_SYS_MENU_3         | save_power_motor(cursor_power)       |
|-----------------------+-----------------------+-----------------------+-----------------------+--------------------------------------|
| ST_SYS_MENU_3_SPEED	| EV_SYS_ESCAPE         |                       | ST_SYS_MENU_2         | cursor_speed = 0 (REVISAR)           |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_NEXT           |                       | ST_SYS_MENU_3         | cursor_speed = (cursor_speed % 9)    |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_ENTER          |                       | ST_SYS_MENU_3         | save_speed_motor(cursor_speed)       |
|-----------------------+-----------------------+-----------------------+-----------------------+--------------------------------------|
| ST_SYS_MENU_3_POWER	| EV_SYS_ESCAPE         |                       | ST_SYS_MENU_2         | cursor_spin = 1 (REVISAR)            |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_NEXT           |                       | ST_SYS_MENU_3         | cursor_spin = (cursor_spin % 2) + 1  |
|                       +-----------------------+-----------------------+-----------------------+--------------------------------------|
|                       | EV_SYS_ENTER          |                       | ST_SYS_MENU_3         | save_spin_motor(cursor_spin)         |
------------------------+-----------------------+-----------------------+-----------------------+---------------------------------------
 * */

/* Events to excite Task Menu */
typedef enum task_menu_ev {EV_MEN_MEN_IDLE,
						   EV_MEN_MEN_ACTIVE,
						   EV_MEN_ENT_IDLE,
						   EV_MEN_ENT_ACTIVE,
						   EV_MEN_NEX_IDLE,
						   EV_MEN_NEX_ACTIVE,
						   EV_MEN_ESC_IDLE,
						   EV_MEN_ESC_ACTIVE} task_menu_ev_t;

/* State of Task Menu */
typedef enum task_menu_st {ST_MEN_XX_IDLE,
						   ST_MEN_XX_ACTIVE,
						   ST_MENU_MAIN,
						   ST_MENU_1,
						   ST_MENU_2,
						   ST_MENU_3_POWER,
						   ST_MENU_3_SPEED,
						   ST_MENU_3_SPIN} task_menu_st_t;

typedef enum {NUL, ON,  OFF} task_motor_power_t;

typedef enum {NULO, LEFT,  RIGHT} task_motor_spin_t;

typedef struct
{
	uint32_t		tick;
	task_menu_st_t	state;
	task_menu_ev_t	event;
	bool			flag;
	uint8_t 		cursor_motor;
	uint8_t 		cursor_task;
	uint8_t 		cursor_power;
	uint8_t 		cursor_speed;
	uint8_t 		cursor_spin;
} task_menu_dta_t;

typedef struct
{
	uint8_t		motor_id;
	task_motor_power_t	power;
	uint8_t	speed;
	task_motor_spin_t spin;
} task_motor_dta_t;

/********************** external data declaration ****************************/
extern task_menu_dta_t task_menu_dta;
//extern task_motor_dta_t task_motor_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_MENU_ATTRIBUTE_H_ */

/********************** end of file ******************************************/

/*
Copyright 2020 Massdrop Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __CONFIG_HW_VERSION_H
#define __CONFIG_HW_VERSION_H

#include "config.h"

#define HW_VERSION_CHECK_PORT   PB  
#define HW_VERSION_CHECK_PIN    16


extern void hw_version_init(void);
extern uint8_t is_hw_version_1(void);



#endif //__CONFIG_VERSION_H

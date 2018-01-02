/*
 * Various command functions for transent system
 * copyright (c) 2017 Tuan Hoang
 *
 * This file is part of Transent.
 *
 * Transent is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * Transent is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Transent; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _TRANSENT_COMMAND_
#define _TRANSENT_COMMAND_

#define METH_LEN 10
#define DATA_LEN 200

typedef struct _cmd{
    char str_cmd[METH_LEN+DATA_LEN];
    char method[METH_LEN];
    char data[DATA_LEN];
}Command;

/**
 * Validate command tring from terminal
 * @param args : pointer to command string
 * return negative number if invalid else 
 * positive 
 */
extern int valid_str_cmd(char* args);

/**
 * Parsing Command object from command string
 * @param args pointer to command string
 * @return pointer to \refCommand object
 */
extern Command* parse_cmd(char* args);

/**
 * Valid command object had in command role
 * @param cmd an Command object
 * @return negative number if invalid else 
 * positive 
 */
extern int valid_cmd(Command cmd);

/**
 * Get suggest commands related with command- 
 * -prefix string or command object
 * @param
 */
extern Command* suggest_cmd(Command cmd);
#endif
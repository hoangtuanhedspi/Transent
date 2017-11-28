/*
 * Various interface for communication between client-server 
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
#ifndef _TRANSENT_INTERFACE_
#define _TRANSENT_INTERFACE_
/**
 * @file
 * Communication interface for client-server
 */


//! Data stream buffer information
//Size of data stream buffer
#define BUFF_SIZE   2056
#define HEADER_LEN  8
#define PACK_SIZE   4
#define PAY_LEN     BUFF_SIZE - HEADER_LEN

//! Request method info stored on HEADER of data stream 
//Request file method
#define RQ_FILE     0x0001
//Request file failed
#define RQ_FAIL     0x0001 << 1
//
#define WAIT_RQ     0x0001 << 2
//Response file was found
#define RP_FOUND    0x0001 << 3
//Response file not found
#define RP_NFOUND   0x0001 << 4
//Undefine method
#define UNDEFINE    -1

typedef enum _pack_position{
    METHOD = 1,
    PAYSIZE,
    PAYLOAD
}PackPosition;

void* add_request(char* buff,int method);
int extract_request(char* buff);
void* attach_payload(char* buff,char* payload,unsigned int size);
char* detach_payload(char* buff);
int get_payload_size(char* buff);
int valid_method(int method);
int get_real_len(char* buff);

#endif
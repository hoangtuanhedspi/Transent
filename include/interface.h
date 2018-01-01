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
//Number of packet in header
#define PACK_COUNT  3
//Size for each packet
#define PACK_SIZE   4
//Header len cal
#define HEADER_LEN  (PACK_SIZE*PACK_COUNT)
//Payload len
#define PAY_LEN     3072
//Size of data stream buffer
#define BUFF_SIZE   (PAY_LEN+HEADER_LEN)

//! Request method info stored on HEADER of data stream 
//Request file method
#define RQ_FILE     0x0001
//Request file failed
#define RQ_FAIL     0x0001 << 1
//Request download file
#define RQ_DL       0x0001 << 6
//Notify infor
#define NOTI_INF    0x0001 << 2
//Response file was found
#define RP_FOUND    0x0001 << 3
//Response file not found
#define RP_NFOUND   0x0001 << 4
//Response list client struct
#define RP_FLIST    0x0001 << 5
//Undefine method
#define UNDEFINE    -1
//Stick end of data stream
#define EOF_FILE    0
//Stick data stream continue transfer
#define FSTREAM     1
//Length of strig method
#define CMETHOD_LEN 10
#define METHOD_COUNT 4
//ennumeric user request method
typedef enum __method{
    STUP,
    LOGIN,
    LOGOUT,
    FIND
}Method;

/**
 * Identify the packet position in buffer stream.
 * If you add a packet position to this list, add it so that
 * \ref PAKETCOUNT also add more 4byte at define
 */
typedef enum _pack_position{
    PACK_EMPTY,
    PACK_METHOD,
    PACK_PAYSIZE,
    PACK_WIDEN,
    PACK_PAYLOAD
}PackPosition;

void* add_request(char* buff,int method);
int extract_request(char* buff);
void* attach_payload(char* buff,char* payload,unsigned int size);
char* detach_payload(char* buff);
int detach_payload2(char* buff,char* payload);
int get_payload_size(char* buff);
int valid_method(int method);
int get_real_len(char* buff);
int stom(char* string_method);
int mtos(Method method);
void packet_info(char* buff);
#endif
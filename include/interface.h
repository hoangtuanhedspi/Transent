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
//Size of meta data
#define MD_LEN 100
//Header len cal
#define HEADER_LEN  (PACK_SIZE*PACK_COUNT+MD_LEN)
//Payload len
#define PAY_LEN     3072
//Size of data stream buffer
#define BUFF_SIZE   (PAY_LEN+HEADER_LEN+MD_LEN)

//! Request method info stored on HEADER of data stream 
//Request file method
#define RQ_FILE     0x0001
//Request file failed
#define RQ_FAIL     0x0001 << 1
//Request download file
#define RQ_DL       0x0001 << 2
//Stick data stream continue 
//reading
#define RQ_STREAM   0x0001 << 3
//Response file was found
#define RP_FOUND    0x0001 << 4
//Response file not found
#define RP_NFOUND   0x0001 << 5
//Response list client struct
#define RP_FLIST    0x0001 << 6
//Stick data stream continue
//transferring
#define RP_STREAM   0x0001 << 7
//Notify infor
#define NOTI_INF    0x0001 << 8
//Undefine method
#define UNDEFINE    -1
//Stick end of data stream
#define EOF_FILE    0
//Length of strig method
#define CMETHOD_LEN 10
#define METHOD_COUNT 5

//ennumeric user request method
typedef enum __method{
    STUP,
    LOGIN,
    LOGOUT,
    FIND,
    SELECT
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

/**
 * Add request action to header of buffer stream
 * @param buff pointer to buffer stram variable
 * @param method value of constant method
 * return void pointer
 */ 
void* add_request(char* buff,int method);

/**
 * Extract request action from buffer stream
 * 
 */
int extract_request(char* buff);

/**
 * Attach payload stream to buffer stream
 */
void* attach_payload(char* buff,char* payload,unsigned int size);

/**
 * Detach payload stream from buffer stream
 */
char* detach_payload(char* buff);

/**
 * Detach payload stream from buffer stream
 */
int detach_payload2(char* buff,char* payload);

/**
 * Get real payload size from header of buffer stream 
 */
int get_payload_size(char* buff);

/**
 * Check valid method in role
 */
int valid_method(int method);

/**
 * Get real len of buffer stream, wrap all data 
 */
int get_real_len(char* buff);

/**
 * Convert string to enum method
 */ 
int stom(char* string_method);

/**
 * Convert from enum method to string
 */ 
char* mtos(Method method);

/**
 * Print package info to command line screen
 */ 
void packet_info(char* buff);

/**
 * Add number of struct element in payload
 */ 
int add_response_number(char* buff, int number);

/**
 * Get number of struct element in payload
 */ 
int extract_response_number(char* buff);

/**
 * 
 */
void* add_meta_data(char* buff, char* meta);

/**
 * 
 */
char* get_meta_data(char* buff);
#endif
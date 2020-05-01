// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this header code is governed by the MIT license, see LICENSE

/**
 * \file uart.h
 * \brief Header file for uart communication with Bluetooth module.
 */

#ifndef SMALLDROP_PHEAD_UART_H
#define SMALLDROP_PHEAD_UART_H

#include "main.h"


/**
 * \fn void receive_command(byte incoming_byte)
 * \brief Receives each byte from serial port until it forms a complete command.
 * 
 * \param received_byte Last received byte.
 */
void receive_command(byte received_byte);

/**
 * \fn void parse_command(byte *command, byte size)
 * \brief Processes the command received.
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void process_command(byte *command, byte size);

#endif // SMALLDROP_PHEAD_UART_H

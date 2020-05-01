// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this source code is governed by the MIT license, see LICENSE

/**
 * \file uart.c
 * \brief Source file for uart communication with Bluetooth module.
 */

#include "uart.h"
#include "protocol.h"

/**
 * \copybrief void receive_command(byte received_byte)
 */
void receive_command(byte received_byte)
{
  // Parse data received
  // All system messages start with byte 0xBD.

  // If received start command code or other bytes from same command
  if ((received_byte == CMD_START_CODE && rx_pos == 0) || (rx_pos <= rx_len + 3 && rx_pos > 1))
    rx_buffer[rx_pos++] = received_byte;
  else if (rx_pos == 1) // Received len byte
  {
    rx_len = received_byte;
    rx_buffer[rx_pos++] = rx_len;
  }

  if (rx_pos == rx_len + 3) // If command complete
  {
    process_command(rx_buffer+1, rx_len); // Pass len also
    rx_pos = 0;
    rx_len = 0;
  }

  if (rx_pos == RX_BUFFER_SIZE)
  {
    // The command is too big, discard it
    rx_pos = 0;
    rx_len = 0;
  }
}

/**
 * \copybrief void parse_command(byte *command, byte size)
 */
void process_command(byte *command, byte size)
{
  // TODO: Validate message CRC

  if (command[0] == 0)
  {
    // Received PING"
    // Reset timeout
    command_reply_ok();
  }
  else
  {
    // parse command
    parse_command(command+1, size);
  }
}
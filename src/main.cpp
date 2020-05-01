// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this source code is governed by the MIT license, see LICENSE

/**
 * \file main.cpp
 * \brief Main source file.
 */

// Main header file
#include "main.h"

#include "uart.h"

/**
 * Global Vars
 *****************************************************************************/
// For variable documentation refer to the file main.h.

// Device description
const char model_name[] = "SD-PHEAD-1";

// Printing properties
word speed;
byte temp;
byte volume;
byte uv_intensity;

// Power management
#if POWER_MODE == 0
PowerMode power_mode = PLUG;
BatState bat_state = NO_BAT;
#else
PowerMode power_mode = BAT;
BatState bat_state = BAT_FULL;
#endif

// Communication
byte rx_buffer[20] = {0};
byte rx_pos = 0;
byte rx_len = 0;

void setup()
{
  // Setup serial comms for communication with HC-05 Bluetooth module
  // baud rate: B115200
  Serial.begin(115200);
}

void loop()
{
  // Check for messages sent by smalldrop system.
  // If the ping message fails a number of times, the connection is considered lost.
  if (Serial.available())
  {
    byte received_byte = Serial.read();
    receive_command(received_byte);
  }
}
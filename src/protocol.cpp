// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this source code is governed by the MIT license, see LICENSE

/**
 * \file protocol.cpp
 * \brief Source file for communication protocol between the print head and the smalldrop system.
 */

#include "main.h"
#include "protocol.h"
#include "crc.h"

/**
 * \copybrief void command_reply(byte *rsp, byte size)
 */
void command_reply(byte *data, byte size)
{
  byte msg[3+size];
  msg[0] = RSP_START_CODE;          // message code
  msg[1] = size;                    // len
  memcpy(msg+2, data, size);        // payload
  msg[size+2] = crc8(msg, size+2);  // Add CRC8 last

  // Send data to serial port
  Serial.write(msg, sizeof msg);
}

/**
 * \copybrief void command_reply_ok(void)
 */
void command_reply_ok(void)
{
  byte data = RSP_OK_CODE;
  command_reply(&data, 1);
}

/**
 * \copybrief void command_error(byte error_code)
 */
void command_error(byte code)
{
  byte msg[4];
  msg[0] = ERR_START_CODE;  // message code
  msg[1] = 1;               // len
  msg[2] = code;            // error code
  msg[3] = crc8(msg, 3);    // Add CRC8 last

  // Send data to serial port
  Serial.write(msg, sizeof msg);
}

/**
 * \copybrief void parse_general_commands(byte *command, byte size)
 */
void parse_general_commands(byte *command, byte size)
{
  switch (command[0]) // Command code
  {
  case 0x01: // init
    volume = (size > 1 && command[1] != 0) ? command[1] : volume; // Has volume argument. Set Volume.
    speed = (size > 2 && command[2] != 0) ? command[2] : speed; // Has speed argument. Set Speed.
#ifdef HAS_TEMPERATURE
    temp = (size > 3 && command[3] != 0) ? command[3] : temp; // Has temp argument. Set temperature.
#endif

    // TODO: Call init routine
    command_reply_ok();
    break;
  case 0x02: // shutdown
    // TODO: Call shutdown routine
    command_reply_ok();
    break;
  case 0x03: // description
    {
      byte data[2+strlen(model_name)];
      data[0] = FW_VERSION;
      data[1] = HW_VERSION;
      memcpy(data+2, model_name, strlen(model_name));
      command_reply(data, sizeof data);
    }
    break;
  case 0x04: // state_led
    // TODO: Manipulate state LEDs
    command_reply_ok();
    break;
  default:
    command_error(E_BAD_COMMAND);
    break;
  }
}

/**
 * \copybrief void parse_calibration_commands(byte *command, byte size)
 */
void parse_calibration_commands(byte *command, byte size)
{
  switch (command[0]) // Command code
  {
  case 0x01: // calibrate
    break;
  default:
    // TODO: Send bad command error
    break;
  }
}

/**
 * \copybrief void parse_battery_commands(byte *command, byte size)
 */
void parse_battery_commands(byte *command, byte size)
{
  switch (command[0]) // Command code
  {
  case 0x01: // read_battery
    break;
  default:
    // TODO: Send bad command error
    break;
  }
}

/**
 * \copybrief void parse_print_commands(byte *command, byte size)
 */
void parse_print_commands(byte *command, byte size)
{
  switch (command[0]) // Command code
  {
  case 0x01: // start_print
    break;
  case 0x02: // stop_print
    break;
  case 0x03: // set_speed
    break;
  case 0x04: // refill
    break;
  default:
    // TODO: Send bad command error
    break;
  }
}

/**
 * \copybrief void parse_temperature_commands(byte *command, byte size)
 */
void parse_temperature_commands(byte *command, byte size)
{
  // TODO: Send bad command error if no hardware support
}

/**
 * \copybrief void parse_uv_commands(byte *command, byte size)
 */
void parse_uv_commands(byte *command, byte size)
{
  // TODO: Send bad command error if no hardware support
}

/**
 * \copybrief void parse_command(byte *command, byte size)
 */
void parse_command(byte *command, byte size)
{
  switch (command[0]) // Command group byte
  {
  case 0x01: // General
    parse_general_commands(command+1, size-1);
    break;
  case 0x02: // Calibration
    parse_calibration_commands(command+1, size-1);
    break;
  case 0x03: // Battery
    parse_battery_commands(command+1, size-1);
    break;
  case 0x04: // Print
    parse_print_commands(command+1, size-1);
    break;
  case 0x05: // Temperature
    parse_temperature_commands(command+1, size-1);
    break;
  case 0x06: // UV lights
    parse_uv_commands(command+1, size-1);
    break;
  default:
    // TODO: Send bad command error
    break;
  }
}

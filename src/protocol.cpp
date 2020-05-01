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
    if (size > 1 && command[1] != 0) // Has volume argument. Set Volume.
    {
      bool valid_volume = false;
      for (byte i = 0; i < sizeof valid_volumes; i++)
      {
        if (command[1] == valid_volumes[i])
        {
          volume = command[1];
          valid_volume = true;
          break;
        }
      }
      if (!valid_volume)
      {
        volume = VOL_10ML; // Set default volume
        command_error(E_INVALID_VOLUME);
      }
    }
    else
      volume = VOL_10ML; // Set default volume

    if (size > 3) // Has speed argument. Set Speed.
    {
      if (command[2] != 0 && command[3] != 0) // Speed is not zero
        speed = command[2] | ((word)command[3] << 8);
    }
    else
      speed = 5; // Set default speed (5 ml/s)  

#ifdef HAS_TEMPERATURE
    temp = (size > 4 && command[4] != 0) ? command[4] : 40; // Has temp argument. Set temperature.
#endif

    command_reply_ok();
    // TODO: Call init routine
    break;
  case 0x02: // shutdown
    command_reply_ok();
    // TODO: Call shutdown routine
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
    if (size > 1)
    {
      bool valid_volume = false;
      for (byte i = 0; i < sizeof valid_volumes; i++)
      {
        if (command[1] == valid_volumes[i])
        {
          volume = command[1];
          valid_volume = true;
          break;
        }
      }
      if (!valid_volume)
      {
        command_error(E_INVALID_VOLUME);
        return;
      }

      // TODO: Run calibration routine
      command_reply_ok();
    }
    else
      command_error(E_BAD_COMMAND);
    break;
  default:
    command_error(E_BAD_COMMAND);
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
    if (power_mode == PowerMode::BAT)
    {
      byte data[2];
      data[0] = bat_val;
      data[1] = bat_val >> 8;
      command_reply(data, 2);
    }
    else
      command_error(E_NO_BAT);
    break;
  default:
    command_error(E_BAD_COMMAND);
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
    // TODO: Start motor to extrude bioink at configured speed.
    command_reply_ok();
    break;
  case 0x02: // stop_print
    // TODO: Stop motor.
    command_reply_ok();
    break;
  case 0x03: // set_speed
    if (size > 2) // Has speed argument. Set Speed.
    {
      if (command[1] != 0 && command[2] != 0) // Speed is not zero
        speed = command[1] | ((word)command[2] << 8);
      command_reply_ok();
    }
    else
      command_error(E_BAD_COMMAND);
    break;
  case 0x04: // refill
    command_reply_ok();
    break;
  default:
    command_error(E_BAD_COMMAND);
    break;
  }
}

/**
 * \copybrief void parse_temperature_commands(byte *command, byte size)
 */
void parse_temperature_commands(byte *command, byte size)
{
#ifdef HAS_TEMPERATURE
  switch (command[0]) // Command code
  {
  case 0x01: // get_temperature
    command_reply(&temp, 1);
    break;
  case 0x02: // set_temperature
    if (size > 1) // Has temperature arg. Set temperature.
    {
      if (command[1] != 0) // Temperature is not zero
        temp = command[1];
      command_reply_ok();
    }
    else
      command_error(E_BAD_COMMAND);
    break;
  default:
    command_error(E_BAD_COMMAND);
    break;
  }
#else  
  command_error(E_NO_TEMP);
#endif
}

/**
 * \copybrief void parse_uv_commands(byte *command, byte size)
 */
void parse_uv_commands(byte *command, byte size)
{
#ifdef HAS_UV_LIGHTS
  switch (command[0]) // Command code
  {
  case 0x01: // turn_on_uv
    // TODO: Turn on the UV lights
    command_reply_ok();
    break;
  case 0x02: // turn_off_uv
    // TODO: Turn off the UV lights
    command_reply_ok();
    break;
  case 0x03: // set_uv_intensity
    if (size > 1) // Has intensity arg. Set UV intensity
    {
      uv_intensity = command[1];
      command_reply_ok();
    }
    else
      command_error(E_BAD_COMMAND);
    break;
  case 0x04: // set_uv_map
    if (size > 1)
    {
      if (command[1] != 0) // Map is not zero
        uv_map = command[1];
      command_reply_ok();
    }
    else
      command_error(E_BAD_COMMAND);
    break;
  default:
    command_error(E_BAD_COMMAND);
    break;
  }
#else  
  command_error(E_NO_UV);
#endif
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
    command_error(E_BAD_COMMAND);
    break;
  }
}

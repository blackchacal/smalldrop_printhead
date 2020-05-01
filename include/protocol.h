// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this header code is governed by the MIT license, see LICENSE

#ifndef SMALLDROP_PHEAD_PROTOCOL_H
#define SMALLDROP_PHEAD_PROTOCOL_H

#include "main.h"

/**
 * \fn void command_reply(byte *rsp, byte size)
 * \brief Reply to system commands.
 * 
 * \param data Response payload data
 * \param size Response payload data size
 */
void command_reply(byte *data, byte size);

/**
 * \fn void command_reply_ok(void)
 * \brief OK reply to system commands, whenever everything is right and no data is sent.
 */
void command_reply_ok(void);

/**
 * \fn void command_error(byte error_code)
 * \brief Reply to system commands with an error.
 * 
 * \param code Error code
 */
void command_error(byte code);

/**
 * \fn void parse_general_commands(byte *command, byte size)
 * \brief Parses the system commands related with general functionality
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void parse_general_commands(byte *command, byte size);

/**
 * \fn void parse_calibration_commands(byte *command, byte size)
 * \brief Parses the system commands related to calibration
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void parse_calibration_commands(byte *command, byte size);

/**
 * \fn void parse_battery_commands(byte *command, byte size)
 * \brief Parses the system commands related to battery management
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void parse_battery_commands(byte *command, byte size);

/**
 * \fn void parse_print_commands(byte *command, byte size)
 * \brief Parses the system commands related to printing
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void parse_print_commands(byte *command, byte size);

/**
 * \fn void parse_temperature_commands(byte *command, byte size)
 * \brief Parses the system commands related to temperature control
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void parse_temperature_commands(byte *command, byte size);

/**
 * \fn void parse_uv_commands(byte *command, byte size)
 * \brief Parses the system commands related to UV lights
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void parse_uv_commands(byte *command, byte size);

/**
 * \fn void parse_command(byte *command, byte size)
 * \brief Parses the system commands
 * 
 * \param command Pointer to command's first byte 
 * \param size Command size 
 */
void parse_command(byte *command, byte size);

#endif // SMALLDROP_PHEAD_PROTOCOL_H
// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this header code is governed by the MIT license, see LICENSE

#ifndef SMALLDROP_PHEAD_MAIN_H
#define SMALLDROP_PHEAD_MAIN_H

#include "stdint.h"

/**
 * MACROS
 *****************************************************************************/

//  Power
#define POWER_MODE  0  // 0 - Power supply | 1 - Battery powered

// Device properties
#define FW_VERSION      0x10 // v1.0, [<4msb>.<4lsb>]. Ex.: v2.10 = 0x2A
#define HW_VERSION      0x10 // v1.0, [<4msb>.<4lsb>]. Ex.: v2.10 = 0x2A
#define MODELNAME_SIZE  20 // Max number of model name characters

// Connection
#define PING_TIMEOUT 5 // Max number of seconds between consecutive ping messages, before connection timeout.

// Error codes
#define E_INVALID_CRC  0x00
#define E_BAD_COMMAND  0x01
#define E_BAD_COMMS    0x02
#define E_BAD_UV_MAP   0x03
#define E_OTHER        0xFF

// Buffers
#define RX_BUFFER_SIZE 20

/**
 * Typedefs
 *****************************************************************************/

/**
 * \typedef byte
 */
typedef uint8_t byte;

/**
 * \typedef word_t
 */
typedef uint16_t word;

/**
 * Structs & Enums
 *****************************************************************************/

enum PowerMode
{
  PLUG, // Powered from external power supply
  BAT, // Battery powered
};

enum BatState
{
  NO_BAT,
  BAT_CHARGING,
  BAT_DEAD,
  BAT_LOW,
  BAT_MEDIUM,
  BAT_FULL,
};

/**
 * Global Vars
 *****************************************************************************/

// Print head description
extern byte model_name[MODELNAME_SIZE]; /** \var Model name string. */

// Printing properties
extern word speed; /** \var Print speed in ml/s (0-65535). */
extern byte temp; /** \var Bioink temperature in degrees Celsius (0-255).*/
extern byte volume; /** \var bioink volume, i.e., the volume of the syringe used in ml (0-255). */
extern byte uv_intensity; /** \var UV lights intensity. */

// Power management
extern PowerMode power_mode; /** \var Power mode used by the print head. */
extern BatState bat_state; /** \var The state of the battery. Only considered if battery powered. */

// Communication
extern byte rx_buffer[RX_BUFFER_SIZE]; /** \var System command receive buffer. */
extern byte rx_pos; /** \var Index of next empty rx_buffer position */
extern byte rx_len; /** \var Length of payload for system command */

#endif // SMALLDROP_PHEAD_MAIN_H
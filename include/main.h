// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this header code is governed by the MIT license, see LICENSE

#ifndef SMALLDROP_PHEAD_MAIN_H
#define SMALLDROP_PHEAD_MAIN_H

#include "stdint.h"
#include "string.h"

#include "Arduino.h"

/**
 * MACROS
 *****************************************************************************/

// Specific functionalities
// #define HAS_TEMPERATURE // Uncomment if device has temperature control support.
// #define HAS_UV_LIGHTS // Uncomment if device has UV lights for print curing.

//  Power
#define POWER_MODE  0  // 0 - Power supply | 1 - Battery powered

// Device properties
#define FW_VERSION      0x10 // v1.0, [<4msb>.<4lsb>]. Ex.: v2.10 = 0x2A
#define HW_VERSION      0x10 // v1.0, [<4msb>.<4lsb>]. Ex.: v2.10 = 0x2A
#define MODELNAME_SIZE  20 // Max number of model name characters

// Connection
#define PING_TIMEOUT 5 // Max number of seconds between consecutive ping messages, before connection timeout.

// Protocol
#define CMD_START_CODE  0xBD
#define EVT_START_CODE  0xEE
#define RSP_START_CODE  0xA0
#define RSP_OK_CODE     0x00
#define ERR_START_CODE  0xA1

// Error codes
#define E_INVALID_CRC  0x00
#define E_BAD_COMMAND  0x01
#define E_BAD_COMMS    0x02
#define E_BAD_UV_MAP   0x03
#define E_OTHER        0xFF

// Event codes
#define EVT_END_INIT      0x01
#define EVT_END_SHUTDOWN  0x02
#define EVT_END_CALIB     0x03
#define EVT_LOW_BATT      0x04
#define EVT_NOINK         0x05
#define EVT_END_REFILL    0x06

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
extern const char model_name[MODELNAME_SIZE]; /** \var Model name string. */

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
// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this source code is governed by the MIT license, see LICENSE

/**
 * \file crc.cpp
 * \brief Source file for communication protocol between the print head and the smalldrop system.
 */

#include "main.h"
#include "crc.h"

/**
 * \copybrief byte crc8(byte *data, byte size)
 */
byte crc8(byte *data, byte size)
{
  return 0xCC;
}
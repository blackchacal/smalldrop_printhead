// Copyright (c) 2019-2020 Ricardo Tonet
// Use of this header code is governed by the MIT license, see LICENSE

#ifndef SMALLDROP_PHEAD_CRC_H
#define SMALLDROP_PHEAD_CRC_H

#include "main.h"

/**
 * \fn byte crc8(byte *data, byte size)
 * \brief Calculate CRC8
 * 
 * \param data Data to calculate CRC
 * \param size Data size
 */
byte crc8(byte *data, byte size);

#endif // SMALLDROP_PHEAD_CRC_H
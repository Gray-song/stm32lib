/**
  ******************************************************************************
  * @file    crc.h
  * @author  gray
  * @version V1.0.0
  * @date    4-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _CRC_H_
#define _CRC_H_
#include "headers.h"
extern uint16_t crc16(const uint8_t *buff,uint32_t len);
extern uint32_t crc32(const uint8_t *buff,uint32_t len);
extern uint8_t crc8(const uint8_t *buff,uint32_t len);
extern uint16_t crc16_search_table(const uint8_t buffer[],  uint8_t size);
#endif

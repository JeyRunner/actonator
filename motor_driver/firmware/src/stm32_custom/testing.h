#pragma once

#include "usbd_cdc_if.h"


void testWriteUsb() {
  uint8_t buffer[] = "Hello, World! ## some test str !\r\n";
  CDC_Transmit_FS(buffer, sizeof(buffer));
}
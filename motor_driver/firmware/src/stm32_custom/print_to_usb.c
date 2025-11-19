#include "usbd_cdc_if.h"

// Redirect standard output to the USB port
int _write(int file, char *ptr, int len)
{
  CDC_Transmit_FS( (uint8_t*) ptr, len);
  return len;
}
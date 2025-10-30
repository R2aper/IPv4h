#include "ipv4hlib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int bytes2netmask(const uint8_t bytes[IPV4_ADDRESS_SIZE]) {
  if (!bytes) {
    return -1;
  }

  // count 1 in bytes
  int netmask = 0;
  for (size_t i = 0; i < IPV4_ADDRESS_SIZE; i++) {
    uint8_t n = bytes[i];
    for (size_t j = 0; n > 0; j++) {
      if (n % 2)
        netmask++;
      n = n / 2;
    }
  }

  return netmask;
}

int netmask2bytes(uint8_t out_bytes[IPV4_ADDRESS_SIZE], int netmask) {
  if (!out_bytes) {
    return -1;
  }

  FILL_BYTES(out_bytes, 0, 0, 0, 0);

  for (int i = 0; i < netmask; i++) {
    if (i < 8) // first octate
      out_bytes[0] += powl(2, i % 8);

    if (i >= 8 && i < 16) // second octate
      out_bytes[1] += powl(2, i % 8);

    if (i >= 16 && i < 24) // third octate
      out_bytes[2] += powl(2, i % 8);

    if (i >= 24 && i < 32) // fourth octate
      out_bytes[3] += powl(2, i % 8);
  }

  return 0;
}

/*--------------------------------Constructors----------------------------------*/

ipv4_address ipv4_address_with_netmask_bytes(
    const uint8_t address_bytes[IPV4_ADDRESS_SIZE],
    const uint8_t netmask_bytes[IPV4_ADDRESS_SIZE]) {
  if (!address_bytes || !netmask_bytes) {
    return (ipv4_address){0};
  }

  ipv4_address a;

  for (size_t i = 0; i < IPV4_ADDRESS_SIZE; i++) {
    a.address_data[i] = address_bytes[i];
    a.netmask_data[i] = netmask_bytes[i];
  }

  return a;
}

ipv4_address
ipv4_address_with_netmask(const uint8_t address_bytes[IPV4_ADDRESS_SIZE],
                          int netmask) {
  if (!address_bytes) {
    return (ipv4_address){0};
  }

  if (netmask > IPV4_ADDRESS_BYTES) {
    return (ipv4_address){0};
  }

  ipv4_address a;
  netmask2bytes(a.netmask_data, netmask);

  for (size_t i = 0; i < IPV4_ADDRESS_SIZE; i++)
    a.address_data[i] = address_bytes[i];

  return a;
}

ipv4_address ipv4_address_from_address_bytes(
    const uint8_t address_bytes[IPV4_ADDRESS_SIZE]) {
  if (IS_A_CLASS(address_bytes[0]))
    return ipv4_address_with_netmask(address_bytes, CLASS_A_NETMASK);

  if (IS_B_CLASS(address_bytes[0]))
    return ipv4_address_with_netmask(address_bytes, CLASS_B_NETMASK);

  if (IS_C_CLASS(address_bytes[0]))
    return ipv4_address_with_netmask(address_bytes, CLASS_C_NETMASK);

  return ipv4_address_with_netmask(address_bytes, 0);
}

/*--------------------------------Constructors----------------------------------*/

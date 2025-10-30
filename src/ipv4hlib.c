#include "ipv4hlib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int ipv4h_error = 0;

enum IPV4H_ERROR {
  OK = 0,
  INVALID_ADDRESS = -1,
  INVALID_MASK = -2,
};

const char *ipv4h_error_str() {
  switch (ipv4h_error) {
  case INVALID_ADDRESS:
    return "Get invalid ipv4 address!";
  case INVALID_MASK:
    return "Get invalid ipv4 mask!";
  case OK:
    return "Success";
  default:
    return "Unkown error!";
  }
}

int bytes2netmask(const uint8_t bytes[IPV4_ADDRESS_SIZE]) {
  if (!bytes) {
    ipv4h_error = INVALID_MASK;
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
    ipv4h_error = INVALID_MASK;
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
  if (!address_bytes) {
    ipv4h_error = INVALID_ADDRESS;
    return (ipv4_address){0};
  }

  if (!netmask_bytes) {
    ipv4h_error = INVALID_MASK;
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
    ipv4h_error = INVALID_ADDRESS;
    return (ipv4_address){0};
  }

  if (netmask > IPV4_ADDRESS_BYTES) {
    ipv4h_error = INVALID_MASK;
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

uint32_t available_hosts(const ipv4_address a) {
  // 2^(32 - mask) - 2
  return pow(2, (IPV4_ADDRESS_BYTES - bytes2netmask(a.netmask_data))) - 2;
}

ipv4_address network_address(const ipv4_address a) {
  ipv4_address ntwk;
  FILL_BYTES(ntwk.address_data, 0, 0, 0, 0);
  FILL_BYTES(ntwk.netmask_data, 0, 0, 0, 0);

  // network address = (address byte and netmask byte)
  for (size_t i = 0; i < IPV4_ADDRESS_SIZE; i++)
    ntwk.address_data[i] = a.address_data[i] & a.netmask_data[i];

  return ntwk;
}

ipv4_address broadcast_address(const ipv4_address a) {
  ipv4_address b;
  FILL_BYTES(b.address_data, 0, 0, 0, 0);
  FILL_BYTES(b.netmask_data, 0, 0, 0, 0);

  // broadcast address = (address byte or !mask byte)
  for (size_t i = 0; i < IPV4_ADDRESS_SIZE; i++)
    b.address_data[i] = a.address_data[i] | ~a.netmask_data[i];

  return b;
}

char get_address_class(const ipv4_address a) {
  if (IS_A_CLASS(a.address_data[0]))
    return 'A';

  if (IS_B_CLASS(a.address_data[0]))
    return 'B';

  if (IS_C_CLASS(a.address_data[0]))
    return 'C';

  if (IS_D_CLASS(a.address_data[0]))
    return 'D';

  if (IS_E_CLASS(a.address_data[0]))
    return 'E';

  return -1;
}

void ipv4_address_print(const ipv4_address a, bool use_CIDR_notation) {
  printf("%u.%u.%u.%u", a.address_data[0], a.address_data[1], a.address_data[2],
         a.address_data[3]);

  int netmask = bytes2netmask(a.netmask_data);

  if (netmask != 0) {
    if (use_CIDR_notation)
      printf("/%d", bytes2netmask(a.netmask_data));
    else
      printf(".%u.%u.%u.%u", a.netmask_data[0], a.netmask_data[1],
             a.netmask_data[2], a.netmask_data[3]);
  }
}

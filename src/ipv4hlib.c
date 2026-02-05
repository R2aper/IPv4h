#include "ipv4hlib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ipv4h_error = 0;

enum IPV4H_ERROR {
  OK = 0,
  INVALID_ADDRESS = -1,
  INVALID_MASK = -2,
  INVALID_ADDRESS_STRING = -3,
  INVALID_STRING_LENGTH = -4,
  PARSING_ERROR = -5,

};

const char *ipv4h_error_str() {
  switch (ipv4h_error) {
  case INVALID_ADDRESS:
    return "Get invalid ipv4 address!";
  case INVALID_MASK:
    return "Get invalid ipv4 mask!";
  case INVALID_ADDRESS_STRING:
    return "Get invalid ipv4 address from string!";
  case INVALID_STRING_LENGTH:
    return "Ipv4 address have invalid length!";
  case PARSING_ERROR:
    return "Parsing ipv4 address failed!";
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

  uint32_t m = ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) | ((uint32_t)bytes[2] << 8) |
               ((uint32_t)bytes[3]);

  if ((~m & (~m + 1)) != 0) { // Check invalid mask
    ipv4h_error = INVALID_MASK;
    return -1;
  }

  int netmask = 0;
  while (m > 0) {
    if (m % 2)
      netmask++;

    m /= 2;
  }

  return netmask;
}

int netmask2bytes(uint8_t out_bytes[IPV4_ADDRESS_SIZE], int netmask) {
  if (!out_bytes || netmask > 32) {
    ipv4h_error = INVALID_MASK;
    return -1;
  }

  uint32_t mask = (netmask == 0) ? 0x00000000U : (0xFFFFFFFFU << (32 - netmask));

  out_bytes[0] = (mask >> 24) & 0xFF;
  out_bytes[1] = (mask >> 16) & 0xFF;
  out_bytes[2] = (mask >> 8) & 0xFF;
  out_bytes[3] = mask & 0xFF;

  return 0;
}

/*--------------------------------Constructors----------------------------------*/

ipv4_address ipv4_address_with_netmask_bytes(const uint8_t address_bytes[IPV4_ADDRESS_SIZE],
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

ipv4_address ipv4_address_with_netmask(const uint8_t address_bytes[IPV4_ADDRESS_SIZE],
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

static ipv4_address ipv4_address_from_str_with_netmask(const char *buff_str, char *slash_pos) {
  ipv4_address a;
  FILL_BYTES(a.address_data, 0, 0, 0, 0);
  FILL_BYTES(a.netmask_data, 0, 0, 0, 0);

  *slash_pos = '\0';
  slash_pos++;

  // Read address
  if (sscanf(buff_str, "%hhu.%hhu.%hhu.%hhu", &a.address_data[0], &a.address_data[1],
             &a.address_data[2], &a.address_data[3]) != 4) {
    ipv4h_error = PARSING_ERROR;
    return (ipv4_address){0};
  }

  // Read netmask
  int netmask = 0;
  if (sscanf(slash_pos, "%d", &netmask) != 1) {
    ipv4h_error = PARSING_ERROR;
    return (ipv4_address){0};
  }

  if (netmask > IPV4_ADDRESS_BYTES) {
    ipv4h_error = INVALID_MASK;
    return (ipv4_address){0};
  }

  // Convert netmask to bytes
  netmask2bytes(a.netmask_data, netmask);

  return a;
}

static ipv4_address ipv4_address_from_str_with_netmask_bytes(const char *buff_str) {
  ipv4_address a;
  FILL_BYTES(a.address_data, 0, 0, 0, 0);
  FILL_BYTES(a.netmask_data, 0, 0, 0, 0);

  int count = sscanf(buff_str, "%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu.%hhu", &a.address_data[0],
                     &a.address_data[1], &a.address_data[2], &a.address_data[3], &a.netmask_data[0],
                     &a.netmask_data[1], &a.netmask_data[2], &a.netmask_data[3]);

  return (count == 8) ? a : (ipv4_address){0};
}

ipv4_address ipv4_address_from_str(const char *str) {
  if (!str) {
    ipv4h_error = INVALID_ADDRESS_STRING;
    return (ipv4_address){0};
  }

  size_t str_len = strlen(str);
  if (str_len < 7 || str_len > 256) {
    ipv4h_error = INVALID_STRING_LENGTH;
    return (ipv4_address){0};
  }

  ipv4_address a;
  FILL_BYTES(a.address_data, 0, 0, 0, 0);
  FILL_BYTES(a.netmask_data, 0, 0, 0, 0);

  char buff[256];
  strncpy(buff, str, sizeof(buff) - 1);
  buff[sizeof(buff) - 1] = '\0';

  char *slash_pos = strchr(buff, '/');
  if (slash_pos) // String format: '0.0.0.0/0'
    return ipv4_address_from_str_with_netmask(buff, slash_pos);

  a = ipv4_address_from_str_with_netmask_bytes(buff);
  if (!IPV4_ADDRESS_IS_ZERO(a)) // String format: '0.0.0.0.0.0.0.0'
    return a;

  // String format: '0.0.0.0'
  if (sscanf(buff, "%hhu.%hhu.%hhu.%hhu", &a.address_data[0], &a.address_data[1],
             &a.address_data[2], &a.address_data[3]) != 4) {
    ipv4h_error = PARSING_ERROR;
    return (ipv4_address){0};
  }

  return ipv4_address_from_address_bytes(a.address_data);
}

ipv4_address ipv4_address_from_address_bytes(const uint8_t address_bytes[IPV4_ADDRESS_SIZE]) {
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
  printf("%u.%u.%u.%u", a.address_data[0], a.address_data[1], a.address_data[2], a.address_data[3]);

  int netmask = bytes2netmask(a.netmask_data);

  if (netmask != 0) {
    if (use_CIDR_notation)
      printf("/%d", netmask);
    else
      printf(".%u.%u.%u.%u", a.netmask_data[0], a.netmask_data[1], a.netmask_data[2],
             a.netmask_data[3]);
  }
}

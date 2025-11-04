#ifndef IPV4HLIB_H
#define IPV4HLIB_H

#include <stdbool.h>
#include <stdint.h>

#define IPV4_ADDRESS_BYTES 32                    // bytes
#define IPV4_ADDRESS_SIZE IPV4_ADDRESS_BYTES / 8 // octates

#define CLASS_A_NETMASK 8
#define CLASS_B_NETMASK 16
#define CLASS_C_NETMASK 24

#define IS_A_CLASS(first_octate) first_octate >= 0 && first_octate <= 127
#define IS_B_CLASS(first_octate) first_octate >= 128 && first_octate <= 191
#define IS_C_CLASS(first_octate) first_octate >= 192 && first_octate <= 223
#define IS_D_CLASS(first_octate) first_octate >= 224 && first_octate <= 239
#define IS_E_CLASS(first_octate) first_octate >= 240 && first_octate <= 255

#define INIT_BYTES(first_octate, second_octate, third_octate, fourth_octate)                       \
  (uint8_t[]) { first_octate, second_octate, third_octate, fourth_octate }

#define FILL_BYTES(bytes, first_octate, second_octate, third_octate, fourth_octate)                \
  bytes[0] = first_octate;                                                                         \
  bytes[1] = second_octate;                                                                        \
  bytes[2] = third_octate;                                                                         \
  bytes[3] = fourth_octate;

#define IPV4_ADDRESS_IS_ZERO(a)                                                                    \
  (a.address_data[0] == 0 && a.address_data[1] == 0 && a.address_data[2] == 0 &&                   \
   a.address_data[3] == 0 && a.netmask_data[0] == 0 && a.netmask_data[1] == 0 &&                   \
   a.netmask_data[2] == 0 && a.netmask_data[3] == 0)

extern int ipv4h_error;

/// @return error message
const char *ipv4h_error_str();

/// Struct for representing IPv4 addresses
typedef struct ipv4_address {
  uint8_t address_data[IPV4_ADDRESS_SIZE]; // Static array of address bytes
  uint8_t netmask_data[IPV4_ADDRESS_SIZE]; // Static array of netmask bytes

} ipv4_address;

/// @brief Convert netmask's bytes into number
int bytes2netmask(const uint8_t bytes[IPV4_ADDRESS_SIZE]);

/// @brief Convert number into netmask's bytes
int netmask2bytes(uint8_t out_bytes[IPV4_ADDRESS_SIZE], int netmask);

/*--------------------------------Constructors----------------------------------*/

/// @brief Init ipv4 address from address and netmask bytes
ipv4_address ipv4_address_with_netmask_bytes(const uint8_t address_bytes[IPV4_ADDRESS_SIZE],
                                             const uint8_t netmask_bytes[IPV4_ADDRESS_SIZE]);

/// @brief Init ipv4 address from address and netmask number
ipv4_address ipv4_address_with_netmask(const uint8_t address_bytes[IPV4_ADDRESS_SIZE], int netmask);

/// @brief Init ipv4 address from address bytes
ipv4_address ipv4_address_from_address_bytes(const uint8_t address_bytes[IPV4_ADDRESS_SIZE]);

/// @brief Init ipv4 address from string
ipv4_address ipv4_address_from_str(const char *str);

/// @brief Init ipv4 address
/// (a.b.c.d)
#define ipv4_address_new(a, b, c, d) ipv4_address_from_address_bytes(INIT_BYTES(a, b, c, d))

/*--------------------------------Constructors----------------------------------*/

/// @return number of available hosts of @a address
uint32_t available_hosts(const ipv4_address a);

/// @return network address by using mask
ipv4_address network_address(const ipv4_address a);

/// @return broadcast address by using mask
ipv4_address broadcast_address(const ipv4_address a);

/// @return @a class(based on first octate of address bytes)
char get_address_class(const ipv4_address a);

/// @brief print ipv4 address into stdout
void ipv4_address_print(const ipv4_address a, bool use_CIDR_notation);

#define ipv4_address_println(a, use_CIDR_notation)                                                 \
  ipv4_address_print(a, use_CIDR_notation);                                                        \
  putchar('\n')

#endif // IPV4HLIB_H

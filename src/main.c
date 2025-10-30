#include <stdio.h>

#include "ipv4hlib.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  ipv4_address a = ipv4_address_new(198, 0, 0, 1);
  printf("Ip-address:");
  ipv4_address_println(a, true);

  printf("Class:'%c'\n", get_address_class(a));

  ipv4_address network_a = network_address(a);
  printf("Network address:");
  ipv4_address_println(network_a, true);

  printf("Broadcast address:");
  ipv4_address broadcast_a = broadcast_address(a);
  ipv4_address_println(broadcast_a, true);

  printf("Available hosts:%u\n", available_hosts(a));

  return 0;
}

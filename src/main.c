#include <stdio.h>

#include "ipv4hlib.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  ipv4_address a = ipv4_address_new(198, 0, 0, 1);
  printf("%d.%d.%d.%d/%d\n", a.address_data[0], a.address_data[1],
         a.address_data[2], a.address_data[3], bytes2netmask(a.netmask_data));

  return 0;
}

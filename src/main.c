#include <stdio.h>
#include <string.h>

#include "ipv4hlib.h"

void usage() {
  puts("Usage: ipv4h [FLAGS]...[Ipv4 address]\n"
       "Getting info of ipv4 address in human format\n"
       "Flags:\n"
       "--no-cidr\tDon't use CIDR notation\n"
       "--network\tPrint only network address\n"
       "--broadcast\tPrint only broadcast address\n"
       "--class\t\tPrint only class of ipv4 address\n"
       "--hosts\t\tPrint only number of available hosts\n"
       "-h\t\tPrint this help message");
}

int main(int argc, char **argv) {
  if (argc < 2) {
    usage();
    return -1;
  }

  ipv4_address ip, network_ip, broadcast_ip;
  bool network_flag = false, broadcast_flag = false, class_flag = false,
       hosts_flag = false, use_CIDR_notation = true;

  char *ip_str = NULL;
  for (int i = 1; i < argc; i++) {
    char *token = argv[i];

    if (token[0] == '-') {
      if (strcmp(token, "-h") == 0) {
        usage();
        return 0;

      } else if (strcmp(token, "--no-cidr") == 0)
        use_CIDR_notation = false;

      else if (strcmp(token, "--network") == 0)
        network_flag = true;

      else if (strcmp(token, "--broadcast") == 0)
        broadcast_flag = true;

      else if (strcmp(token, "--class") == 0)
        class_flag = true;

      else if (strcmp(token, "--hosts") == 0)
        hosts_flag = true;
      else {
        fprintf(stderr, "Unknown flag: %s\n", token);
        return -1;
      }

    } else {
      if (ip_str) {
        fprintf(stderr, "Get more than 1 ipv4 address!\n");
        return -1;
      }
      ip_str = token;
    }
  }

  // If no flags provided print full info
  if (!network_flag && !broadcast_flag && !class_flag && !hosts_flag)
    network_flag = broadcast_flag = class_flag = hosts_flag = true;

  ip = ipv4_address_from_str(ip_str);
  if (IPV4_ADDRESS_IS_ZERO(ip)) {
    fprintf(stderr, "Error! %s\n", ipv4h_error_str());
    return -1;
  }

  printf("IPv4: ");
  ipv4_address_println(ip, use_CIDR_notation);

  if (network_flag) {
    network_ip = network_address(ip);
    printf("Network address: ");
    ipv4_address_println(network_ip, use_CIDR_notation);
  }

  if (broadcast_flag) {
    broadcast_ip = broadcast_address(ip);
    printf("Broadcast address: ");
    ipv4_address_println(broadcast_ip, use_CIDR_notation);
  }

  if (hosts_flag)
    printf("Available hosts: %u\n", available_hosts(ip));

  if (class_flag)
    printf("Class: '%c'\n", get_address_class(ip));

  return 0;
}

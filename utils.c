#include "utils.h"

#include <stdio.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

void print_mac(const u_int8_t *mac_)
{
	int i = 0;
	for (i = 0; i < ETH_ALEN; i++) {
		if (i > 0) {
			printf(":");
		}
		printf("%.2x", mac_[i]);
	}

}


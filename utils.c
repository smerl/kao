#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

char* get_mac(const u_int8_t *mac)
{	
	char *s_mac = malloc(sizeof(char) * (ETH_ALEN * 3) + 1);
	if (s_mac == NULL) {
		return NULL;
	}
	memset(s_mac, 0, sizeof(char) * (ETH_ALEN * 3));

	int i, n;
	n = 0;

	for (i = 0; i < (ETH_ALEN); i++) {
		// snprintf returns the number of written bytes
		n += snprintf(s_mac + n, 4, "%.02x:", mac[i]);
		printf("--%s\n", s_mac);
	}

	// we remove the last :
	s_mac[n-1] = '\0';

	return s_mac;
}


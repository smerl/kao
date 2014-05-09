#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

char* get_mac(const u_int8_t *mac)
{	
	char *s_mac = malloc(sizeof(char) * (ETH_ALEN * 3));
	if (s_mac == NULL) {
		return NULL;
	}
	memset(s_mac, 0, sizeof(char) * (ETH_ALEN * 3));

	int i = 0;
	for (i = 0; i < (ETH_ALEN); i++) {
		if (i > 0) {
			s_mac[strlen(s_mac)] = ':';
		}
		char buf[3];
		snprintf(buf, 3, "%.02x", mac[i]);
		s_mac[strlen(s_mac)] = (char)buf[0];
		s_mac[strlen(s_mac)] = (char)buf[1];
	}

	return s_mac;
}


#include <stdio.h>
#include <stdlib.h>

#include <netinet/if_ether.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pcap.h>

#include "utils.h"

void packet_handler(u_char *, const struct pcap_pkthdr *, const u_char *);
u_int16_t eth_handler(u_char *, const struct pcap_pkthdr *, const u_char *);
u_int16_t ipv4_handler(void);
u_int16_t tcp4_handler(void);

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [pcap file]\n", argv[0]);
		return -1;
	}

	char *filename = argv[1];
	char *filter_exp = argv[2];
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	struct pcap_pkthdr *header;
	const u_char *packet;
	struct bpf_program fp;

	handle = pcap_open_offline(filename, errbuf);

	if (handle == NULL) {
		fprintf(stderr, "Couldn't open pcap file: %s\n", argv[1]);
		return -2;
	}

	if (pcap_compile(handle, &fp, filter_exp, 0, 0) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n",
			filter_exp, pcap_geterr(handle));
		return -3;
	}

	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n",
			filter_exp, pcap_geterr(handle));
		return -4;
	}

	pcap_loop(handle, 100, packet_handler, NULL);

	pcap_freecode(&fp);
	pcap_close(handle);

	return EXIT_SUCCESS;
}

void packet_handler(u_char *param, const struct pcap_pkthdr *header,
	const u_char *data)
{
	static int count = 1;
	int i = 0;
	u_int16_t eth_type;

	printf("Packet #%d\n", count);
	printf("Packet size: %u\n", header->len);
	printf("Captured size: %u\n", header->caplen);
	printf("Timestamp: %ld:%ld\n", header->ts.tv_sec, header->ts.tv_usec);
	printf("\n");

	eth_type = eth_handler(param, header, data);

	if (eth_type == ETHERTYPE_IP) {
		printf("Ethernet type: IPv4\n");
	} else if (eth_type == ETHERTYPE_ARP) {
		printf("ARP: ");
	} else if (eth_type == ETHERTYPE_REVARP) {
		printf("RARP: ");
	}

	printf("\n\n");

	count++;
}

u_int16_t eth_handler(u_char *param, const struct pcap_pkthdr *header,
	const u_char *data)
{
	const struct ether_header *eth_header;
	u_short eth_type;
	char *s_mac;
	char *d_mac;

	eth_header = (struct ether_header*)(data);
	eth_type = ntohs(eth_header->ether_type);
	s_mac = get_mac(eth_header->ether_shost);
	d_mac = get_mac(eth_header->ether_dhost);
	printf("\tSrc MAC: %s\n", s_mac);
	printf("\tDst MAC: %s\n", d_mac);

	free(s_mac);
	free(d_mac);

	return eth_type;
}

u_int16_t ipv4_handler(void)
{
	return 0;
}

u_int16_t tcp4_handler(void)
{
	return 0;
}


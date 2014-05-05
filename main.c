#include <stdio.h>
#include <stdlib.h>

#include <netinet/if_ether.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pcap.h>

#include "utils.h"

void packet_handler(u_char *, const struct pcap_pkthdr *, const u_char *);

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
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return -3;
	}

	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return -4;
	}

	pcap_loop(handle, 0, packet_handler, NULL);

	return EXIT_SUCCESS;
}

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *data) 
{
	static int count = 1;
	int i = 0;

	printf("Packet #%d\n", count);
	printf("Packet size %u\n", header->len);
	printf("Captured size %u\n", header->caplen);
	printf("Timestamp %ld:%ld\n", header->ts.tv_sec, header->ts.tv_usec);
	
	const struct ether_header *eth_header;
	eth_header = (struct ether_header*)(data);
	printf("Src MAC "); print_mac(eth_header->ether_shost); printf("\n");
	printf("Dst MAC "); print_mac(eth_header->ether_dhost); printf("\n");

	printf("\n\n");

	count++;
}


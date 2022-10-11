#include <stdio.h>
#include <pcap.h>

void hexdump(const unsigned char *buf, int len)
{
	int i;
	printf("## buf=%p, len=%d\n",buf,len);
	for(i = 0; i < len; i++) {
		if(i && i%16==0)
			printf("\n");
		printf("%02x ", buf[i]);
	}
	printf("\n\n");
}

//
// gcc pcap.c -lpcap
//
int main(int argc, char *argv[])
{
	int i, ret;
	pcap_t *handle;		/* Session handle */
	pcap_if_t *alldevsp;
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	struct bpf_program fp;	/* The compiled filter */
	char filter_exp[] = "port 22";	/* The filter expression */
	bpf_u_int32 mask;	/* Our netmask */
	bpf_u_int32 net;	/* Our IP */
	struct pcap_pkthdr header;	/* The header that pcap gives us */
	const u_char *packet;	/* The actual packet */
	char *dev;		/* The device to sniff on */

	/* Define the device */
	ret = pcap_findalldevs(&alldevsp, errbuf);
	if (ret != 0) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return (2);
	}
	dev = alldevsp->name;

	/* Find the properties for the device */
	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	}
	/* Open the session in promiscuous mode */
	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return (2);
	}
	/* Compile and apply the filter */
	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return (2);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return (2);
	}

	for(i = 0; i < 10; i++) {
		/* Grab a packet */
		packet = pcap_next(handle, &header);
		hexdump(packet, header.len);
		printf("[%d] Jacked a packet with length of [%d]\n", i, header.len);
	}

	/* And close the session */
	pcap_close(handle);
	return (0);
}


#ifdef PACKET_BUILD

#include "packet_public.h"

message_t message; /* current message structure */
mm_t MM; /* memory manager will allocate memory for packets */
int pkt_cnt = 0; /* how many packets have arrived for current message */
int pkt_total = 1; /* how many packets to be received for the message */
int NumMessages = 3; /* number of messages we will receive */

packet_t get_packet () {
	packet_t pkt;
	static int which = 0;

	pkt.how_many = 3;
	pkt.which = which;
	if (which == 0)
	strcpy (pkt.data, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	else if (which == 1)
	strcpy (pkt.data, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	else
	strcpy (pkt.data, "cccccccccccccccccccccccccccccccccccccccccccccccccccccccc");
	which = (which + 1)%3;
	return pkt;
}

void packet_handler(int sig)
{
	packet_t pkt;

	/*  fprintf (stderr, "IN PACKET HANDLER, sig=%d\n", sig); */
	pkt = get_packet();

	/* insert your code here ... stick packet in memory */
}

/* Create message from packets ... deallocate packets */
char *assemble_message () {

	char *MSG;

	/* Allocate MSG and assemble packets into it */

	/* reset these for next message */
	pkt_total = 1;
	pkt_cnt = 0;
	message.num_packets = 0;

	return MSG;
}

int main (int argc, char **argv)
{

	/* init memory manager -- turns out that 64 is the packet size! */

	/* set up alarm handler -- mask all signals within it */

	/* turn on alarm timer ... use  INTERVAL and INTERVAL_USEC for sec and usec values */

	message.num_packets = 0;

	for (i=1; i<=NumMessages; i++) {
		while (pkt_cnt < pkt_total)
		pause(); /* block until next packet */

		MSG = assemble_message ();
		fprintf (stderr, "GOT IT: message=%s\n", MSG);
	}

	/* Deallocate memory manager */
}

#endif //PACKET

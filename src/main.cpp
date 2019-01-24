// Nguyen Dai Son

#include "dnsc.hpp"

int main(int argc, char* argv[])
{
	dnsc dns_client;
	if ( false == dns_client.scan_input(argc, argv)) {
		dns_client.need_help(argv[0]);
		return 1;
	}

	dns_client.make_query();
	dns_client.dump_query();
	dns_client.send_query();

	return 0;
}


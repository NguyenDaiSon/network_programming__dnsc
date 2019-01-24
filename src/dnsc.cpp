// Nguyen Dai Son

#include "dnsc.hpp"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#endif

dnsc::dnsc()
	: m_hid_u16(10)
    	, m_hrd_u16(1)
    	, m_saddr_str("8.8.8.8")
    	, m_qname_str("www.google.com")
	, m_mbuf_ptr(0)
{
}

dnsc::~dnsc()
{
	delete m_mbuf_ptr;
}

bool dnsc::scan_input(int argc, char* argv[])
{
	static struct option arr_options[] =  {
		{ "addr", 	required_argument, 0, 'a' },
		{ "id", 	required_argument, 0, 'i' },
		{ "rd", 	required_argument, 0, 'r' },
		{ "qname", 	required_argument, 0, 'q' },
		{ 0, 0, 0, 0 }
	};

	int cgra(1), opt(-1);
	bool ok( static_cast<int>(sizeof(arr_options)/sizeof(arr_options[0])) == argc);
	while ( ok and -1 != (opt = getopt_long_only( argc, argv, "a:i:r:q:", arr_options, 0 ))) {
		switch (opt)
		{
		case 'a': // addr
			m_saddr_str = optarg;
			++cgra;
			break;

		case 'i': // id
			m_hid_u16 = atoi(optarg);
			++cgra;
			break;

		case 'r': // rd
			m_hrd_u16 = atoi(optarg);
			++cgra;
			break;

		case 'q': // qname
			m_qname_str = optarg;
			++cgra;
			break;

		default:
			ok = false;
		}
	}
	return ok && (cgra == argc);
}

void dnsc::need_help(const char * const& program_name)
{
	const char *mssg = "				  0  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1\n\
				  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                      ID                       |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                    QDCOUNT                    |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                    ANCOUNT                    |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                    NSCOUNT                    |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                    ARCOUNT                    |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                                               |\n\
				/                     QNAME                     /\n\
				/                                               /\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                     QTYPE                     |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n\
				|                     QCLASS                    |\n\
				+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+\n";

	std::cout << "\n"
	          << program_name
			  << ":\n"
			  << "\t---------------------------------------------------------------------------------------------\n"
			  << "\t> This tool is able to send a DNS query with QTYPE A(a host address) to specified DNS server\n"
			  << "\t> This tool is built based on RFC 1035: \"DOMAIN NAMES - IMPLEMENTATION AND SPECIFICATION\"\n"
			  << "\t---------------------------------------------------------------------------------------------\n"
			  << "\t> An example on how to use:\n"
			  << "\t\t> "
			  << program_name
			  << " --addr=8.8.8.8"
			  << " --id=10"
			  << " --rd=1"
			  << " --qname=\"www.google.com\"\n"
			  << "\t\t\t Where:\n"
			  << "\t\t\t --addr : IPv4 DNS Server Address\n"
			  << "\t\t\t --id   : \"identifier\" in the DNS header\n"
			  << "\t\t\t --rd   : \"recursion desired\" iun the DNS header\n"
			  << "\t\t\t --qname: \"domain name\" in the Question Part of DNS message\n"
			  << "\t---------------------------------------------------------------------------------------------\n"
			  << "\t> DNS Query:\n"
			  << mssg
			  << "\t---------------------------------------------------------------------------------------------\n"
			  << "\tNguyen Dai Son"
			  << std::endl;
}

void dnsc::send_query()
{
#	if _WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0) {
		std::cerr << "Error initialising WSA" << std::endl;
		return;
	}
#	endif

	int sock_fd(-1);
	struct sockaddr_in dns_server;
	memset(&dns_server, 0, sizeof(dns_server));

	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return;

	dns_server.sin_family      = AF_INET;
	dns_server.sin_port        = htons(53);
	dns_server.sin_addr.s_addr = inet_addr(m_saddr_str.c_str());

	(void)sendto (sock_fd
				  , (const char*)m_mbuf_ptr->data()
				  , m_mbuf_ptr->length()
				  , 0
				  , (struct sockaddr *)&dns_server
				  , sizeof(dns_server)
				  )
				  ;

	close(sock_fd);
}

void dnsc::dump_query()
{
	m_mbuf_ptr->dump();
}

void dnsc::make_query()
{
	/**: RFC 1035 ***************************************
										1  1  1  1  1  1
		  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                      ID                       |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    QDCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    ANCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    NSCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    ARCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                                               |
		/                     QNAME                     /
		/                                               /
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                     QTYPE                     |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                     QCLASS                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*****************************************************/
	init_mbuf();
	add_id();
	add_flags();
	add_qdcount();
	add_ancount();
	add_nscount();
	add_arcount();
	add_qname();
	add_qtype();
	add_qclass();
}

void dnsc::init_mbuf()
{
	/**: RFC 1035 ***************************************
										1  1  1  1  1  1
		  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                      ID                       |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    QDCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    ANCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    NSCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                    ARCOUNT                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                                               |
		/                     QNAME                     /
		/                                               /
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                     QTYPE                     |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                     QCLASS                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*****************************************************/
	assert(m_mbuf_ptr == 0);
	std::size_t max_size = sizeof (u16) // ID
					+ sizeof (u16) // Flags
					+ sizeof (u16) // QDCOUNT
					+ sizeof (u16) // ANCOUNT
					+ sizeof (u16) // NSCOUNT
					+ sizeof (u16) // ARCOUNT
					+ (m_qname_str.length() + sizeof(u8) + sizeof(u8))
					+ sizeof (u16) // QTYPE
					+ sizeof (u16) // QCLASS
					;
	m_mbuf_ptr = new mbuf(max_size);
	assert(m_mbuf_ptr != 0);
}

void dnsc::add_id()
{
	m_mbuf_ptr->take_ref<u16>() = htons(m_hid_u16);
}

void dnsc::add_flags()
{
	/**: RFC 1035 ***************************************
										1  1  1  1  1  1
		  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*****************************************************/
	assert(m_mbuf_ptr != 0);
	struct
	{
	        u16 RCODE:4;
	        u16 Z:3;
	        u16 RA:1;
	        u16 RD:1;
	        u16 TC:1;
	        u16 AA:1;
	        u16 Opcode:4;
	        u16 QR:1;
	} flag_st;

	flag_st.QR = 0; // Query
	flag_st.Opcode = 0; // Standard query
	flag_st.AA = 0;
	flag_st.TC = 0;
	flag_st.RD = m_hrd_u16;
	flag_st.RA = 0;
	flag_st.Z = 0;
	flag_st.RCODE = 0; // No error

	m_mbuf_ptr->take_ref<u16>() = htons(*reinterpret_cast<u16*>(&flag_st));
}

void dnsc::add_qdcount()
{
	m_mbuf_ptr->take_ref<u16>() = htons(1);
}

void dnsc::add_ancount()
{
	m_mbuf_ptr->take_ref<u16>() = htons(0);
}

void dnsc::add_nscount()
{
	m_mbuf_ptr->take_ref<u16>() = htons(0);
}

void dnsc::add_arcount()
{
	m_mbuf_ptr->take_ref<u16>() = htons(0);
}

void dnsc::add_qname()
{
	assert(not m_qname_str.empty());

	std::size_t found(0);
	std::string qname(m_qname_str);

	for ( ; (found = qname.find_first_of('.')) != std::string::npos; qname = qname.substr(found + 1)) {
		m_mbuf_ptr->take_ref<u8>() = found;
		memcpy(m_mbuf_ptr->take_ptr(found), qname.substr(0, found).c_str(), found);
	}

	m_mbuf_ptr->take_ref<u8>() = found = qname.length();
	memcpy(m_mbuf_ptr->take_ptr(found), qname.c_str(), found);

	m_mbuf_ptr->take_ref<u8>() = '\0';
}

void dnsc::add_qtype()
{
	m_mbuf_ptr->take_ref<u16>() = htons(1); // A
}

void dnsc::add_qclass()
{
	m_mbuf_ptr->take_ref<u16>() = htons(1); // IN
}

// Nguyen Dai Son

#ifndef DNSC_HPP_
#define DNSC_HPP_
#include "mbuf.hpp"
#include <string>
#include <cstring>
#include <cstdio>

class dnsc {
public:
	dnsc();
	~dnsc();
	bool scan_input(int argc, char* argv[]);
	void need_help(const char * const& program_name);
	void make_query();
	void dump_query();
	void send_query();

private:
	dnsc(dnsc const &);
	dnsc& operator=(dnsc const &);

private:
	void init_mbuf();

	// DNS header part
	void add_id();
	void add_flags();
	void add_qdcount();
	void add_ancount();
	void add_nscount();
	void add_arcount();

	// DNS question part
	void add_qname();
	void add_qtype();
	void add_qclass();

private:
	u16 m_hid_u16;
	u16 m_hrd_u16;
	std::string m_saddr_str;
	std::string m_qname_str;
	mbuf* m_mbuf_ptr;
};

#endif /* DNSC_HPP_ */

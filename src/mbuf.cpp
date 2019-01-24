// Nguyen Dai Son

#include "mbuf.hpp"
#include <iostream>
#include <iomanip>

mbuf::mbuf(u16 max_len)
	: m_cur_idx_u16(0)
    , m_max_len_u16(max_len)
    , m_data_u8ptr(0)
{
	assert(max_len != 0 and m_data_u8ptr == 0);
	m_data_u8ptr = new u8[max_len];
	assert(m_data_u8ptr != 0);
	memset(m_data_u8ptr, 0, max_len);
}

mbuf::~mbuf()
{
	delete []m_data_u8ptr;
}

u8* mbuf::take_ptr(u16 byte)
{
	assert( m_cur_idx_u16 + byte <= m_max_len_u16);

	u16 cur_idx_u16(m_cur_idx_u16);
	m_cur_idx_u16 += byte;

	return &m_data_u8ptr[cur_idx_u16];
}

const u8* mbuf::data() const
{
	return m_data_u8ptr;
}

void mbuf::dump() const
{
	assert( m_data_u8ptr != 0 and m_max_len_u16 > 0 );

	u16 base_u16(0);
	u16 step_u16(16);
	u16 index_u16(0);

	while( base_u16 < m_max_len_u16)
	{
		std::cout << "0x"
			  << std::right
			  << std::setfill('0')
			  << std::setw(4)
		          << std::hex
		          << base_u16
		          << ": "
		          ;

		for( index_u16 = 0; index_u16 < step_u16; ++index_u16 )
		{
			std::cout << std::right
					  << std::setfill('0')
					  << std::setw(2)
					  << std::hex
					  << (u16)m_data_u8ptr[base_u16+index_u16]
					  ;
			if( (index_u16 + 1) == step_u16)
				continue;

			if( ((base_u16 + index_u16 + 1) % 2) == 0)
				std::cout << " ";
		}

		std::cout << " [";
		for( index_u16 = 0; index_u16 < step_u16; ++index_u16)
		{
			if( (base_u16 + index_u16) <=  m_max_len_u16)
			{
				u8 c_u8 = m_data_u8ptr[base_u16 + index_u16];
				if( 32 <= c_u8 and c_u8 <= 127 )
					std::cout << char(c_u8);
				else
					std::cout << '.';
			}
			else break;
		}

		std::cout << "]\n";

		base_u16 += step_u16;
	}
}

u16 mbuf::length() const
{
	return m_max_len_u16;
}

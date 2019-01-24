// Nguyen Dai Son

#ifndef MBUF_HPP_
#define MBUF_HPP_

#include <cassert>
#include <cstring>

typedef unsigned char u8;
typedef unsigned short u16;

class mbuf {
public:
	mbuf(u16 max_len);
	~mbuf();

	const u8* data() const;
	u16 length() const;

	u8* take_ptr(u16 byte);
	template <typename T> T& take_ref();

	void dump() const;

private:
	mbuf();
	mbuf(mbuf const&);
	mbuf& operator=(mbuf const&);

private:
	u16 m_cur_idx_u16;
	u16 m_max_len_u16;
	u8 *m_data_u8ptr;
};

template <typename T>
T& mbuf::take_ref()
{
	assert( m_cur_idx_u16 + sizeof(T) <= m_max_len_u16 );

	u16 cur_idx_u16(m_cur_idx_u16);
	m_cur_idx_u16 += sizeof(T);

	return *reinterpret_cast<T*>(&m_data_u8ptr[cur_idx_u16]);
}

#endif /* MBUF_HPP_ */

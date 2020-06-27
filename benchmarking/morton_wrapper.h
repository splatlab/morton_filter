/*
 * ============================================================================
 *
 *       Filename:  ququ_wrapper.h
 *
 *         Author:  Prashant Pandey (), ppandey2@cs.cmu.edu
 *   Organization:  Carnegie Mellon University
 *
 * ============================================================================
 */

#ifndef QUQU_WRAPPER_H
#define QUQU_WRAPPER_H

#include "../morton_filter.h"

using namespace CompressedCuckoo;
Morton3_8 *m_filter;

uint64_t n_bits = 27;

inline int m_init(uint64_t nbits)
{
	uint64_t nslots = (1ULL << nbits);
	m_filter = new Morton3_8(nslots);
	return 0;
}

inline int m_insert(__uint128_t val)
{
	if (!m_filter->insert(val)) {
		return 0;
	}
	return 1;
}

inline int m_lookup(__uint128_t val)
{
	if (!m_filter->likely_contains(val)) {
		return 0;
	}
	return 1;
}

inline int m_remove(__uint128_t val)
{
	if (!m_filter->delete_item(val)) {
		return 0;
	}
	return 1;
}

inline __uint128_t m_range()
{
	return 1ULL << (n_bits + 8);
}

inline int m_destroy()
{
	return 0;
}

#endif

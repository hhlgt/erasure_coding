#ifndef PC_H
#define PC_H
#include "jerasure.h"
#include "reed_sol.h"
#include "cauchy.h"
#include "common.h"
#include "rs.h"

namespace ECProject
{
	void encode_PC(int k1, int m1, int k2, int m2, char **data_ptrs, char **coding_ptrs, int blocksize);
	void decode_PC(int k1, int m1, int k2, int m2, char **data_ptrs, char **coding_ptrs, int blocksize, int *erasures, int failed_num);
	bool check_if_decodable_PC(int k1, int m1, int k2, int m2, int *erasures, int failed_num);
}

#endif
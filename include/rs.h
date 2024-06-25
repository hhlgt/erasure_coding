#ifndef RS_H
#define RS_H
#include "jerasure.h"
#include "reed_sol.h"
#include "cauchy.h"
#include "common.h"

namespace ECProject
{
	void make_encoding_matrix_RS(int k, int m, int *final_matrix);
	void encode_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int blocksize);
	void decode_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int blocksize, int *erasures, int failed_num);
	void encode_partial_blocks_for_encoding_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr);
    void encode_partial_blocks_for_decoding_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);
    bool check_if_decodable_RS(int m, int failed_num);
}

#endif
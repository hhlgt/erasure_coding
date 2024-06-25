#ifndef COMMON_H
#define COMMON_H
#include "jerasure.h"
#include "reed_sol.h"
#include "cauchy.h"
#include "devcommon.h"

namespace ECProject
{
	enum ECTYPE
	{
		RS,
		PC,
		Azu_LRC,
		Azu_LRC_1,
		Opt_LRC,
		Opt_Cau_LRC,
		Uni_Cau_LRC
	};
	void print_matrix(int *matrix, int rows, int cols, std::string msg);
	void get_full_matrix(int *matrix, int k);
	void make_submatrix_by_rows(int cols, int *matrix, int *new_matrix, std::shared_ptr<std::vector<int>> blocks_idx_ptr);
	void make_submatrix_by_cols(int cols, int rows, int *matrix, int *new_matrix, std::shared_ptr<std::vector<int>> blocks_idx_ptr);
	void perform_addition(char **data_ptrs, char **coding_ptrs, int block_size, int block_num, int parity_num);
	void encode_partial_blocks_for_encoding(int k, int m, int *full_matrix, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr);
    void encode_partial_blocks_for_decoding(int k, int m, int *full_matrix, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);
}

#endif
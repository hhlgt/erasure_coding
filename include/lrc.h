#ifndef LRC_H
#define LRC_H
#include "jerasure.h"
#include "reed_sol.h"
#include "cauchy.h"
#include "common.h"

namespace ECProject
{
	enum LRCTYPE
	{
		Azu_LRC,
		Azu_LRC_1,
		Opt_LRC
	};
	bool make_matrix_Azu_LRC(int k, int g, int l, int *final_matirx);
	bool make_matrix_Azu_LRC_1(int k, int g, int l, int *final_matirx);
	bool make_matrix_Opt_LRC(int k, int g, int l, int *final_matirx);
	bool encode_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	bool encode_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	bool encode_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	bool encode_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	bool encode_partial_blocks_for_encoding_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr);
    bool encode_partial_blocks_for_decoding_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);
    bool encode_partial_blocks_for_encoding_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr);
    bool encode_partial_blocks_for_decoding_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);
    bool encode_partial_blocks_for_encoding_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr);
    bool encode_partial_blocks_for_decoding_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);
    bool encode_partial_blocks_for_encoding_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr);
    bool encode_partial_blocks_for_decoding_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);
    bool decode_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num);
    bool decode_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num);
    bool decode_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num);
	bool decode_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num);
}


#endif
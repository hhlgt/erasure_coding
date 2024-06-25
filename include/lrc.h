#ifndef LRC_H
#define LRC_H
#include "jerasure.h"
#include "reed_sol.h"
#include "cauchy.h"
#include "common.h"

namespace ECProject
{
	// full encoding matrix, with demensions as (g + l) × k
	void make_encoding_matrix_Azu_LRC(int k, int g, int l, int *final_matrix);
	void make_encoding_matrix_Azu_LRC_1(int k, int g, int l, int *final_matrix);
	void make_encoding_matrix_Opt_LRC(int k, int g, int l, int *final_matrix);
	void make_encoding_matrix_Opt_Cau_LRC(int k, int g, int l, int *final_matrix);
	void make_encoding_matrix_Opt_Cau_LRC_v2(int k, int g, int l, int *final_matrix);
	void make_encoding_matrix_Uni_Cau_LRC(int k, int g, int l, int *final_matrix);
	void make_encoding_matrix_Uni_Cau_LRC_v2(int k, int g, int l, int *final_matrix);
	void make_encoding_matrix(ECTYPE lrc_type, int k, int g, int l, int *final_matrix);

	// encoding matrix for a single local parity, with demensions as l × group_size
	void make_group_matrix_Azu_LRC(int k, int g, int l, int *group_matrix, int group_id);
	void make_group_matrix_Azu_LRC_1(int k, int g, int l, int *group_matrix, int group_id);
	void make_group_matrix_Opt_LRC(int k, int g, int l, int *group_matrix, int group_id);
	void make_group_matrix_Opt_Cau_LRC(int k, int g, int l, int *group_matrix, int group_id);
	void make_group_matrix_Uni_Cau_LRC(int k, int g, int l, int *group_matrix, int group_id);
	void make_group_matrix_Uni_Cau_LRC(int k, int g, int l, int *group_matrix, int group_id);
	void make_group_matrix(ECTYPE lrc_type, int k, int g, int l, int *group_matrix, int group_id);

	// encode
	void encode_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	void encode_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	void encode_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	void encode_Opt_Cau_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	void encode_Uni_Cau_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	void encode_LRC(ECTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size);
	void decode_LRC(ECTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num);

	// encode partial blocks for encoding global parity block, can be used in global parity recalculation in merging process
	void encode_partial_blocks_for_encoding_LRC_global(ECTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr);
    // global repair: encode partial blocks for decoding when using global parities
    void encode_partial_blocks_for_decoding_LRC_global(ECTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);
    // local repair: encode partial blocks for decoding in a local group
    void encode_partial_blocks_for_decoding_LRC_local(ECTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr);

    // check if decodable
    bool check_if_decodable_Azu_LRC(int k, int g, int l, int *erasures, int failed_num);
    bool check_if_decodable_Azu_LRC_1(int k, int g, int l, int *erasures, int failed_num);
    bool check_if_decodable_Opt_LRC(int k, int g, int l, int *erasures, int failed_num);
    bool check_if_decodable_Opt_Cau_LRC(int k, int g, int l, int *erasures, int failed_num);
    bool check_if_decodable_Uni_Cau_LRC(int k, int g, int l, int *erasures, int failed_num);
    bool check_if_decodable_LRC(ECTYPE lrc_type, int k, int g, int l, int *erasures, int failed_num);
}


#endif
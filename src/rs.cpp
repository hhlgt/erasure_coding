#include <common.h>
#include<rs.h>

bool ECProject::encode_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size)
{
	int *rs_matrix = reed_sol_vandermonde_coding_matrix(k, m, 8);
	jerasure_matrix_encode(k, m, 8, rs_matrix, data_ptrs, coding_ptrs, block_size);
	free(rs_matrix);
	return true;
}

bool ECProject::decode_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num)
{
	if(failed_num > m)
	{
		std::cout << "[Decode] Undecodable!" << std::endl;
		return false;
	}
	int *rs_matrix = reed_sol_vandermonde_coding_matrix(k, m, 8);
	int i = 0;
	i = jerasure_matrix_decode(k, m, 8, rs_matrix, failed_num, erasures, data_ptrs, coding_ptrs, block_size);
	if(i == -1)
	{
		std::cout << "[Decode] Failed!" << std::endl;
	}
	return true;
}

bool ECProject::encode_partial_blocks_for_encoding_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr)
{
	int *rs_matrix = reed_sol_vandermonde_coding_matrix(k, m, 8);
	return encode_partial_blocks_for_encoding(k, m, rs_matrix, data_ptrs, coding_ptrs, block_size, datas_idx_ptr, parities_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_decoding_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr)
{
	int *rs_matrix = reed_sol_vandermonde_coding_matrix(k, m, 8);
	return encode_partial_blocks_for_decoding(k, m, rs_matrix, data_ptrs, coding_ptrs, block_size, sls_idx_ptr, svrs_idx_ptr, fls_idx_ptr);
}

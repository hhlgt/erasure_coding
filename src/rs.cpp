#include <common.h>
#include<rs.h>


void ECProject::make_encoding_matrix_RS(int k, int m, int *final_matrix)
{
	int *matrix = reed_sol_vandermonde_coding_matrix(k, m, 8);
    
    bzero(final_matrix, sizeof(int) * k * m);

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < k; j++)
        {
            final_matrix[i * k + j] = matrix[i * k + j];
        }
    }

    free(matrix);
}

void ECProject::encode_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size)
{
	int *rs_matrix = reed_sol_vandermonde_coding_matrix(k, m, 8);
	jerasure_matrix_encode(k, m, 8, rs_matrix, data_ptrs, coding_ptrs, block_size);
	free(rs_matrix);
}

void ECProject::decode_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num)
{
	if(failed_num > m)
	{
		std::cout << "[Decode] Undecodable!" << std::endl;
		return;
	}
	int *rs_matrix = reed_sol_vandermonde_coding_matrix(k, m, 8);
	int i = 0;
	i = jerasure_matrix_decode(k, m, 8, rs_matrix, failed_num, erasures, data_ptrs, coding_ptrs, block_size);
	if(i == -1)
	{
		std::cout << "[Decode] Failed!" << std::endl;
		return;
	}
}

void ECProject::encode_partial_blocks_for_encoding_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr)
{
	std::vector<int> rs_matrix((k + m) * k, 0);
    get_full_matrix(rs_matrix.data(), k);
    make_encoding_matrix_RS(k, m, &(rs_matrix.data())[k * k]);
	encode_partial_blocks_for_encoding(k, m, rs_matrix.data(), data_ptrs, coding_ptrs, block_size, datas_idx_ptr, parities_idx_ptr);
}

void ECProject::encode_partial_blocks_for_decoding_RS(int k, int m, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr)
{
	std::vector<int> rs_matrix((k + m) * k, 0);
    get_full_matrix(rs_matrix.data(), k);
    make_encoding_matrix_RS(k, m, &(rs_matrix.data())[k * k]);
	encode_partial_blocks_for_decoding(k, m, rs_matrix.data(), data_ptrs, coding_ptrs, block_size, sls_idx_ptr, svrs_idx_ptr, fls_idx_ptr);
}


bool ECProject::check_if_decodable_RS(int m, int failed_num)
{
	if(m >= failed_num)
	{
		return true;
	}
	else
	{
		return false;
	}
}
#include <lrc.h>
#include <common.h>

bool ECProject::make_matrix_Azu_LRC(int k, int g, int l, int *final_matrix)
{
	int r = (k + l - 1) / l;
    int *matrix = reed_sol_vandermonde_coding_matrix(k, g, 8);
    
    bzero(final_matrix, sizeof(int) * k * (g + l));

    for(int i = 0; i < g; i++)
    {
        for(int j = 0; j < k; j++)
        {
            final_matrix[i * k + j] = matrix[i * k + j];
        }
    }

    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < k; j++)
        {
            if(i * r <= j && j < (i + 1) * r)
            {
                final_matrix[(i + g) * k + j] = 1;
            }
        }
    }

    free(matrix);
    return true;
}

bool ECProject::make_matrix_Azu_LRC_1(int k, int g, int l, int *final_matrix)
{
    int r = (k + l - 1) / l;
    int *matrix = reed_sol_vandermonde_coding_matrix(k, g, 8);
    
    bzero(final_matrix, sizeof(int) * k * (g + l));

    for(int i = 0; i < g; i++)
    {
        for(int j = 0; j < k; j++)
        {
            final_matrix[i * k + j] = matrix[i * k + j];
        }
    }

    std::vector<int> l_matrix(l * (k + g), 0);
    std::vector<int> d_g_matrix((k + g) * k, 0);
    int idx = 0;
    for(int i = 0; i < l - 1; i++)
    {
        int group_size = std::min(r, k - i * r);
        for(int j = 0; j < group_size; j++)
        {
            l_matrix[i * (k + g) + idx] = 1;
            idx++;
        }
    }
    for(int j = 0; j < g; j++)
    {
        l_matrix[(l - 1) * (k + g) + idx] = 1;
        idx++;
    }
    for(int i = 0; i < k; i++)
    {
        d_g_matrix[i * k + i] = 1;
    }
    idx = k * k;
    for(int i = 0; i < g; i++)
    {
        for(int j = 0; j < k; j++)
        {
            d_g_matrix[idx + i * k + j] = matrix[i * k + j];
        }
    }

    int *mix_matrix = jerasure_matrix_multiply(l_matrix.data(), d_g_matrix.data(), l, k + g, k + g, k, 8);

    idx = g * k;
    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < k; j++)
        {
            final_matrix[idx + i * k + j] = mix_matrix[i * k + j];
        }
    }

    free(matrix);
    free(mix_matrix);
    return true;
}

bool ECProject::make_matrix_Opt_LRC(int k, int g, int l, int *final_matrix)
{
    int r = (k + g + l - 1) / l;
    int *matrix = reed_sol_vandermonde_coding_matrix(k, g, 8);
    
    bzero(final_matrix, sizeof(int) * k * (g + l));

    for(int i = 0; i < g; i++)
    {
        for(int j = 0; j < k; j++)
        {
            final_matrix[i * k + j] = matrix[i * k + j];
        }
    }

    std::vector<int> l_matrix(l * (k + g), 0);
    std::vector<int> d_g_matrix((k + g) * k, 0);
    int idx = 0;
    for(int i = 0; i < l; i++)
    {
        int group_size = std::min(r, k + g - i * r);
        for(int j = 0; j < group_size; j++)
        {
            l_matrix[i * (k + g) + idx] = 1;
            idx++;
        }
    }
    for(int i = 0; i < k; i++)
    {
        d_g_matrix[i * k + i] = 1;
    }
    idx = k * k;
    for(int i = 0; i < g; i++)
    {
        for(int j = 0; j < k; j++)
        {
            d_g_matrix[idx + i * k + j] = matrix[i * k + j];
        }
    }

    // print_matrix(l_matrix.data(), l, k + g);
    // print_matrix(d_g_matrix.data(), k + g, k);

    int *mix_matrix = jerasure_matrix_multiply(l_matrix.data(), d_g_matrix.data(), l, k + g, k + g, k, 8);

    idx = g * k;
    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < k; j++)
        {
            final_matrix[idx + i * k + j] = mix_matrix[i * k + j];
        }
    }

    free(matrix);
    free(mix_matrix);
    return true;
}

bool ECProject::encode_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size)
{
	std::vector<int> lrc_matrix((g + l) * k, 0);
	make_matrix_Azu_LRC(k, g, l, lrc_matrix.data());
	jerasure_matrix_encode(k, g + l, 8, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size);
    return true;
}

bool ECProject::encode_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size)
{
	// std::vector<int> lrc_matrix((g + l - 1) * k, 0);
	// make_matrix_Azu_LRC(k, g, l - 1, lrc_matrix.data());
	// jerasure_matrix_encode(k, g + l - 1, 8, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size);
	// std::vector<int> new_matrix(g, 1);
	// jerasure_matrix_encode(g, 1, 8, new_matrix.data(), coding_ptrs, &coding_ptrs[g + l - 1], block_size);
    std::vector<int> lrc_matrix((g + l) * k, 0);
    make_matrix_Azu_LRC_1(k, g, l, lrc_matrix.data());
    jerasure_matrix_encode(k, g + l, 8, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size);
    return true;
}

bool ECProject::encode_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size)
{
    // int *matrix = reed_sol_vandermonde_coding_matrix(k, g, 8);
    // free(matrix);
    // std::vector<int> group_number(l, 0);
    // int r = (k + g + l - 1) / l;
    // for(int i = 0; i < l; i++)
    // {
    //     int group_size = std::min(r, k + g - i * r);
    //     std::vector<char *> vector_number(group_size);
    //     char **new_data = (char **)vector_number.data();
    //     for(int j = 0; j < group_size; j++)
    //     {
    //         if(i * g + j < k)
    //         {
    //             new_data[j] = data_ptrs[i * g + j];
    //         }
    //         else
    //         {
    //             new_data[j] = coding_ptrs[i * g + j - k];
    //         }
    //     }
    //     std::vector<int> new_matrix(group_size, 1);
    //     jerasure_matrix_encode(group_size, 1, 8, new_matrix.data(), new_data, &coding_ptrs[g + i], block_size);
    // }
    std::vector<int> lrc_matrix((g + l) * k, 0);
    make_matrix_Opt_LRC(k, g, l, lrc_matrix.data());
    jerasure_matrix_encode(k, g + l, 8, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size);
    return true;
}

bool ECProject::encode_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size)
{
    std::vector<int> lrc_matrix((g + l) * k, 0);
    if(lrc_type == Azu_LRC)
    {
        make_matrix_Azu_LRC(k, g, l, lrc_matrix.data());
    }
    else if(lrc_type == Azu_LRC_1)
    {
        make_matrix_Azu_LRC_1(k, g, l, lrc_matrix.data());
    }
    else if(lrc_type == Opt_LRC)
    {
        make_matrix_Opt_LRC(k, g, l, lrc_matrix.data());
    }
    print_matrix(lrc_matrix.data(), g + l, k);
    jerasure_matrix_encode(k, g + l, 8, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size);
    return true;
}
	

bool ECProject::encode_partial_blocks_for_encoding_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr)
{
	std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
	make_matrix_Azu_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
	return encode_partial_blocks_for_encoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, datas_idx_ptr, parities_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_decoding_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr)
{
	std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
    make_matrix_Azu_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
	return encode_partial_blocks_for_decoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, sls_idx_ptr, svrs_idx_ptr, fls_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_encoding_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr)
{
    std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
    make_matrix_Azu_LRC_1(k, g, l, &(lrc_matrix.data())[k * k]);
    return encode_partial_blocks_for_encoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, datas_idx_ptr, parities_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_decoding_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr)
{
    std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
    make_matrix_Azu_LRC_1(k, g, l, &(lrc_matrix.data())[k * k]);
    return encode_partial_blocks_for_decoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, sls_idx_ptr, svrs_idx_ptr, fls_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_encoding_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr)
{
    std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
    make_matrix_Opt_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
    return encode_partial_blocks_for_encoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, datas_idx_ptr, parities_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_decoding_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr)
{
    std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
    make_matrix_Opt_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
    return encode_partial_blocks_for_decoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, sls_idx_ptr, svrs_idx_ptr, fls_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_encoding_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> datas_idx_ptr, std::shared_ptr<std::vector<int>> parities_idx_ptr)
{
    std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
    if(lrc_type == Azu_LRC)
    {
        make_matrix_Azu_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
    }
    else if(lrc_type == Azu_LRC_1)
    {
        make_matrix_Azu_LRC_1(k, g, l, &(lrc_matrix.data())[k * k]);
    }
    else if(lrc_type == Opt_LRC)
    {
        make_matrix_Opt_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
    }
    return encode_partial_blocks_for_encoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, datas_idx_ptr, parities_idx_ptr);
}

bool ECProject::encode_partial_blocks_for_decoding_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, std::shared_ptr<std::vector<int>> sls_idx_ptr, std::shared_ptr<std::vector<int>> svrs_idx_ptr, std::shared_ptr<std::vector<int>> fls_idx_ptr)
{
    std::vector<int> lrc_matrix((k + g + l) * k, 0);
    get_full_matrix(lrc_matrix.data(), k);
    if(lrc_type == Azu_LRC)
    {
        make_matrix_Azu_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
    }
    else if(lrc_type == Azu_LRC_1)
    {
        make_matrix_Azu_LRC_1(k, g, l, &(lrc_matrix.data())[k * k]);
    }
    else if(lrc_type == Opt_LRC)
    {
        make_matrix_Opt_LRC(k, g, l, &(lrc_matrix.data())[k * k]);
    }
    return encode_partial_blocks_for_decoding(k, g + l, lrc_matrix.data(), data_ptrs, coding_ptrs, block_size, sls_idx_ptr, svrs_idx_ptr, fls_idx_ptr);
}

bool ECProject::decode_Azu_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num)
{
    std::vector<int> lrc_matrix((g + l) * k, 0);
    make_matrix_Azu_LRC(k, g, l, lrc_matrix.data());
    int i = 0;
    i = jerasure_matrix_decode(k, g + l, 8, lrc_matrix.data(), failed_num, erasures, data_ptrs, coding_ptrs, block_size);
    if(i == -1)
    {
        std::cout << "[Decode] Failed!" << std::endl;
        return false;
    }
    return true;
}

bool ECProject::decode_Azu_LRC_1(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num)
{
    std::vector<int> lrc_matrix((g + l) * k, 0);
    make_matrix_Azu_LRC_1(k, g, l, lrc_matrix.data());
    int i = 0;
    i = jerasure_matrix_decode(k, g + l, 8, lrc_matrix.data(), failed_num, erasures, data_ptrs, coding_ptrs, block_size);
    if(i == -1)
    {
        std::cout << "[Decode] Failed!" << std::endl;
        return false;
    }
    return true;
}

bool ECProject::decode_Opt_LRC(int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num)
{
    std::vector<int> lrc_matrix((g + l) * k, 0);
    make_matrix_Opt_LRC(k, g, l, lrc_matrix.data());
    int i = 0;
    i = jerasure_matrix_decode(k, g + l, 8, lrc_matrix.data(), failed_num, erasures, data_ptrs, coding_ptrs, block_size);
    if(i == -1)
    {
        std::cout << "[Decode] Failed!" << std::endl;
        return false;
    }
    return true;
}

bool ECProject::decode_LRC(LRCTYPE lrc_type, int k, int g, int l, char **data_ptrs, char **coding_ptrs, int block_size, int *erasures, int failed_num)
{
    std::vector<int> lrc_matrix((g + l) * k, 0);
    if(lrc_type == Azu_LRC)
    {
        make_matrix_Azu_LRC(k, g, l, lrc_matrix.data());
    }
    else if(lrc_type == Azu_LRC_1)
    {
        make_matrix_Azu_LRC_1(k, g, l, lrc_matrix.data());
    }
    else if(lrc_type == Opt_LRC)
    {
        make_matrix_Opt_LRC(k, g, l, lrc_matrix.data());
    }
    int i = 0;
    i = jerasure_matrix_decode(k, g + l, 8, lrc_matrix.data(), failed_num, erasures, data_ptrs, coding_ptrs, block_size);
    if(i == -1)
    {
        std::cout << "[Decode] Failed!" << std::endl;
        return false;
    }
    return true;
}


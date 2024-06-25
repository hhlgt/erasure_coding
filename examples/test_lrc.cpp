#include <lrc.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

void test_decodability(int k, int g, int l, ECProject::ECTYPE encode_type);
void count_decodability_in_all_f_failures(int k, int g, int l, ECProject::ECTYPE encode_type, int f);
void test_decoding(int k, int g, int l, int block_size, ECProject::ECTYPE encode_type, std::string targetdir);
void test_partial_decoding_global(int k, int g, int l, int block_size, ECProject::ECTYPE encode_type, std::string targetdir);
void test_partial_decoding_local(int k, int g, int l, int block_size, ECProject::ECTYPE encode_type, std::string targetdir);

int main(int argc, char const *argv[])
{
	char buff[256];
    getcwd(buff, 256);
    std::string cwf = std::string(argv[0]);

    int k = 4;
    int l = 2;
    int g = 2;
    int objects_num = 1; // <10
    int block_size = 256;
    int value_length = 1;
    ECProject::ECTYPE encode_type = ECProject::Opt_Cau_LRC;

    int parity_blocks_number = g + l;
    std::vector<char *> v_data(k);
    std::vector<char *> v_coding(parity_blocks_number);
    char **data = (char **)v_data.data();
    char **coding = (char **)v_coding.data();
    std::vector<std::vector<char>> coding_area(parity_blocks_number, std::vector<char>(block_size));

    std::string readdir = std::string(buff) + cwf.substr(1, cwf.rfind('/') - 1) + "/../data/";
    std::string targetdir = std::string(buff) + cwf.substr(1, cwf.rfind('/') - 1) + "/../storage/";
    if (access(targetdir.c_str(), 0) == -1)
    {
        mkdir(targetdir.c_str(), S_IRWXU);
    }

    char *value = new char[value_length * 1024];

    // Storage
    for(int j = 0; j < objects_num; j++)
    {
        std::string key = "Object" + std::to_string(j);
        std::string readpath = readdir + key;
        
        std::ifstream ifs(readpath);
        ifs.read(value, value_length * 1024);
        ifs.close();
        char *p_value = const_cast<char *>(value);
        for (int i = 0; i < k; i++)
        {
            data[i] = p_value + i * block_size;
        }
        for (int i = 0; i < parity_blocks_number; i++)
        {
            coding[i] = coding_area[i].data();
        }
        ECProject::encode_LRC(encode_type, k, g, l, data, coding, block_size);
        std::cout << "finish encoding!" << std::endl;
        for(int i = 0; i < k + g + l; i++)
        {
            std::string block_key;
            block_key = key + "_" + std::to_string(i);
	        std::string writepath = targetdir + block_key;
	        std::ofstream ofs(writepath, std::ios::binary | std::ios::out | std::ios::trunc);
	        if(i < k)
	        {
	          ofs.write(data[i], block_size);
              std::cout << "Block " << i << ": " << data[i] << std::endl;
	        }
	        else
	        {
	          ofs.write(coding[i - k], block_size);
              std::cout << "Block " << i << ": " << coding[i - k] << std::endl;
	        }
	        ofs.flush();
	        ofs.close();
        }
    }

    test_partial_decoding_local(k, g, l, block_size, encode_type, targetdir);

    delete value;
	return 0;
}

void test_decodability(int k, int g, int l, ECProject::ECTYPE encode_type)
{
    int erasures[4] = {0, 2, 4, -1};
    int failed_num = 3;
    bool flag = ECProject::check_if_decodable_LRC(encode_type, k, g, l, erasures, failed_num);
    if(flag)
    {
        std::cout << "[^-^] Decodable!" << std::endl;
    }
    else
    {
        std::cout << "[T_T] Undecodable!" << std::endl;
    }
}

// enumerate all f failures
void count_decodability_in_all_f_failures(int k, int g, int l, ECProject::ECTYPE encode_type, int f)
{
    int n = k + g + l;
    std::vector<int> nums(n, 0);
    for(int i = 0; i < n; i++)
    {
        nums[i] = i;
    }
    std::vector<bool> bitmask(n, false);
    for (int i = 0; i < f; ++i) {
        bitmask[i] = true;
    }
    int tot_cnt = 0;
    int f_cnt = 0;
    int *erasures = new int[f + 1];
    erasures[f] = -1;
    do {
        
        for (int i = 0; i < n; ++i) {
            if (bitmask[i]) {
                erasures[i] = nums[i];
                
            }
        }
        bool flag = ECProject::check_if_decodable_LRC(encode_type, k, g, l, erasures, f);
        if(!flag)
        {
            f_cnt += 1;
        }
        tot_cnt += 1;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    std::cout << "Result: [Total]" << tot_cnt << ", [Failed]" << f_cnt << std::endl;
    delete erasures;
}

void test_decoding(int k, int g, int l, int block_size, ECProject::ECTYPE encode_type, std::string targetdir)
{
    int n = k + l + g;
    auto svrs_idx_ptr = std::make_shared<std::vector<int>>();
    auto fls_idx_ptr = std::make_shared<std::vector<int>>();
    svrs_idx_ptr->push_back(0);
    svrs_idx_ptr->push_back(2);
    svrs_idx_ptr->push_back(4);
    svrs_idx_ptr->push_back(5);
    svrs_idx_ptr->push_back(6);
    svrs_idx_ptr->push_back(7);
    fls_idx_ptr->push_back(1);
    fls_idx_ptr->push_back(3);

    int failed_num = int(fls_idx_ptr->size());

    // read data
    std::vector<char *> stripe_data(n);
    char **stripe_blocks = (char **)stripe_data.data();
    std::vector<std::vector<char>> stripe_data_area(n, std::vector<char>(block_size));
    for(int i = 0; i < n; i++)
    {
        stripe_blocks[i] = stripe_data_area[i].data();
    }
    for(auto it = svrs_idx_ptr->begin(); it != svrs_idx_ptr->end(); it++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        std::ifstream ifs(readpath);
        ifs.read(stripe_blocks[idx], block_size);
        ifs.close();
    }
    for(auto it = fls_idx_ptr->begin(); it != fls_idx_ptr->end(); it++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        std::ifstream ifs(readpath);
        ifs.read(stripe_blocks[idx], block_size);
        ifs.close();
    }

    int *erasures = new int[failed_num + 1];
    int i = 0;
    for(auto it = fls_idx_ptr->begin(); it != fls_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        erasures[i] = idx;
    }
    erasures[failed_num] = -1;
    printf("\n--- Repaired directly ---\n");
    std::vector<char *> r_data(k);
    char **data_ptrs = (char **)r_data.data();
    std::vector<char *> r_coding(g + l);
    char **coding_ptrs = (char **)r_coding.data();
    for(int i = 0; i < k + g + l; i++)
    {
        if(i < k)
        {
            data_ptrs[i] = stripe_blocks[i];
        }
        else
        {
            coding_ptrs[i - k] = stripe_blocks[i];
        }
        
        printf("Block %d : %s\n", i, stripe_blocks[i]);
    }
    for(int i = 0; i < failed_num; i++)
    {
        printf("Block %d before lost:\n%s\n", erasures[i], stripe_blocks[erasures[i]]);
        memset(stripe_blocks[erasures[i]], 0, block_size);
        printf("Block %d after lost:\n%s\n", erasures[i], stripe_blocks[erasures[i]]);
    }

    ECProject::decode_LRC(encode_type, k, g, l, data_ptrs, coding_ptrs, block_size, erasures, failed_num);

    for(int i = 0; i < failed_num; i++)
    {
        printf("Block %d repaired:\n%s\n", erasures[i], stripe_blocks[erasures[i]]);
    }
}

void test_partial_decoding_global(int k, int g, int l, int block_size, ECProject::ECTYPE encode_type, std::string targetdir)
{
    int n = k + l + g;
    auto svrs_idx_ptr = std::make_shared<std::vector<int>>();
    auto sls1_idx_ptr = std::make_shared<std::vector<int>>();
    auto sls2_idx_ptr = std::make_shared<std::vector<int>>();
    auto fls_idx_ptr = std::make_shared<std::vector<int>>();
    svrs_idx_ptr->push_back(0);
    svrs_idx_ptr->push_back(2);
    svrs_idx_ptr->push_back(4);
    svrs_idx_ptr->push_back(5);
    svrs_idx_ptr->push_back(6);
    svrs_idx_ptr->push_back(7);
    fls_idx_ptr->push_back(3);
    fls_idx_ptr->push_back(1);
    fls_idx_ptr->push_back(4);
    fls_idx_ptr->push_back(5);
    sls1_idx_ptr->push_back(2);
    sls1_idx_ptr->push_back(7);
    sls1_idx_ptr->push_back(6);
    sls1_idx_ptr->push_back(5);
    sls2_idx_ptr->push_back(4);
    sls2_idx_ptr->push_back(0);

    int failed_num = int(fls_idx_ptr->size()), partial_num = 2;
    int partial_blocks_num = failed_num * partial_num;
    std::vector<char *> partial_coding(partial_blocks_num);
    char **partial_blocks = (char **)partial_coding.data();
    std::vector<std::vector<char>> partial_coding_area(partial_blocks_num, std::vector<char>(block_size));
    for(int i = 0; i < partial_blocks_num; i++)
    {
        partial_blocks[i] = partial_coding_area[i].data();
    }
    std::vector<char *> stripe_data(k + g + l);
    char **stripe_blocks = (char **)stripe_data.data();

    // partial 1
    std::vector<char *> sls1_data(4);
    char **sls1 = (char **)sls1_data.data();
    std::vector<std::vector<char>> sls1_data_area(4, std::vector<char>(block_size));
    int i = 0;
    for(auto it = sls1_idx_ptr->begin(); it != sls1_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        sls1[i] = sls1_data_area[i].data();
        stripe_blocks[idx] = sls1[i];
        std::ifstream ifs(readpath);
        ifs.read(sls1[i], block_size);
        ifs.close();
    }
    printf("Partial 1: encoding partial blocks!\n");
    ECProject::encode_partial_blocks_for_decoding_LRC_global(encode_type, k, g, l, sls1, partial_blocks, block_size, sls1_idx_ptr, svrs_idx_ptr, fls_idx_ptr);

    // partial 2
    std::vector<char *> sls2_data(4);
    char **sls2 = (char **)sls2_data.data();
    std::vector<std::vector<char>> sls2_data_area(4, std::vector<char>(block_size));
    i = 0;
    for(auto it = sls2_idx_ptr->begin(); it != sls2_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        sls2[i] = sls2_data_area[i].data();
        stripe_blocks[idx] = sls2[i];
        std::ifstream ifs(readpath);
        ifs.read(sls2[i], block_size);
        ifs.close();
    }
    printf("Partial 2: encoding partial blocks!\n");
    ECProject::encode_partial_blocks_for_decoding_LRC_global(encode_type, k, g, l, sls2, &partial_blocks[failed_num], block_size, sls2_idx_ptr, svrs_idx_ptr, fls_idx_ptr);

    // failed part
    std::vector<char *> failed_data(failed_num);
    char **failed_blocks = (char **)failed_data.data();
    std::vector<std::vector<char>> failed_data_area(failed_num, std::vector<char>(block_size));
    i = 0;
    for(auto it = fls_idx_ptr->begin(); it != fls_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        failed_blocks[i] = failed_data_area[i].data();
        stripe_blocks[idx] = failed_blocks[i];
        std::ifstream ifs(readpath);
        ifs.read(failed_blocks[i], block_size);
        ifs.close();
    }

    std::vector<char *> repaired_data(failed_num);
    char **repaired_blocks = (char **)repaired_data.data();
    std::vector<std::vector<char>> repaired_data_area(failed_num, std::vector<char>(block_size));
    for(int j = 0; j < failed_num; j++)
    {
        repaired_blocks[j] = repaired_data_area[j].data();
    }
    printf("Decoding\n");
    ECProject::perform_addition(partial_blocks, repaired_blocks, block_size, partial_blocks_num, failed_num);

    printf("\n--- Applying encode-and-transfer ---\n");
    i = 0;
    for(auto it = fls_idx_ptr->begin(); it != fls_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        printf("\nLost block %d:\n", idx);
        printf("Before lost: %s\n", failed_blocks[i]);
        printf("After repair: %s\n", repaired_blocks[i]);
    }
}

void test_partial_decoding_local(int k, int g, int l, int block_size, ECProject::ECTYPE encode_type, std::string targetdir)
{
    int n = k + l + g;
    auto svrs_idx_ptr = std::make_shared<std::vector<int>>();
    auto sls1_idx_ptr = std::make_shared<std::vector<int>>();
    auto sls2_idx_ptr = std::make_shared<std::vector<int>>();
    auto fls_idx_ptr = std::make_shared<std::vector<int>>();
    svrs_idx_ptr->push_back(2);
    svrs_idx_ptr->push_back(5);
    svrs_idx_ptr->push_back(4);
    svrs_idx_ptr->push_back(7);
    sls1_idx_ptr->push_back(2);
    sls1_idx_ptr->push_back(5);
    sls2_idx_ptr->push_back(4);
    sls2_idx_ptr->push_back(7);
    fls_idx_ptr->push_back(3);

    int failed_num = 1, partial_num = 2;
    int partial_blocks_num = failed_num * partial_num;
    std::vector<char *> partial_coding(partial_blocks_num);
    char **partial_blocks = (char **)partial_coding.data();
    std::vector<std::vector<char>> partial_coding_area(partial_blocks_num, std::vector<char>(block_size));
    for(int i = 0; i < partial_blocks_num; i++)
    {
        partial_blocks[i] = partial_coding_area[i].data();
    }

    // partial 1
    std::vector<char *> sls1_data(4);
    char **sls1 = (char **)sls1_data.data();
    std::vector<std::vector<char>> sls1_data_area(4, std::vector<char>(block_size));
    int i = 0;
    for(auto it = sls1_idx_ptr->begin(); it != sls1_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        sls1[i] = sls1_data_area[i].data();
        std::ifstream ifs(readpath);
        ifs.read(sls1[i], block_size);
        ifs.close();
    }
    printf("Partial 1: encoding partial blocks!\n");
    ECProject::encode_partial_blocks_for_decoding_LRC_local(encode_type, k, g, l, sls1, partial_blocks, block_size, sls1_idx_ptr, svrs_idx_ptr, fls_idx_ptr);
    
    // partial 2
    std::vector<char *> sls2_data(4);
    char **sls2 = (char **)sls2_data.data();
    std::vector<std::vector<char>> sls2_data_area(4, std::vector<char>(block_size));
    i = 0;
    for(auto it = sls2_idx_ptr->begin(); it != sls2_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        sls2[i] = sls2_data_area[i].data();
        std::ifstream ifs(readpath);
        ifs.read(sls2[i], block_size);
        ifs.close();
    }
    printf("Partial 2: encoding partial blocks!\n");
    ECProject::encode_partial_blocks_for_decoding_LRC_local(encode_type, k, g, l, sls2, &partial_blocks[1], block_size, sls2_idx_ptr, svrs_idx_ptr, fls_idx_ptr);

    // failed part
    std::vector<char *> failed_data(failed_num);
    char **failed_blocks = (char **)failed_data.data();
    std::vector<std::vector<char>> failed_data_area(failed_num, std::vector<char>(block_size));
    i = 0;
    for(auto it = fls_idx_ptr->begin(); it != fls_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        std::string key;
        key = "Object" + std::to_string(idx/n) + "_" + std::to_string(idx);
        std::string readpath = targetdir + key;
        failed_blocks[i] = failed_data_area[i].data();
        std::ifstream ifs(readpath);
        ifs.read(failed_blocks[i], block_size);
        ifs.close();
    }

    // std::vector<char *> groups(3);
    // char **local_group = (char **)groups.data();
    // std::vector<char *> parity(1);
    // char **local_parity = (char **)parity.data();
    // std::vector<std::vector<char>> parity_area(1, std::vector<char>(block_size));
    // local_group[0] = failed_blocks[0];
    // local_group[1] = sls1[0];
    // local_group[2] = sls2[0];
    // local_parity[0] = parity_area[0].data();
    // std::vector<int> local_matrix(3, 0);
    // ECProject::group_matrix_Uni_Cau_LRC(k, g, l, local_matrix.data(), 1);
    // ECProject::print_matrix(local_matrix.data(), 1, 3, "group_matrix");
    // jerasure_matrix_encode(3, 1, 8, local_matrix.data(), local_group, local_parity, block_size);
    // std::cout << "Block 7: " << local_parity[0] << std::endl;


    std::vector<char *> repaired_data(failed_num);
    char **repaired_blocks = (char **)repaired_data.data();
    std::vector<std::vector<char>> repaired_data_area(failed_num, std::vector<char>(block_size));
    for(int j = 0; j < failed_num; j++)
    {
        repaired_blocks[j] = repaired_data_area[j].data();
    }
    printf("Decoding\n");
    ECProject::perform_addition(partial_blocks, repaired_blocks, block_size, partial_blocks_num, failed_num);

    printf("\n--- Applying encode-and-transfer ---\n");
    i = 0;
    for(auto it = fls_idx_ptr->begin(); it != fls_idx_ptr->end(); it++, i++)
    {
        int idx = *it;
        printf("\nLost block %d:\n", idx);
        printf("Before lost: %s\n", failed_blocks[i]);
        printf("After repair: %s\n", repaired_blocks[i]);
    }
}
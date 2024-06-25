#include <pc.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

void test_decodability(int k1, int m1, int k2, int m2);
void count_decodability_in_all_f_failures(int k1, int m1, int k2, int m2, int f);
void test_decoding(int k1, int m1, int k2, int m2, int block_size, std::string targetdir);

int main(int argc, char const *argv[])
{
	char buff[256];
    getcwd(buff, 256);
    std::string cwf = std::string(argv[0]);

    int k1 = 4;
    int m1 = 2;
    int k2 = 2;
    int m2 = 1;
    int k = k1 * k2;
    int objects_num = 1; // <10
    int block_size = 128;
    int value_length = 1;
    // ECProject::ECTYPE encode_type = ECProject::PC;

    int parity_blocks_number = (k1 + m1) * (k2 + m2) - k1 * k2;
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
        ECProject::encode_PC(k1, m1, k2, m2, data, coding, block_size);
        std::cout << "finish encoding!" << std::endl;
        for(int i = 0; i < (k1 + m1) * (k2 + m2); i++)
        {
            std::string block_key;
            block_key = key + "_" + std::to_string(i);
	        std::string writepath = targetdir + block_key;
	        std::ofstream ofs(writepath, std::ios::binary | std::ios::out | std::ios::trunc);
	        if(i < k)
	        {
	          ofs.write(data[i], block_size);
              // std::cout << "Block " << i << ": " << data[i] << std::endl;
	        }
	        else
	        {
	          ofs.write(coding[i - k], block_size);
              // std::cout << "Block " << i << ": " << coding[i - k] << std::endl;
	        }
	        ofs.flush();
	        ofs.close();
        }
    }

    test_decoding(k1, m1, k2, m2, block_size, targetdir);

    delete value;
	return 0;
}

void test_decodability(int k1, int m1, int k2, int m2)
{
    int erasures[4] = {0, 2, 4, -1};
    int failed_num = 3;
    bool flag = ECProject::check_if_decodable_PC(k1, m1, k2, m2, erasures, failed_num);
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
void count_decodability_in_all_f_failures(int k1, int m1, int k2, int m2, int f)
{
    int n = (k1 + m1) * (k2 + m2);
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
        bool flag = ECProject::check_if_decodable_PC(k1, m1, k2, m2, erasures, f);
        if(!flag)
        {
            f_cnt += 1;
        }
        tot_cnt += 1;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    std::cout << "Result: [Total]" << tot_cnt << ", [Failed]" << f_cnt << std::endl;
    delete erasures;
}

void test_decoding(int k1, int m1, int k2, int m2, int block_size, std::string targetdir)
{
	int n = (k1 + m1) * (k2 + m2);
    auto svrs_idx_ptr = std::make_shared<std::vector<int>>();
    auto fls_idx_ptr = std::make_shared<std::vector<int>>();
    svrs_idx_ptr->push_back(0);
    svrs_idx_ptr->push_back(2);
    svrs_idx_ptr->push_back(4);
    svrs_idx_ptr->push_back(11);
    svrs_idx_ptr->push_back(8);
    svrs_idx_ptr->push_back(7);
    svrs_idx_ptr->push_back(10);
    svrs_idx_ptr->push_back(12);
    svrs_idx_ptr->push_back(14);
    svrs_idx_ptr->push_back(15);
    svrs_idx_ptr->push_back(9);
    svrs_idx_ptr->push_back(13);
    fls_idx_ptr->push_back(1);
    fls_idx_ptr->push_back(3);
    fls_idx_ptr->push_back(5);
    fls_idx_ptr->push_back(6);

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
    int k = k1 * k2;
    int m = n - k;
    std::vector<char *> r_data(k);
    char **data_ptrs = (char **)r_data.data();
    std::vector<char *> r_coding(m);
    char **coding_ptrs = (char **)r_coding.data();
    for(int i = 0; i < n; i++)
    {
        if(i < k)
        {
            data_ptrs[i] = stripe_blocks[i];
        }
        else
        {
            coding_ptrs[i - k] = stripe_blocks[i];
        }
    }
    for(int i = 0; i < failed_num; i++)
    {
        printf("Block %d before lost:\n%s\n", erasures[i], stripe_blocks[erasures[i]]);
        memset(stripe_blocks[erasures[i]], 0, block_size);
        printf("Block %d after lost:\n%s\n", erasures[i], stripe_blocks[erasures[i]]);
    }

    ECProject::decode_PC(k1, m1, k2, m2, data_ptrs, coding_ptrs, block_size, erasures, failed_num);

    for(int i = 0; i < failed_num; i++)
    {
        printf("Block %d repaired:\n%s\n", erasures[i], stripe_blocks[erasures[i]]);
    }
}
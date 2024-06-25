#include <pc.h>
#include <rs.h>


/*
	PC: (k2 + m2) rows * (k1 + m1) cols
	encode_PC:
	data_ptrs: odered by row
	D(0) ... D(k1*k2-1)
	coding_ptrs: ordered by row
    R(0) ... R(k2*m1-1) C(0) ...  C(m2*k1-1) G(0) ... G(m2*m1-1)
*/
void ECProject::encode_PC(int k1, int m1, int k2, int m2, char **data_ptrs, char **coding_ptrs, int blocksize)
{
	// encode row parities
	for(int i = 0; i < k2; i++)
	{
		std::vector<char *> t_coding(m1);
    	char **coding = (char **)t_coding.data();
    	for(int j = 0; j < m1; j++)
    	{
    		coding[j] = coding_ptrs[i * m1 + j];
    	}
    	encode_RS(k1, m1, &data_ptrs[i * k1], coding, blocksize);
	}
	// encode column parities
	for(int i = 0; i < k1 + m1; i++)
	{
		std::vector<char *> t_data(k2);
    	char **data = (char **)t_data.data();
    	if(i < k1)
    	{
    		for(int j = 0; j < k2; j++)
    		{
    			data[j] = data_ptrs[j * k1 + i];
    		}
    	}
    	else
    	{
    		for(int j = 0; j < k2; j++)
    		{
    			data[j] = coding_ptrs[j * m1 + i - k1];
    		}
    	}
    	std::vector<char *> t_coding(m2);
    	char **coding = (char **)t_coding.data();
		if(i < k1)
		{
			for(int j = 0; j < m2; j++)
			{
				coding[j] = coding_ptrs[k2 * m1 + j * k1 + i];
			}
		}
		else
		{
			for(int j = 0; j < m2; j++)
			{
				coding[j] = coding_ptrs[k2 * m1 + k1 * m2 + j * m1 + i - k1];
			}
		}
    	encode_RS(k2, m2, data, coding, blocksize);
	}
}

// the index order is the same as shown above
void ECProject::decode_PC(int k1, int m1, int k2, int m2, char **data_ptrs, char **coding_ptrs, int blocksize, int *erasures, int failed_num)
{
	std::vector<std::vector<int>> failed_map(k2 + m2, std::vector<int>(k1 + m1, 0));
    std::vector<std::vector<char *>> blocks_map(k2 + m2, std::vector<char *>(k1 + m1, nullptr));
    std::vector<int> fb_row_cnt(k2 + m2, 0);
    std::vector<int> fb_col_cnt(k1 + m1, 0);

    int data_idx = 0;
    int parity_idx = 0;
    for(int i = 0; i < k2; i++)
    {
    	for(int j = 0; j < k1 + m1; j++)
    	{
    		if(j < k1)
    		{
    			blocks_map[i][j] = data_ptrs[data_idx++];
    		}
    		else
    		{
    			blocks_map[i][j] = coding_ptrs[parity_idx++];
    		}
    	}
    }
    int g_parity_idx = k2 * m1 + m2 * k1;
    for(int i = k2; i < k2 + m2; i++)
    {
    	for(int j = 0; j < k1 + m1; j++)
    	{
    		if(j < k1)
    		{
    			blocks_map[i][j] = coding_ptrs[parity_idx++];
    		}
    		else
    		{
    			blocks_map[i][j] = coding_ptrs[g_parity_idx++];
    		}
    	}
    }

    for(int i = 0; i < failed_num; i++)
    {
    	int row = -1, col = -1;
    	int failed_idx = erasures[i];
    	if(failed_idx < k1 * k2)	// data block
    	{
    		row = failed_idx / k1;
    		col = failed_idx % k1;
    	}
    	else if(failed_idx < (k1 + m1) * k2)	// row parity block
    	{
    		int tmp = failed_idx - k1 * k2;
    		row = tmp / m1;
    		col = tmp % m1 + k1;
    	}
    	else if(failed_idx < (k1 + m1) * k2 + m2 * k1)	// column parity block
    	{
    		int tmp = failed_idx - (k1 + m1) * k2;
    		row = tmp / k1 + k2;
    		col = tmp % k1;
    	}
    	else	//
    	{
    		int tmp = failed_idx - (k1 + m1) * k2 - m2 * k1;
    		row = tmp / m1 + k2;
    		col = tmp % m1 + k1;
    	}
    	failed_map[row][col] = 1;
    	fb_row_cnt[row]++;
    	fb_col_cnt[col]++;
    }

    while(failed_num > 0)
    {
	    // part one
	    for(int i = 0; i < k1 + m1; i++)
	    {
	        if(fb_col_cnt[i] <= m2 && fb_col_cnt[i] > 0) // repair on column
	        {
	        	std::vector<char *> data(k2, nullptr);
	        	std::vector<char *> coding(m2, nullptr);
	        	int *erasure = new int[fb_col_cnt[i] + 1];
	        	erasure[fb_col_cnt[i]] = -1;
	        	int cnt = 0;
	        	for(int jj = 0; jj < k2; jj++)
	        	{
	        		if(failed_map[jj][i])
	        		{
	        			erasure[cnt++] = jj;
	        		}
	        		data[jj] = blocks_map[jj][i];
	        	}
	        	for(int jj = 0; jj < m2; jj++)
	        	{
	        		if(failed_map[jj + k2][i])
	        		{
	        			erasure[cnt++] = jj + k2;
	        		}
	        		coding[jj] = blocks_map[jj + k2][i];
	        	}
	        	decode_RS(k2, m2, data.data(), coding.data(), blocksize, erasure, fb_col_cnt[i]);
	        	// update failed_map
		        for(int jj = 0; jj < k2 + m2; jj++)
		        {
				    if(failed_map[jj][i])
				    {
			            failed_map[jj][i] = 0;
			            failed_num -= 1;
			            fb_row_cnt[jj] -= 1;
			            fb_col_cnt[i] -= 1;
			        }
		        }
		        delete erasure;
		    }
    	}
    	if(failed_num == 0)
    	{
    		break;
    	}
    	// part two
        int max_row = -1;
        for(int i = 0; i < k2 + m2; i++)
	    {
	        if(fb_row_cnt[i] <= m1 && fb_row_cnt[i] > 0) // repair on row
	        {
	        	max_row = i;
	        	std::vector<char *> data(k1, nullptr);
	        	std::vector<char *> coding(m1, nullptr);
	        	int *erasure = new int[fb_row_cnt[i] + 1];
	        	erasure[fb_row_cnt[i]] = -1;
	        	int cnt = 0;
	        	for(int jj = 0; jj < k1; jj++)
	        	{
	        		if(failed_map[i][jj])
	        		{
	        			erasure[cnt++] = jj;
	        		}
	        		data[jj] = blocks_map[i][jj];
	        	}
	        	for(int jj = 0; jj < m1; jj++)
	        	{
	        		if(failed_map[i][jj + k1])
	        		{
	        			erasure[cnt++] = jj + k1;
	        		}
	        		coding[jj] = blocks_map[i][jj + k1];
	        	}
	        	decode_RS(k1, m1, data.data(), coding.data(), blocksize, erasure, fb_row_cnt[i]);
	        	// update failed_map
		        for(int jj = 0; jj < k1 + m1; jj++)
		        {
				    if(failed_map[i][jj])
				    {
			            failed_map[i][jj] = 0;
			            failed_num -= 1;
			            fb_row_cnt[i] -= 1;
			            fb_col_cnt[jj] -= 1;
			        }
		        }
		        delete erasure;
		    }
    	}
    	if(max_row == -1)
        {
	        std::cout << "Undecodable!!" << std::endl;
	        return;
        }
    }
}

// the index order is the same as shown above
bool ECProject::check_if_decodable_PC(int k1, int m1, int k2, int m2, int *erasures, int failed_num)
{
	std::vector<std::vector<int>> failed_map(k2 + m2, std::vector<int>(k1 + m1, 0));
    std::vector<int> fb_row_cnt(k2 + m2, 0);
    std::vector<int> fb_col_cnt(k1 + m1, 0);

    for(int i = 0; i < failed_num; i++)
    {
    	int row = -1, col = -1;
    	int failed_idx = erasures[i];
    	if(failed_idx < k1 * k2)	// data block
    	{
    		row = failed_idx / k1;
    		col = failed_idx % k1;
    	}
    	else if(failed_idx < (k1 + m1) * k2)	// row parity block
    	{
    		int tmp = failed_idx - k1 * k2;
    		row = tmp / m1;
    		col = tmp % m1 + k1;
    	}
    	else if(failed_idx < (k1 + m1) * k2 + m2 * k1)	// column parity block
    	{
    		int tmp = failed_idx - (k1 + m1) * k2;
    		row = tmp / k1 + k2;
    		col = tmp % k1;
    	}
    	else	//
    	{
    		int tmp = failed_idx - (k1 + m1) * k2 - m2 * k1;
    		row = tmp / m1 + k2;
    		col = tmp % m1 + k1;
    	}
    	failed_map[row][col] = 1;
    	fb_row_cnt[row]++;
    	fb_col_cnt[col]++;
    }

    while(failed_num > 0)
    {
	    // part one
	    for(int i = 0; i < k1 + m1; i++)
	    {
	        if(fb_col_cnt[i] <= m2 && fb_col_cnt[i] > 0) 	// repair on column
	        {
	        	// update failed_map
		        for(int jj = 0; jj < k2 + m2; jj++)
		        {
				    if(failed_map[jj][i])
				    {
			            failed_map[jj][i] = 0;
			            failed_num -= 1;
			            fb_row_cnt[jj] -= 1;
			            fb_col_cnt[i] -= 1;
			        }
		        }
		    }
    	}
    	if(failed_num == 0)
    	{
    		break;
    	}
    	// part two
        int max_row = -1;
        for(int i = 0; i < k2 + m2; i++)
	    {
	        if(fb_row_cnt[i] <= m1 && fb_row_cnt[i] > 0)	 // repair on row
	        {
	        	max_row = i;
	        	// update failed_map
		        for(int jj = 0; jj < k1 + m1; jj++)
		        {
				    if(failed_map[i][jj])
				    {
			            failed_map[i][jj] = 0;
			            failed_num -= 1;
			            fb_row_cnt[i] -= 1;
			            fb_col_cnt[jj] -= 1;
			        }
		        }
		    }
    	}
    	if(max_row == -1)
        {
	        return false;
        }
    }
    return true;
}
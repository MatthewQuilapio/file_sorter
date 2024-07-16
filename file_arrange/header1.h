#ifndef header1
#define header1

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <list>
#include <ctype.h>

void file_scan(std::string dir_name1);
void folder_scan(std::string dir_name2);
void file_move(std::string dir_name3, std::string ext_dir1);
void clean_outside_files(std::string dirname4);

void folder_scan_for_file_size(std::string dir_name5);
int* sort_numbers(std::vector<std::string>& file_vector_sizes, int *max_out_size, int *max_file_count);

void file_move_alphabetical(std::string dir_name6);

#endif 

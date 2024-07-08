#ifndef print_function
#define print_function

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <string.h>
#include <vector>
#include <algorithm>

void file_scan(std::string dir_name1);
void folder_scan(std::string dir_name2);
void file_move(std::string dir_name3, std::string ext_dir1);
void clean_outside_files(std::string dirname4);


#endif 
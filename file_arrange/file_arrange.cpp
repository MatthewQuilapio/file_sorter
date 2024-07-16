#include "header1.h"

using namespace std::filesystem;

int* sort_numbers(std::vector<std::string>& file_vector_sizes, int *max_out_size, int *max_file_count)
{
    std::vector<std::string> init_value = file_vector_sizes;
    std::vector<int> file_vector_length;

    long long unsigned int ctr = init_value.size();

    for(long long unsigned int i = 0; i < ctr; i++)
    {
        file_vector_length.push_back(static_cast<int>(init_value[i].length()));
    }

    sort(file_vector_length.begin(), file_vector_length.end());

    auto last = std::unique(file_vector_length.begin(), file_vector_length.end());          /* Removes duplicate element */

    file_vector_length.erase(last, file_vector_length.end()); 
    long long unsigned int ctr2 = static_cast <long long unsigned int>(file_vector_length.size());
    int* return_vector_size = new int[ctr2];
    *max_file_count = static_cast <int>(ctr);
    *max_out_size = static_cast <int>(ctr2);

    std::cout << "File count: " << *max_file_count << '\n';

    for(long long unsigned int j = 0; j < ctr2; j++)
    {
        return_vector_size[j] =  static_cast<int>(file_vector_length[j]);
        std::cout << "return_vector_size: " << return_vector_size[j] << '\n'; 
    }
    
    return return_vector_size;
}

void file_scan(std::string dir_name1)
{
    path dirFilePath = dir_name1;

    if (exists(dirFilePath) && is_directory(dirFilePath)) { 
        // Loop through each item (file or subdirectory) in 
        // the directory 
        for (const auto& entry : directory_iterator(dirFilePath)) { 
            // Output the path of the file or subdirectory 
            std::cout << "File: " << entry.path().filename() << '\n';
        }
    } 
    else 
    { 
        // Handle the case where the directory doesn't exist 
        std::cerr << "Directory not found." << '\n'; 

    }
}

void file_move(std::string dir_name3, std::string ext_dir1)
{
    path dirCopyPath = dir_name3;
    path dirPath_with_ext = dirCopyPath / ext_dir1;

    std::string extension_cond = '.' + ext_dir1;

    //std::cout << "Directory: " << extension_cond << '\n'; 

    
    if (exists(dirCopyPath) && is_directory(dirCopyPath))
    {
        for(const auto& entry : directory_iterator(dirCopyPath))
        {
            std::string filename_path = entry.path().filename().string();


            if (entry.is_regular_file())
            {
                if((entry.path().extension().string()) == extension_cond)
                {
                    auto source_file =  dirCopyPath / filename_path;
                    auto target_file = dirPath_with_ext / filename_path;
                    std::ifstream source(source_file, std::ios::binary); // Open source file
                    std::ofstream dest(target_file, std::ios::binary); // Open destination file
                    dest << source.rdbuf();
                }
            }
        }
    } 
}

void clean_outside_files(std::string dirname4)    /* Function for deleting outside files */
{
    path dirCleanPath = dirname4;

    for (const auto& entry : directory_iterator(dirCleanPath)) 
    {
        if (entry.is_regular_file())
        {
            auto file_for_cleaning = entry.path().string();
            remove(file_for_cleaning);
        }
    } 
}

/* This is to scan folders and create folders for moving files. */
void folder_scan(std::string dir_name2)
{
    path dirPath = dir_name2;
    std::vector<std::string> available_extensions;      /* definition for file extensions */
    std::vector<std::string> extension_length;          /* definition to get the file extension lengths listed */
    std::vector<std::string> arranged_ext_length;       /* definition for arrange extension lengths by numbers */
    std::vector<std::string> arranged_extension;        /* definition to arranged extension */


    if (exists(dirPath) && is_directory(dirPath))
    { 
        for (const auto& entry : directory_iterator(dirPath))
        { 
            /* Output the path of the file or subdirectory */
            std::string file_path_string = entry.path().string();

            std::string extension_name = entry.path().extension().string();
            extension_name.erase(std::remove(extension_name.begin(),extension_name.end(),'.'), extension_name.end());
            
            if(extension_name != "")    /* To prevent null values add to vector of extension names */
            {
                available_extensions.push_back(extension_name);     /* Add extension names */
            }
        }
        sort(available_extensions.begin(), available_extensions.end());

        auto last = std::unique(available_extensions.begin(), available_extensions.end());          /* Removes duplicate element */

        available_extensions.erase(last, available_extensions.end());        /* Removes last null element */

        long long unsigned int ext_size = static_cast <long long unsigned int> (available_extensions.size());

        /* This fixes the bugs for copying the wrong file to the wrong folder (e.g. a csv file copies to a .c folder) */

        for (long long unsigned int i = 0; i < ext_size; i++)
        {
            std::string::size_type temp_length = available_extensions[i].length();
            
            extension_length.push_back(std::to_string(temp_length));
        }

        sort(extension_length.begin(), extension_length.end());

        for (long long unsigned int j = 0, k = ext_size; j < ext_size; j++, k--)
        {
            arranged_ext_length.push_back(extension_length[k-1]);        
        }

        /* To determine the arrangement by string size */
        for (long long unsigned int l = 0; l < ext_size; l++)     
        {
           for (long long unsigned int m = 0; m < ext_size; m++)
           {
                if ((std::to_string(available_extensions[m].length())) == arranged_ext_length[l])
                {
                    arranged_extension.push_back(available_extensions[m]);
                }
           }
        }
            
        /*This function is used to create uncreated folders. */
        for (const std::string& ext : arranged_extension)
        {
            path dirPath_ext = dirPath / ext; /* To concatenate the extension folder and current path*/
            if (exists(dirPath_ext) && is_directory(dirPath))
            {
                /* file_scan(dirPath_ext.string()); */
                file_move(dir_name2, ext);
            }
            else
            {
                create_directory(dirPath_ext);      /* Create a new folder with extension name */
                std::cout << "Folder Created!" << '\n';
                file_move(dir_name2, ext);
            }
        }
    } 
    else 
    { 
        
        std::cerr << "Directory not found." << '\n';  /* Handle the case where the directory doesn't exist  */
    }
}

void folder_scan_for_file_size(std::string dir_name5)
{
    path dirPath = dir_name5;
    int* out_sizes;
    int max_out_size;
    int max_file_count;
    std::vector<std::string> file_sizes;
    std::vector<int> file_sizes_out;
    std::vector<std::string> file_list;

    if (exists(dirPath) && is_directory(dirPath))
    { 
        for (const auto& entry : directory_iterator(dirPath))
        { 
            if (entry.is_regular_file())
            {
                std::string filename_path = entry.path().filename().string();
                file_list.push_back(filename_path);
                auto answer = file_size(entry) / static_cast <unsigned int> (1000);
                file_sizes.push_back(std::to_string(answer));
            }
            
        }

        sort(file_sizes.begin(), file_sizes.end());

        out_sizes = sort_numbers(file_sizes, &max_out_size, &max_file_count);

        //std::cout << "S: " << max_out_size << '\n';


        for (int i = 0; i < max_out_size; i++) 
        {
            std::string digit_dir = "Digit_" + std::to_string(out_sizes[i]);
            path dirPath_digits = dirPath / digit_dir;
            std::cout << "Directory: " << dirPath_digits << '\n';
            //std::cout << "Size: " << out_sizes[i] << '\n';
            for (int j = 0; j < max_file_count; j++)
            {  
                if (exists(dirPath_digits) && is_directory(dirPath_digits))
                {
                    auto DirPath_size_copy = dirPath / file_list[j];
                    auto answer2 = file_size(DirPath_size_copy) / static_cast <unsigned int> (1000);
                    std::cout << "Size: " << answer2 << '\n';
                    if (std::to_string(answer2).size() == out_sizes[i])
                    {
                        auto source_file =  DirPath_size_copy;
                        auto target_file = dirPath_digits / file_list[j];
                        std::ifstream source(source_file, std::ios::binary); // Open source file
                        std::ofstream dest(target_file, std::ios::binary); // Open destination file
                        dest << source.rdbuf();
                    }
                  
                }
                else
                {
                    create_directory(dirPath_digits);      /* Create a new folder with extension name */
                    auto DirPath_size_copy = dirPath / file_list[j];
                    auto answer2 = file_size(DirPath_size_copy) / static_cast <unsigned int> (1000);
                    std::cout << "Size: " << answer2 << '\n';
                    if (std::to_string(answer2).size() == out_sizes[i])
                    {
                        auto source_file =  DirPath_size_copy;
                        auto target_file = dirPath_digits / file_list[j];
                        std::ifstream source(source_file, std::ios::binary); // Open source file
                        std::ofstream dest(target_file, std::ios::binary); // Open destination file
                        dest << source.rdbuf();
                    }                 
                }
            }
        }
    } 
    else 
    {   
        std::cerr << "Directory not found." << '\n';  /* Handle the case where the directory doesn't exist  */
    }
}

void file_move_alphabetical(std::string dir_name6)
{
    path dirPath = dir_name6;
    std::string temporary_file_name;
    std::vector<std::string> file_names_alph;
    std::vector<std::string> file_names_list;
    int ctr = 0;
    
    if (exists(dirPath) && is_directory(dirPath))
    {
        for (const auto& entry : directory_iterator(dirPath)) 
        { 
            if (entry.is_regular_file())
            {
                temporary_file_name = entry.path().filename().string();
                file_names_list.push_back(temporary_file_name);
                char temp_char = static_cast<char>(toupper(temporary_file_name[0]));
                file_names_alph.push_back(std::to_string(temp_char));
                //std::cout << "File: " << file_names_list[ctr] << " "<< ctr << temp_char <<'\n';
                //std::cout << "Char: " << file_names_alph[ctr] << '\n';
                ctr++;
            }
        }
        sort(file_names_alph.begin(), file_names_alph.end());
        auto unique_alph_list = std::unique(file_names_alph.begin(), file_names_alph.end());
        file_names_alph.erase(unique_alph_list, file_names_alph.end());
        long long unsigned int file_names_alph_size = file_names_alph.size();
        long long unsigned int file_names_list_size = file_names_list.size();
        //std::cout << "Size: " << file_names_alph_size << '\n';
        

        for (long long unsigned int i = 0; i < file_names_alph_size; i++)
        {
            char alph_char = stoi(file_names_alph[i]);
            std::string temp_str(1, alph_char); /* Converts single character into string so when printing it will print as string */
            path dirPath_char = dirPath / temp_str;
            
            std::cout << "dirPath_char: " << dirPath_char << '\n';
            for (long long unsigned int j = 0; j < file_names_list_size; j++)
            {
                if (exists(dirPath_char) && is_directory(dirPath_char))
                {
                    path dirPath_copy = dirPath / file_names_list[j];
                    if(file_names_list[j].front() == alph_char)
                    {
                        auto source_file =  dirPath_copy;
                        auto target_file = dirPath_char / file_names_list[j];
                        std::ifstream source(source_file, std::ios::binary); // Open source file
                        std::ofstream dest(target_file, std::ios::binary); // Open destination file
                        dest << source.rdbuf();
                    }
                }
                else
                {
                    create_directory(dirPath_char);
                    path dirPath_copy = dirPath / file_names_list[j];
                    if(file_names_list[j].front() == alph_char)
                    {
                        auto source_file =  dirPath_copy;
                        auto target_file = dirPath_char / file_names_list[j];
                        std::ifstream source(source_file, std::ios::binary); // Open source file
                        std::ofstream dest(target_file, std::ios::binary); // Open destination file
                        dest << source.rdbuf();
                    }
                }

            }
        }
    }
    else
    {
        std::cerr << "Directory not found." << '\n';  /* Handle the case where the directory doesn't exist  */
    }

}


int main()
{
    std::string dir_input;
    std::string input_command;

    printf("Please Enter Directory: \n");
    std::cin >> dir_input;

    printf("Please Enter Command: \n");
    std::cin >> input_command;

    /* file_scan(dir_input); */
    if(input_command == "0")
    {
        folder_scan(dir_input);
        clean_outside_files(dir_input);
    }
    else if(input_command == "1")
    {
        folder_scan_for_file_size(dir_input);
        clean_outside_files(dir_input);
    }
    else if(input_command == "2")
    {
        file_move_alphabetical(dir_input);
    }

    system("pause");
}

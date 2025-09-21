#include "header1.h"

using namespace std::filesystem;

/**
 * @brief Sorts a list of file sizes (as strings), removes duplicates,
 *        and returns the sorted unique lengths as an integer array.
 * 
 * @param file_vector_sizes  Vector of file size strings.
 * @param max_out_size       Output: Number of unique lengths.
 * @param max_file_count     Output: Total number of input files.
 * @return int*              Dynamically allocated array of unique sizes.
 *                           (Caller is responsible for freeing memory.)
 */
int* sort_numbers(std::vector<std::string>& file_vector_sizes, int *max_out_size, int *max_file_count)
{
    std::vector<std::string> init_value = file_vector_sizes;
    std::vector<int> file_vector_length;

    long long unsigned int ctr = init_value.size();
    
    /* Convert string sizes into integer lengths */
    for(long long unsigned int i = 0; i < ctr; i++)
    {
        file_vector_length.push_back(static_cast<int>(init_value[i].length()));
    }

    /* Sort the lengths */
    sort(file_vector_length.begin(), file_vector_length.end());

    /* Removes duplicate element */
    auto last = std::unique(file_vector_length.begin(), file_vector_length.end());          
    file_vector_length.erase(last, file_vector_length.end());
    
    long long unsigned int ctr2 = static_cast <long long unsigned int>(file_vector_length.size());
    int* return_vector_size = new int[ctr2];

    *max_file_count = static_cast <int>(ctr);
    *max_out_size = static_cast <int>(ctr2);

    std::cout << "File count: " << *max_file_count << '\n';

    /* Copy results into return array */
    for(long long unsigned int j = 0; j < ctr2; j++)
    {
        return_vector_size[j] =  static_cast<int>(file_vector_length[j]);
        std::cout << "return_vector_size: " << return_vector_size[j] << '\n'; 
    }
    
    return return_vector_size;
}

/**
 * @brief Scans a directory and prints all file/folder names.
 * 
 * @param dir_name1  Path to directory.
 */
void file_scan(std::string dir_name1)
{
    path dirFilePath = dir_name1;

    if (exists(dirFilePath) && is_directory(dirFilePath)) { 
        /* Loop through each item (file or subdirectory) in the directory  */
        for (const auto& entry : directory_iterator(dirFilePath)) { 
            /* Output the path of the file or subdirectory */ 
            std::cout << "File: " << entry.path().filename() << '\n';
        }
    } 
    else 
    { 
        /* Handle the case where the directory doesn't exist  */
        std::cerr << "Directory not found." << '\n'; 

    }
}

/**
 * @brief Moves files with a specific extension into a subfolder.
 * 
 * @param dir_name3  Source directory.
 * @param ext_dir1   Extension to filter (e.g., "txt").
 */
void file_move(std::string dir_name3, std::string ext_dir1)
{
    path dirCopyPath = dir_name3;
    path dirPath_with_ext = dirCopyPath / ext_dir1;

    std::string extension_cond = '.' + ext_dir1;

    if (exists(dirCopyPath) && is_directory(dirCopyPath))
    {
        for(const auto& entry : directory_iterator(dirCopyPath))
        {
            std::string filename_path = entry.path().filename().string();
            
            if (entry.is_regular_file())
            {
                /* Check if file extension matches */
                if((entry.path().extension().string()) == extension_cond)
                {
                    auto source_file =  dirCopyPath / filename_path;
                    auto target_file = dirPath_with_ext / filename_path;

                    /* Copy file into extension-specific folder */
                    std::ifstream source(source_file, std::ios::binary);
                    std::ofstream dest(target_file, std::ios::binary);
                    dest << source.rdbuf();
                }
            }
        }
    } 
}

/**
 * @brief Deletes all files inside a directory.
 * 
 * @param dirname4  Path to directory.
 */
void clean_outside_files(std::string dirname4)
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

/**
 * @brief Organizes files into subfolders based on their extension.
 *        Creates folders automatically if they don’t exist.
 * 
 * @param dir_name2  Path to directory.
 */
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

            /* To prevent null values add to vector of extension names */
            if(extension_name != "")    
            {
                available_extensions.push_back(extension_name);
            }
        }

         /* Sort and removes duplicate extensions */
        sort(available_extensions.begin(), available_extensions.end());
        auto last = std::unique(available_extensions.begin(), available_extensions.end());         
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

        /* Match lengths back to extensions */
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
            
        /*Create folders and move files into them */
        for (const std::string& ext : arranged_extension)
        {
            path dirPath_ext = dirPath / ext;
            if (exists(dirPath_ext) && is_directory(dirPath))
            {
                file_move(dir_name2, ext);
            }
            else
            {
                 /* Create a new folder with extension name */
                create_directory(dirPath_ext);     
                std::cout << "Folder Created!" << '\n';
                file_move(dir_name2, ext);
            }
        }
    } 
    else 
    {
        /* Handle the case where the directory doesn't exist  */
        std::cerr << "Directory not found." << '\n'; 
    }
}

/**
 * @brief Organizes files into folders based on the number of digits
 *        in their file sizes (in KB).
 * 
 * @param dir_name5  Path to directory.
 */
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
        /* Collect file sizes and names */
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
        
        /* Create directories based on digit count of file size */
        for (int i = 0; i < max_out_size; i++) 
        {
            std::string digit_dir = "Digit_" + std::to_string(out_sizes[i]);
            path dirPath_digits = dirPath / digit_dir;
            std::cout << "Directory: " << dirPath_digits << '\n';

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
                        std::ifstream source(source_file, std::ios::binary);
                        std::ofstream dest(target_file, std::ios::binary);
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
        /* Handle the case where the directory doesn't exist  */
        std::cerr << "Directory not found." << '\n';  
    }
}

/**
 * @brief Organizes files alphabetically into folders
 *        based on their starting character.
 * 
 * @param dir_name6  Path to directory.
 */
void file_move_alphabetical(std::string dir_name6)
{
    path dirPath = dir_name6;
    std::string temporary_file_name;
    std::vector<std::string> file_names_alph;
    std::vector<std::string> file_names_list;
    int ctr = 0;
    
    if (exists(dirPath) && is_directory(dirPath))
    {
        /* Extract first letters of filenames */
        for (const auto& entry : directory_iterator(dirPath)) 
        { 
            if (entry.is_regular_file())
            {
                temporary_file_name = entry.path().filename().string();
                file_names_list.push_back(temporary_file_name);
                char temp_char = static_cast<char>(toupper(temporary_file_name[0]));
                file_names_alph.push_back(std::to_string(temp_char));
                ctr++;
            }
        }

        /* Sort and deduplicate letters */
        sort(file_names_alph.begin(), file_names_alph.end());
        auto unique_alph_list = std::unique(file_names_alph.begin(), file_names_alph.end());
        file_names_alph.erase(unique_alph_list, file_names_alph.end());
        
        long long unsigned int file_names_alph_size = file_names_alph.size();
        long long unsigned int file_names_list_size = file_names_list.size();        

        /* Create directories and move files accordingly */
        for (long long unsigned int i = 0; i < file_names_alph_size; i++)
        {
            char alph_char = stoi(file_names_alph[i]);
            /* Converts single character into string so when printing it will print as string */
            std::string temp_str(1, alph_char); 
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
                        std::ifstream source(source_file, std::ios::binary);
                        std::ofstream dest(target_file, std::ios::binary);
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
                        std::ifstream source(source_file, std::ios::binary);
                        std::ofstream dest(target_file, std::ios::binary);
                        dest << source.rdbuf();
                    }
                }

            }
        }
    }
    else
    {
        std::cerr << "Directory not found." << '\n'; 

}

/**
 * @brief Entry point of the program.
 * 
 * User chooses a directory and a command:
 *  - "0": Organize files by extension.
 *  - "1": Organize files by file size digit length.
 *  - "2": Organize files alphabetically.
 */
int main()
{
    std::string dir_input;
    std::string input_command;

    printf("Please Enter Directory: \n");
    std::cin >> dir_input;

    printf("Please Enter Command: \n");
    std::cin >> input_command;

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



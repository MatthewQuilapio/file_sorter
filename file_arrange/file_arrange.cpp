#include "print_function.h"

using namespace std::filesystem;

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

    
    if (exists(dirCopyPath) && is_directory(dirCopyPath))
    {
        for(const auto& entry : directory_iterator(dirCopyPath))
        {
            std::string filename_path = entry.path().filename().string();


            if (entry.is_regular_file())
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

void delete_unwanted_file(std::string dirname4)
{
    path dirDelPath = dirname4;
    path directory_ext_delete;

    if (exists(dirDelPath) && is_directory(dirDelPath)) 
    { 
        // Loop through each item (file or subdirectory) in 
        // the directory 
        std::vector<std::string> available_extensions;      /* definition for file extensions */
        std::vector<std::string> directory_array_delete;
        for (const auto& entry : directory_iterator(dirDelPath)) 
        { 
            // Output the path of the file or subdirectory
            std::string filename_path = entry.path().filename().string();

            std::string extension_name = entry.path().extension().string();
            extension_name.erase(std::remove(extension_name.begin(),extension_name.end(),'.'), extension_name.end());
            
            if(extension_name != "")    /* To prevent null values add to vector of extension names */
            {
                available_extensions.push_back(extension_name);     /* Add extension names */
            }
        }

        sort(available_extensions.begin(), available_extensions.end());

        auto last = std::unique(available_extensions.begin(), available_extensions.end());

        available_extensions.erase(last, available_extensions.end());

        int ext_len = static_cast<int>(available_extensions.size());  
        
        for(int i = 0; i < ext_len; i++)
        {
            directory_ext_delete = dirDelPath / available_extensions[i];
            directory_array_delete.push_back(directory_ext_delete.string());

            path new_dir_delete = directory_array_delete[i];

            for (const auto& entry : directory_iterator(new_dir_delete))
            {   
                if (entry.is_regular_file())
                {
                    auto curr_ext = entry.path().extension().string();
                    curr_ext.erase(std::remove(curr_ext.begin(),curr_ext.end(),'.'), curr_ext.end());
                    if(curr_ext != available_extensions[i])     /* For file deletion*/
                    {
                        auto delete_file = entry.path();
                        /* std::cout << "Delete: " << delete_file << '\n'; */
                        remove(delete_file);

                    }
                   /*  else        // For keeping file
                    {
                        
                    } */
                }
            }   
        }
    }
}

void clean_outside_files(std::string dirname5)    /* Function for deleting outside files */
{
    path dirCleanPath = dirname5;

    for (const auto& entry : directory_iterator(dirCleanPath)) 
    {
        if (entry.is_regular_file())
        {
            auto file_for_cleaning = entry.path().string();
            remove(file_for_cleaning);
        }
    } 
}

void folder_scan(std::string dir_name2)
{
    path dirPath = dir_name2;
    std::vector<std::string> available_extensions;      /* definition for file extensions */

    if (exists(dirPath) && is_directory(dirPath))
    { 
        // Loop through each item (file or subdirectory) in 
        // the directory 
        int ctr = 0;
        for (const auto& entry : directory_iterator(dirPath))
        { 
            // Output the path of the file or subdirectory
            std::string file_path_string = entry.path().string();

            std::string extension_name = entry.path().extension().string();
            extension_name.erase(std::remove(extension_name.begin(),extension_name.end(),'.'), extension_name.end());
            
            if(extension_name != "")    /* To prevent null values add to vector of extension names */
            {
                available_extensions.push_back(extension_name);     /* Add extension names */
            }
            
            ctr++;
        }
        sort(available_extensions.begin(), available_extensions.end());

        auto last = std::unique(available_extensions.begin(), available_extensions.end());

        available_extensions.erase(last, available_extensions.end());
        
    
        /*This function is used to print vector values of extensions*/
        for (const std::string& ext : available_extensions)
        {
            path dirPath_ext = dirPath / ext; /* To concatenate the extension folder and current path*/
           
           std::cout << "Folder: " << dirPath_ext << '\n';
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
        // Handle the case where the directory doesn't exist 
        std::cerr << "Directory not found." << '\n';
    }
}


int main()
{
    std::string dir_input;

    printf("Please Enter Directory: \n");
    std::cin >> dir_input;

    /* file_scan(dir_input); */
    folder_scan(dir_input);
    delete_unwanted_file(dir_input);
    clean_outside_files(dir_input);

    system("pause");
}
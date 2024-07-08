#include "header1.h"

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

void folder_scan(std::string dir_name2)
{
    path dirPath = dir_name2;
    std::vector<std::string> available_extensions;      /* definition for file extensions */
    std::vector<std::string> extension_length;
    std::vector<std::string> arranged_ext_length;
    std::vector<std::string> arranged_extension;


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

        int ext_size = static_cast <signed int> (available_extensions.size());

        for (int i = 0; i < ext_size; i++)
        {
            std::string::size_type temp_length = available_extensions[i].length();
            
            extension_length.push_back(std::to_string(temp_length));
        }

        sort(extension_length.begin(), extension_length.end());

        for (int j = 0, k = ext_size; j < ext_size; j++, k--)
        {
            arranged_ext_length.push_back(extension_length[k-1]);        
        }

        for (int l = 0; l < ext_size; l++)
        {
           for (int m = 0; m < ext_size; m++)
           {
                if ((std::to_string(available_extensions[m].length())) == arranged_ext_length[l])
                {
                    arranged_extension.push_back(available_extensions[m]);
                }
           }
        }
            
        /*This function is used to print vector values of extensions*/
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
        // Handle the case where the directory doesn't exist 
        std::cerr << "Directory not found." << '\n';
    }
}

int main()
{
    std::string dir_input;

    printf("Please Enter Directory: \n");
    std::cin >> dir_input;

    file_scan(dir_input);
    folder_scan(dir_input);
    clean_outside_files(dir_input);

    system("pause");
}
#include "val.hpp"

#include <filesystem>
namespace fs = std::filesystem;


std::vector<std::string> VALID_FILE_EXT {".jpg", ".png", ".jpeg"}; // path.extenstion().string() retuns ext with the dot included.


bool is_img_ext_valid(const std::vector<std::string>& vec, std::string val){
    auto it = std::find(vec.begin(), vec.end(), val);
    return it != vec.end();
}

bool Val::read_img_files(images_t& images){
    // FIXME Does directory iterator return full paths o rrelative paths
    for (const auto& img_f: fs::directory_iterator(images.path)){
        // Validation of file path and extension

        std::string img_f_path_str = img_f.path().string();

        if (!img_f.is_regular_file()) {
            std::cerr << img_f_path_str << ": Is not a valid file" << '\n';
            return false;
        
        }

        if (!is_img_ext_valid(VALID_FILE_EXT, img_f.path().extension().string())) {
            std::cerr << img_f_path_str << ": extension is not valid for a image file" << '\n';
            return false;
        }

        cv::Mat img = cv::imread(img_f_path_str);

        if (img.empty()){
            std::cerr << img_f_path_str << ": Unable to read image file" << '\n';
            return false;
        }

        images.imgs_mat.push_back(img);
        images.f_names.push_back(img_f.path().filename().string());
        images.size++;
    }
    std::cout << images.size << ": Images have been read from " << images.path << '\n'; 
    return true;
}
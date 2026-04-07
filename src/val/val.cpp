#include "val.hpp"

#include <filesystem>
namespace fs = std::filesystem;


std::vector<std::string> VALID_FILE_EXT {".jpg", ".png", ".jpeg"}; // path.extenstion().string() retuns ext with the dot included.


bool is_img_ext_valid(const std::vector<std::string>& vec, std::string val){
    auto it = std::find(vec.begin(), vec.end(), val);
    return it != vec.end();
}

// Used for inter module check --> throw err
void Val::validate_images(const cv::Mat& img_1, const cv::Mat& img_2){
    /*
        Type --> No. of channels (in each pixel) (1 --> greyscale, 3 --> BGR) 
        + Data type of each channel value (CV_8U --> Unsigned int, 8 bits long)
    */
    if (img_1.size() != img_2.size() || img_1.type() != img_2.type()){
        throw std::invalid_argument("Both inputted images are not of the same type or size");
    }
}


bool Val::read_img_files(Images& images, const std::string& lib_path){
    // AUtomatically converts string to path obj
    if (!fs::exists(lib_path)) {
        throw std::runtime_error("Image dir does not exist: " + lib_path);
    }

    for (const auto& img_f: fs::directory_iterator(lib_path)){
        // Validation of file path and extension

        std::string img_f_path_str = img_f.path().string();

        if (!img_f.is_regular_file()) {
            std::cerr << img_f_path_str << ": Is not a valid file" << '\n';
            continue;
        
        }

        if (!is_img_ext_valid(VALID_FILE_EXT, img_f.path().extension().string())) {
            std::cerr << img_f_path_str << ": extension is not valid for a image file" << '\n';
            continue;
        }

        cv::Mat img_mat = cv::imread(img_f_path_str);

        if (img_mat.empty()){
            std::cerr << img_f_path_str << ": Unable to read image file" << '\n';
            continue;
        }

        std::pair<std::string, cv::Mat> img_map {img_f.path().stem().string(), img_mat};
        images.push_back(img_map);
    }

    std::cout << images.size() << ": Images have been read from " << lib_path << '\n'; 

    if (images.empty()){
        std::cerr << "No images have been successfully read from " << lib_path << '\n';
        return false;
    }
    return true;
}
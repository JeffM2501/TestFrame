
#include "platform_tools.h"

#include "../clip/clip.h"

std::string OpenFileDialog(const char* filename, std::vector<std::pair<std::string, std::string>> filterValues);
std::string SaveFileDialog(const char* filename, std::vector<std::pair<std::string, std::string>> filterValues);

namespace PlatformTools
{
    void CopyImageToClipboard(Image& image)
    {
        clip::image_spec spec;
        spec.width = image.width;
        spec.height = image.height;
        spec.bits_per_pixel = 32;
        spec.bytes_per_row = spec.width * 4;
        spec.red_mask = 0xff;
        spec.green_mask = 0xff00;
        spec.blue_mask = 0xff0000;
        spec.alpha_mask = 0xff000000;
        spec.red_shift = 0;
        spec.green_shift = 8;
        spec.blue_shift = 16;
        spec.alpha_shift = 24;
        clip::image img(image.data, spec);
        clip::set_image(img);
    }

    std::string ShowOpenFileDialog(const char* filename, std::vector<std::pair<std::string, std::string>> filterValues)
    {
        return OpenFileDialog(filename, filterValues);
    }

    std::string ShowOpenFileDialog(const char* filename)
    {
        std::vector<std::pair<std::string, std::string>> filterValues;
        return OpenFileDialog(filename, filterValues);
    }

    std::string ShowSaveFileDialog(const char* filename, std::vector<std::pair<std::string, std::string>> filterValues)
    {
        return SaveFileDialog(filename, filterValues);
    }

    std::string ShowSaveFileDialog(const char* filename)
    {
        std::vector<std::pair<std::string, std::string>> filterValues;
        return SaveFileDialog(filename, filterValues);
    }
}
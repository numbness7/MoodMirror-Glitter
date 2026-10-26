#include <color.hpp>


namespace cdata{
    std::string col2str(ColorRGBA col){
        return "(" 
            + std::to_string(static_cast<unsigned int>(col.red  )) + ", "
            + std::to_string(static_cast<unsigned int>(col.green)) + ", "
            + std::to_string(static_cast<unsigned int>(col.blue )) + ", "
            + std::to_string(static_cast<unsigned int>(col.alpha)) + ")";
    }
}
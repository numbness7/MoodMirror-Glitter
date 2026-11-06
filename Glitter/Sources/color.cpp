#include <color.hpp>


namespace cdata{
    std::string col2str(ColorRGBA col){
        return "(" 
            + std::to_string(static_cast<unsigned int>(col.red  )) + ", "
            + std::to_string(static_cast<unsigned int>(col.green)) + ", "
            + std::to_string(static_cast<unsigned int>(col.blue )) + ", "
            + std::to_string(static_cast<unsigned int>(col.alpha)) + ")";
    }
    const ColorRGBA vec4ToCol(const glm::vec4 vec4)
    {
        return ColorRGBA{
            (unsigned char)((vec4.x)* (255.0f)),
            (unsigned char)((vec4.y)* (255.0f)),
            (unsigned char)((vec4.z)* (255.0f)),
            (unsigned char)((vec4.a)* (255.0f))
        };
    }
    unsigned char vec3PartToColorPart(const glm::vec3 vec3, const unsigned char part)
    {
        assert(part >= 0 && part <= 3);
        return (unsigned char)((vec3[part]) * (255.0f));
    }
    unsigned char vec4PartToColorPart(const glm::vec4 vec4, const unsigned char part)
    {
        assert(part >= 0 && part <= 4);
        return (unsigned char)((vec4[part]) * (255.0f));
    }
    bool ColorRGBA::operator==(const ColorRGBA &color) const
    {
        return 
            color.red == this->red &&
            color.green == this->green &&
            color.blue == this->blue &&
            color.alpha == this->alpha;
    }
}
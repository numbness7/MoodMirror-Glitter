#ifndef TEXTURES_H
#define TEXTURES_H
#include <string>
#include <color.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
#include <thread>
#include <chrono>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <mathoperations.hpp>
#include <variablemanipulation.hpp>
// Data sizes
#define BYTESIZE 8
// #PNG -------------------------------------------------------------
/// @brief Raw data of a png file, may have been generated procedurally.
///
/// row is the y value of the pixel to be displayed. The y-axis points upward.
///
/// col is the x value of the pixel to be displayed. The x-axis points right.
///
/// gridIndex is the index of a pixel.
///
/// index is the index of a channel of a pixel.
class PngData{
    public:
        /// @brief Allocates memory for png data to fit the space defined by the parameters. nrChannels is set to 4.
        /// @param width
        /// @param height 
        PngData(const int width, const int height);
        /// @brief Allocates memory for png data to fit the space defined by the parameters.
        /// @param width 
        /// @param height 
        /// @param nrChannels color channels
        PngData(const int width, const int height, const int nrChannels);
        /// @brief Either copies the data from data based on the parameters or assigns the objects data to the passed data.
        /// @param width 
        /// @param height 
        /// @param nrChannels color channels
        /// @param data raw uncompressed png data
        /// @param copy if true do copy, otherwise assign this objects data to passed data
        PngData(const int width, const int height, const int nrChannels, unsigned char* data, bool copy=false);
        /// @brief Copies png data.
        /// @param pngData to be copied
        PngData(const PngData& pngData);
        /// @brief Deletes data.
        ~PngData();
        /// @brief Returns the index of the unsigned char at the parameters.
        /// @param row 
        /// @param col 
        /// @param channel 
        /// @return 
        int index(int row, int col, int channel) const { assert(row >= 0 && row < height && col >= 0 && col < width && channel >= 0 && channel < nrChannels); return row*width*nrChannels+col*nrChannels+channel; }
        /// @brief Returns the index of unsigned char.
        /// @param gridIndex index ignoring nrChannels.
        /// @param channel
        /// @return 
        int indexFromGrid(const int gridIndex, const int channel) const { assert(gridIndex >= 0 && gridIndex < gridSize() && channel >= 0 && channel < nrChannels);  return gridIndex*nrChannels + channel; }
        int size() const { return height*width*nrChannels; }
        /// @brief Size ignoring nrChannels.
        /// @return 
        int gridSize() const { return height*width; }
        int row(int index) const { assert(index >= 0 && index < size()); return floorWrap(index,(width*nrChannels)); }
        int col(int index)  const{ return floorWrap(index - row(index)*width*nrChannels,nrChannels); }
        int rowFromGrid(const int gridIndex) const { assert(gridIndex >= 0 && gridIndex < gridSize()); return floorWrap(gridIndex,(width)); }
        int colFromGrid(const int gridIndex) const { return gridIndex - rowFromGrid(gridIndex)*width; }
        int rFG(int gridIndex) const { return rowFromGrid(gridIndex); }
        int cFG(int gridIndex) const { return colFromGrid(gridIndex); }
        int gridIndex(int row, int col) const { assert(row >= 0 && row < height && col >= 0 && col < width); return width*row + col; }
        int gridIndex(int index){ assert(index >= 0 && index < size()); return floorWrap(index,nrChannels); }
        int channel(int index) { assert(index >= 0 && index < size()); return index - row(index)*width*nrChannels - col(index)*nrChannels; }
        cdata::ColorRGB  getColorRGB(int row, int col) const;
        cdata::ColorRGBA getColorRGBA(int row, int col) const;
        void setData(int row, int col, int channel, const unsigned char& byte);
        unsigned char getData(const int gridIndex, const int channel) const { return data[indexFromGrid(gridIndex,channel)]; }
        void setData(const int gridIndex, const int channel, const unsigned char& byte) { data[indexFromGrid(gridIndex,channel)] = byte; }
        void setData(int index, const unsigned char& byte) { assert(index >= 0 && index < size()); data[index] = byte; }
        unsigned char getData(int row, int col, int channel) const;
        /// @brief Set the color of a pixel with RGBA channels.
        /// @param gridIndex 
        /// @param color 
        void setColor(const int gridIndex, const cdata::ColorRGBA color){
            setData(gridIndex, redChannel, color.red);
            setData(gridIndex, blueChannel, color.blue);
            setData(gridIndex, greenChannel, color.green);
            setData(gridIndex, alphaChannel, color.alpha);
        }
        /// @brief Set the color of a pixel with RGB channels.
        /// @param gridIndex 
        /// @param color 
        void setColor(int gridIndex, cdata::ColorRGB color){
            setData(gridIndex, redChannel, color.red);
            setData(gridIndex, blueChannel, color.blue);
            setData(gridIndex, greenChannel, color.green);
        }
        /// @brief Set the color of a pixel with RGBA channels.
        /// @param row 
        /// @param col 
        /// @param color 
        void setColor(const int row, const int col, const cdata::ColorRGBA color){
            setColor(gridIndex(row,col), color);
        }
        /// @brief Set the color of a pixel with RGB channels.
        /// @param row 
        /// @param col 
        /// @param color 
        void setColor(int row, int col, cdata::ColorRGB color){
            setColor(gridIndex(row,col), color);
        }
        bool isCompatable(const PngData& pngData) const { return (this->width == pngData.width && this->height == pngData.height && this->nrChannels == pngData.nrChannels); }
        /// @brief The result of a previous = operation.
        /// @return True if this objects width,height, and nrChannels parameters equaled the other objects respective parameters; false otherwise.
        bool isValid() const{
            return valid;
        }
        const unsigned char* getData () const { return data; }
        /// @brief Set data of this object to data of pngData. No allocation is done here.
        /// @param pngData 
        /// @return this object.
        PngData& operator=(const PngData& pngData) noexcept {
            valid = isCompatable(pngData);
            if(valid)
                doCopy(this->data, pngData.getData(), this->size(), pngData.size());
            return *this;
        }
        
        const int width;
        const int height;
        const int nrChannels;
        static const int nrChannelsDefault = 4;
        static const int redChannel = 0;
        static const int greenChannel = 1;
        static const int blueChannel = 2;
        static const int alphaChannel = 3;
        
    private:
        unsigned char* data;
        bool valid = true;
};

void generatePNGBlank(PngData& pngData);
/// @brief Generate a OpenGL texture from pngData.
/// @param pngData texture data.
/// @return reference to OpenGL texture.
unsigned int createPngTexture(const PngData& pngData);
void freePNGTextureData(unsigned char* data);
void generatePNGData(unsigned char* data, int width, int height, int nrChannels);
bool isOnBorder(int width, int height, int row, int col, int hThickness, int vThickness);
unsigned char getBorderCheckColor(int width, int height, int row, int col, int hThickness, int vThickness);
void generatePNGBorder(unsigned char* data, int width, int height, int nrChannels, int hThickness, int vThickness);
void generatePNGBorder(PngData& pngData, const int width, const int height,  const int hThickness, const int vThickness, const cdata::ColorRGBA borderColor, const cdata::ColorRGBA innerColor, const bool ignoreInner, const bool ignoreBorder);
unsigned int obtainPNGBorder(int width, int height,  int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder);
unsigned int obtainPNGBorder(PngData& pngData,  int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder);
unsigned int obtainPNGBlank(PngData& pngData);
cdata::ColorRGBA pngColorFromValue(const unsigned int& value);
unsigned int valueFromPngColor(cdata::ColorRGBA color);
PngData loadPngTexture(std::string texture_filepath);
void loadPngTexture(std::string texture_filepath, PngData& pD);
void pngDataSavePng(const std::string filepath, const PngData& pngData);
unsigned char* initializePNGTextureData(int width, int height, int nrChannels);
unsigned int createPngTexture(const PngData* pngData);
unsigned int createPngTexture(const PngData& pngData);
void pngDataSavePng(const std::string filepath, const PngData& pngData);


// #Texture -------------------------------------------------------------

unsigned int createPngTexture(std::string texture_filepath);
unsigned int createTexture(std::string texture_filepath, std::string filetype);
void createTexture(unsigned int &texture, std::string texture_filepath, std::string filetype);

struct OpenGLTexture {
    std::string name;
    unsigned int address;
};
struct OpenGLTextureOptions{
    GLsizei width  = 256;
    GLsizei height = 256;
    /* border is always 0 so I don't bother adding it */
    GLenum internalFormat = GL_RGB8;
    GLenum format = GL_RGB;
    GLenum type = GL_UNSIGNED_BYTE;
    GLenum target = GL_TEXTURE_2D;
    GLint level = 0;
};

namespace defaults{
    const OpenGLTextureOptions options{};
};

namespace textureTypeMappings{                                                                                                  
    inline OpenGLTextureOptions map2D   (const float*,         const GLsizei width,  const GLsizei height){ return {width,     height, GL_R32F,    GL_RED,         GL_FLOAT,         GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const glm::vec2*,     const GLsizei width,  const GLsizei height){ return {width,     height, GL_RG32F,   GL_RG,          GL_FLOAT,         GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const glm::vec3*,     const GLsizei width,  const GLsizei height){ return {width,     height, GL_RGB32F,  GL_RGB,         GL_FLOAT,         GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const glm::vec4*,     const GLsizei width,  const GLsizei height){ return {width,     height, GL_RGBA32F, GL_RGBA,        GL_FLOAT,         GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const glm::mat2*,     const GLsizei width,  const GLsizei height){ return {width * 2, height, GL_RG32F,   GL_RG,          GL_FLOAT,         GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const glm::mat3*,     const GLsizei width,  const GLsizei height){ return {width * 3, height, GL_RGB32F,  GL_RGB,         GL_FLOAT,         GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const glm::mat4*,     const GLsizei width,  const GLsizei height){ return {width * 4, height, GL_RGBA32F, GL_RGBA,        GL_FLOAT,         GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const int*,           const GLsizei width,  const GLsizei height){ return {width,     height, GL_R32I,    GL_RED_INTEGER, GL_INT,           GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const short*,         const GLsizei width,  const GLsizei height){ return {width,     height, GL_R16I,    GL_RED_INTEGER, GL_INT,           GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const char*,          const GLsizei width,  const GLsizei height){ return {width,     height, GL_R8I,     GL_RED_INTEGER, GL_INT,           GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const unsigned int*,  const GLsizei width,  const GLsizei height){ return {width,     height, GL_R32UI,   GL_RED_INTEGER, GL_UNSIGNED_INT,  GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const unsigned short*,const GLsizei width,  const GLsizei height){ return {width,     height, GL_R16UI,   GL_RED_INTEGER, GL_UNSIGNED_INT,  GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions map2D   (const unsigned char*, const GLsizei width,  const GLsizei height){ return {width,     height, GL_R8UI,    GL_RED_INTEGER, GL_UNSIGNED_BYTE, GL_TEXTURE_2D, 0};};
    template<typename T> OpenGLTextureOptions map2D(const std::vector<T> v, const GLsizei width,  const GLsizei height) { return map2D(v.data(), width, height); }; 
    inline OpenGLTextureOptions png4Map                                  (const GLsizei width,  const GLsizei height){ return {width,     height, GL_RGBA8,   GL_RGBA,        GL_UNSIGNED_BYTE, GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions png3Map                                  (const GLsizei width,  const GLsizei height){ return {width,     height, GL_RGB8,    GL_RGB,         GL_UNSIGNED_BYTE, GL_TEXTURE_2D, 0};};
    inline OpenGLTextureOptions diffDimensions(const GLsizei width, const GLsizei height, const OpenGLTextureOptions options) { OpenGLTextureOptions optionsDiff{options}; optionsDiff.width = width; optionsDiff.height = height; return optionsDiff; };
};

namespace ttm = textureTypeMappings;
        
unsigned int useTextureDataToCreateTexture(const void* data, GLsizei width, GLsizei height, GLenum target = GL_TEXTURE_2D, GLint level = 0, GLint internalFormat = GL_RGBA,  GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
unsigned int useTextureDataToCreateTexture(const void* data, OpenGLTextureOptions options);
template <typename T> unsigned int useTextureDataToCreateTexture(T* data, GLsizei width, GLsizei height){
    return useTextureDataToCreateTexture(data, ttm::map2D(data, width, height));
}
template <typename T> unsigned int useTextureDataToCreateTexture(std::vector<T> data, GLsizei width, GLsizei height){
    return useTextureDataToCreateTexture(data.data(), ttm::map2D(data, width, height));
}
// #Classes -------------------------------------------------------------


class TextureAtlas{
    public:
        TextureAtlas(const OpenGLTextureOptions options, const unsigned int textureCnt, const unsigned int atlasCnt, const std::string name);
        unsigned int getTextureCnt    () const { return textureCnt;    }
        unsigned int getBackIndex     () const { return backIndex;      }
        unsigned int getShiftLeft     () const { return (textureCnt - 1) - backIndex;      }
        unsigned int getTextureAddress       (unsigned int index = 0) const { return textures[index].address; }
        const OpenGLTexture& getTexture       (unsigned int index=0) const { return textures[index]; }
        const std::vector<OpenGLTexture>& getTextures      () const { return textures; };
        const OpenGLTextureOptions getTextureOptions() const { return options; }
        void replaceTexture(const void* data);
        template <typename T> void replaceTexture(const std::vector<T> data, GLsizei width, GLsizei height);
        unsigned int getTexturesPerAtlas() const { return texturesPerAtlas; }
        unsigned int getAtlasCnt() const { return atlasCnt; }


    private:
        const OpenGLTextureOptions options;
        const unsigned int textureCnt;
        unsigned int backIndex;
        std::vector<OpenGLTexture> textures;
        const unsigned int atlasCnt;
        const unsigned int texturesPerAtlas;
};
bool operator==(const OpenGLTextureOptions& o1,const OpenGLTextureOptions& o2);

std::tuple<OpenGLTexture, OpenGLTextureOptions> loadTexture(std::string filePath, std::string name);
#endif
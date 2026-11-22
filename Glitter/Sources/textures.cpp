#include <textures.hpp>
unsigned char* initializePNGTextureData(int width, int height, int nrChannels){
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*nrChannels);
    return data;
}
void freePNGTextureData(unsigned char* data){
    free(data);
}
bool isOnBorder(int width, int height, int row, int col, int hThickness, int vThickness){
    return col < hThickness || col > width - (hThickness + 1) || row < vThickness || row > height - (vThickness + 1);
}
unsigned char getBorderCheckColor(int width, int height, int row, int col, int hThickness, int vThickness){
    if (isOnBorder(width, height, row, col, hThickness, vThickness)) return cdata::FULL_COLOR_D;
    else return cdata::EMPTY_COLOR_D; 

}
void generatePNGBorder(PngData& pngData, const int width, const int height,  const int hThickness, const int vThickness, const cdata::ColorRGBA borderColor, const cdata::ColorRGBA innerColor, const bool ignoreInner, const bool ignoreBorder){
    for(int gI = 0; gI < pngData.gridSize(); gI++){
        if (isOnBorder(width, height, pngData.rFG(gI), pngData.cFG(gI), hThickness, vThickness)){
            if(!ignoreBorder) pngData.setColor(gI, borderColor);
        }
        else{
            if(!ignoreInner) pngData.setColor(gI, innerColor);
        }
    }
}
void generatePNGBorder(unsigned char* data, int width, int height, int nrChannels, int hThickness, int vThickness){
    PngData pngData(width, height, nrChannels);
    generatePNGBorder(pngData, width, height, hThickness, vThickness, cdata::BLACK_D_RGBA, cdata::WHITE_D_RGBA, false, false);
    doCopy(data, pngData.getData(), pngData.size(), pngData.size());
}

void loadPngTexture(std::string texture_filepath, PngData& pD)
{
    do{
        pD = loadPngTexture(texture_filepath);
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    } while((!pD.isValid()));
}

PngData loadPngTexture(std::string texture_filepath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    PngData pngData(width, height, nrChannels, data);
    return pngData;
}
unsigned int obtainPNGBorder(int width, int height,  int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder){
    PngData pngData(width, height);
    generatePNGBorder(pngData, width, height,  hThickness, vThickness, borderColor, innerColor, ignoreInner, ignoreBorder);
    unsigned texture = createPngTexture(pngData);
    return texture;
}
unsigned int obtainPNGBlank(PngData& pngData){
    generatePNGBlank(pngData);
    unsigned texture = createPngTexture(pngData);
    return texture;
}
void generatePNGBlank(PngData& pngData){
    for(int i = 0; i < pngData.size(); i++){
        pngData.setData(i, cdata::FULL_COLOR_D);
    }
}
void generatePNGBlank(PngData& pngData, cdata::ColorRGBA color){
    for(int gI = 0; gI < pngData.gridSize(); gI++){
        pngData.setColor(gI, color);
    }
}
void generatePNGBlank(PngData& pngData, cdata::ColorRGB color){
    for(int gI = 0; gI < pngData.gridSize(); gI++){
        pngData.setColor(gI, color);
    }
}

unsigned int obtainPNGBorder(PngData &pngData, int hThickness, int vThickness, cdata::ColorRGBA borderColor, cdata::ColorRGBA innerColor, bool ignoreInner, bool ignoreBorder)
{
    generatePNGBorder(pngData, pngData.width, pngData.height,  hThickness, vThickness, borderColor, innerColor, ignoreInner, ignoreBorder);
    unsigned texture = createPngTexture(pngData);
    return texture;
}
cdata::ColorRGBA pngColorFromValue(const unsigned int &value)
{
    const cdata::ColorRGBA color(
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::redChannel   ) * sizeof(unsigned char)*BYTESIZE)),
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::greenChannel ) * sizeof(unsigned char)*BYTESIZE)),
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::blueChannel  ) * sizeof(unsigned char)*BYTESIZE)),
        (unsigned char) (value >> ((PngData::nrChannelsDefault - 1 - PngData::alphaChannel ) * sizeof(unsigned char)*BYTESIZE))
    );

    return color;
}
unsigned int valueFromPngColor(cdata::ColorRGBA color)
{
    unsigned int value = 0;
    value = ((color.red  )   << ((PngData::redChannel   ) *sizeof(unsigned char)*BYTESIZE))  |
            ((color.green)   << ((PngData::greenChannel ) *sizeof(unsigned char)*BYTESIZE))  |
            ((color.blue )   << ((PngData::blueChannel  ) *sizeof(unsigned char)*BYTESIZE))  |
            ((color.alpha)   << ((PngData::alphaChannel ) *sizeof(unsigned char)*BYTESIZE))  ;
    return value;
}
void generatePNGData(unsigned char *data, int width, int height, int nrChannels)
{
    std::cout << "W: " << width << ", H: " << height << ", N: " << nrChannels << std::endl;
    static const unsigned int size = 0xFF;
    std::cout << "S: " << size << std::endl;

    unsigned int stepsToIncrementRed;
    if (height % size != 0)
        stepsToIncrementRed = height/(size) + 1;
    else
        stepsToIncrementRed = height/(size);
    std::cout << "STIR: " << stepsToIncrementRed << std::endl;
    unsigned int stepsToIncrementAlpha;
    if (width % size != 0)
        stepsToIncrementAlpha = width/(size) + 1;
    else
        stepsToIncrementAlpha = width/(size);
    unsigned char red = 0;
    unsigned int other = 0;
    
    for(int row = 0; row < height; row+=1){
        if (row % stepsToIncrementRed == 0) red++;
        if (row % stepsToIncrementRed == 0) other++;
        unsigned char alpha = 0;
        for(int col = 0; col < width*nrChannels; col+=nrChannels){
            if(col % stepsToIncrementAlpha == 0) alpha++;

            for(int channel = 0; channel < nrChannels; channel++){
                if(channel == 0) // Red
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 1) // Green
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 2) // Blue
                    data[row*width*nrChannels+col+channel] = red;
                if(channel == 3) // Don't know
                    data[row*width*nrChannels+col+channel] = alpha;
            }
        }
    }
    std::cout << "O: " << other << std::endl;
}

unsigned int useTextureDataToCreateTexture(const void* data, OpenGLTextureOptions options){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(options.target, texture);
    // CHATGPT START
    int stores = 0;
    
    switch(options.format){
        case GL_RGBA:
        case GL_RGBA_INTEGER:
            stores = 4;
            break;
        case GL_RGB:
        case GL_RGB_INTEGER:
            stores = 1;
            break;
        case GL_RG:
        case GL_RG_INTEGER:
            stores = 1;
            break;
        case GL_RED:
        case GL_RED_INTEGER:
            stores = 1;
            break;
        default:
            std::cout << "SOMETHING WENT HORRIBLY WRONG AHHHHHH!!!\n" << std::endl;
            assert(false);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, stores);
    glTexParameteri(options.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(options.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(options.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(options.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // CHATGPT END
    
    glTexImage2D(options.target, options.level, options.internalFormat, options.width, options.height, 0 /* border (must be zero smh) */ , options.format, options.type, data);
    return texture;
}

unsigned int useTextureDataToCreateTexture(const void* data, GLsizei width, GLsizei height, GLenum target, GLint level, GLint internalFormat,  GLenum format, GLenum type){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(target, texture);
    // CHATGPT START
    int stores = 0;
    switch(format){
        case GL_RGBA:
        case GL_RGBA_INTEGER:
            stores = 4;
            break;
        case GL_RG:
        case GL_RG_INTEGER:
            stores = 2;
            break;
        case GL_RGB:
        case GL_RGB_INTEGER:
        case GL_RED:
        case GL_RED_INTEGER:
            stores = 1;
            break;
        default:
            std::cout << "SOMETHING WENT HORRIBLY WRONG AHHHHHH!!!\n" << std::endl;
            assert(false);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, stores);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // CHATGPT END
    glTexImage2D(target, level, internalFormat, width, height, 0 /* border (must be zero smh) */ , format, type, data);
    return texture;
}
unsigned int createPngTexture(std::string texture_filepath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    PngData pngData(width, height, nrChannels, data);
    return createPngTexture(pngData);
}
unsigned int createPngTexture(const PngData* pngData)
{
    unsigned int texture;
    if(pngData->nrChannels == 3)
        texture = useTextureDataToCreateTexture(pngData->getData(), ttm::png3Map(pngData->width, pngData->height));
    if(pngData->nrChannels == 4)
        texture = useTextureDataToCreateTexture(pngData->getData(), ttm::png4Map(pngData->width, pngData->height));
    return texture;
}
unsigned int createPngTexture(const PngData& pngData)
{
    return createPngTexture(&pngData);
}
unsigned int createTexture(std::string texture_filepath, std::string filetype)
{
    unsigned int texture;
    createTexture(texture, texture_filepath, filetype);
    return texture;
}

void createTexture(unsigned int &texture, std::string texture_filepath, std::string filetype){
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nrChannels, 0);
    glGenTextures(1, &texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    if (data){
        if(filetype == "jpg")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if(filetype == "png")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else {
            throw std::runtime_error("Bad file type: " + filetype);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        throw std::runtime_error("Failed to load texture: " + std::string(texture_filepath));
    }
    stbi_image_free(data);
}
// #PngData Class Functions ------------------------------------------------------------

PngData::PngData(const int width, const int height, const int nrChannels): width{width},  height{height}, nrChannels{nrChannels}{
    this->data = initializePNGTextureData(width, height, nrChannels);
}
PngData::PngData(int width, int height) : width{width},  height{height}, nrChannels{PngData::nrChannelsDefault}{
    this->data = initializePNGTextureData(width, height, nrChannels);
}
PngData::PngData(const int width, const int height, const int nrChannels, unsigned char *data, bool copy) : width{width},  height{height}, nrChannels{nrChannels}
{
    if(copy){
        this->data = copyInitialize(data, width*height*nrChannels);
    }
    else this->data = data;
}
PngData::PngData(const PngData &pngData) : width{pngData.width}, height{pngData.height}, nrChannels{pngData.nrChannels}
{
    this->data = copyInitialize(pngData.getData(), pngData.size());
}
PngData::~PngData()
{
    freePNGTextureData(this->data);
}

cdata::ColorRGB PngData::getColorRGB(int row, int col) const
{
    unsigned char red = this->getData(row, col, redChannel);
    unsigned char green = this->getData(row, col, greenChannel);
    unsigned char blue = this->getData(row, col, blueChannel);

    return cdata::ColorRGB(red, green, blue);
}
cdata::ColorRGBA PngData::getColorRGBA(int row, int col) const
{
    unsigned char red = this->getData(row, col, redChannel);
    unsigned char green = this->getData(row, col, greenChannel);
    unsigned char blue = this->getData(row, col, blueChannel);
    unsigned char alpha = this->getData(row, col, alphaChannel);

    return cdata::ColorRGBA(red, green, blue, alpha);
}
unsigned char PngData::getData(int row, int col, int channel) const
{
    return this->data[index(row, col, channel)];
}

void PngData::setData(int row, int col, int channel, const unsigned char &byte)
{
    this->data[index(row, col, channel)] = byte;
}
void pngDataSavePng(const std::string filepath, const PngData& pngData){
    stbi_write_png(filepath.c_str(), pngData.width, pngData.height, pngData.nrChannels, pngData.getData(), pngData.nrChannels * pngData.width);
}
TextureAtlas::TextureAtlas(const OpenGLTextureOptions options, const unsigned int textureCnt, const unsigned int atlasCnt, const std::string name) : options{options}, textureCnt{textureCnt}, backIndex{textureCnt - 1}, atlasCnt{atlasCnt}, texturesPerAtlas{(unsigned int)ceilWrap((int)textureCnt, (int)atlasCnt)}
{
    unsigned int atlasWidth = texturesPerAtlas*options.width;
    for(int i = 0; i < atlasCnt; i++){
        std::string atlasName = name + "[" + std::to_string(i) + "]";
        OpenGLTextureOptions atlasOptions = ttm::diffDimensions(atlasWidth,this->options.height,options);
        unsigned int atlasAddress = useTextureDataToCreateTexture(nullptr, atlasOptions);
        this->textures.push_back({atlasName, atlasAddress});
    }
}

void TextureAtlas::replaceTexture(const void *data)
{
    unsigned int atlasIndex = backIndex/texturesPerAtlas;
    unsigned int offsetIndex = (backIndex%texturesPerAtlas);
    glBindTexture(options.target, textures[atlasIndex].address);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glTexSubImage2D(
        options.target,
        options.level,
        offsetIndex * options.width,
        0,
        options.width,
        options.height,
        options.format,
        options.type,
        data
    );
    glGenerateMipmap(options.target);
    if(backIndex == 0)
        backIndex = textureCnt - 1;
    else
        backIndex -= 1;
}
bool operator==(const OpenGLTextureOptions& o1,const OpenGLTextureOptions& o2){
    if(o1.format != o2.format)
        return false;
    if(o1.height != o2.height)
        return false;
    if(o1.internalFormat != o2.internalFormat)
        return false;
    if(o1.level != o2.level)
        return false;
    if(o1.target != o2.target)
        return false;
    if(o1.type != o2.type)
        return false;
    if(o1.width != o2.width)
        return false;
    return true;
}
template <typename T> void TextureAtlas::replaceTexture(const std::vector<T> vec, GLsizei width, GLsizei height){
    assert(vec.size() >= width*height);
    const OpenGLTextureOptions vecOptions = ttm::map2D(vec, width*textureCnt, height);
    assert(vecOptions == options);
    replaceTexture(vec.data());
}
std::tuple<OpenGLTexture, OpenGLTextureOptions> loadTexture(std::string filePath, std::string name){

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    OpenGLTextureOptions options;
    options.target = GL_TEXTURE_2D;
    options.width = width;
    options.height = height;
    options.level = 0;
    options.type = GL_UNSIGNED_BYTE;
    switch(nrChannels){
        case 4:
            options.format = GL_RGBA;
            options.internalFormat = GL_RGBA8;
            break;
        case 3:
            options.format = GL_RGB;
            options.internalFormat = GL_RGB8;
            break;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(options.target, texture);
    // CHATGPT START
    int stores = 0;
    
    switch(options.format){
        case GL_RGBA:
        case GL_RGBA_INTEGER:
            stores = 4;
            break;
        case GL_RGB:
        case GL_RGB_INTEGER:
            stores = 1;
            break;
        case GL_RG:
        case GL_RG_INTEGER:
            stores = 1;
            break;
        case GL_RED:
        case GL_RED_INTEGER:
            stores = 1;
            break;
        default:
            std::cout << "SOMETHING WENT HORRIBLY WRONG AHHHHHH!!!\n" << std::endl;
            assert(false);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, stores);
    glTexParameteri(options.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(options.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(options.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(options.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // CHATGPT END
    
    glTexImage2D(options.target, options.level, options.internalFormat, options.width, options.height, 0 /* border (must be zero smh) */ , options.format, options.type, data);
    return {{name,texture},options};
}
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Application/stb_image.h"

std::map<std::string, Texture*> Texture::mTextureCache;

Texture* Texture::createTexture(const std::string& path, unsigned int unit) {
    //1检查是否缓存过本路径对应的纹理对象
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()){
    //对于iterater(迭代器），first-》 keysecond-》 value
        return iter->second;
    }
    //2 如果本路径对应的texture没有生成过，则重新生成
    auto texture = new Texture(path, unit) ;
    mTextureCache[path] = texture;

    return texture; 

}
Texture* Texture::createTextureFromMemory(
    const std::string& path,
    unsigned int unit,
    unsigned char* dataIn,
    uint32_t widthIn,
    uint32_t heightIn
) {
    //1检查是否缓存过本路径对应的纹理对象
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {
        //对于iterater(迭代器），first-》 keysecond-》 value
        return iter->second;
    }
    //2 如果本路径对应的texture没有生成过，则重新生成
    auto texture = new Texture(unit,dataIn, widthIn, heightIn); 
    mTextureCache[path] = texture;

    return texture;
}

Texture::Texture(const std::string& path, unsigned int unit)
{
    mUnit = unit;

    // 1 反转y轴
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), (int*)&mWidth, (int*)&mHeight, (int*)&mChannels, STBI_rgb_alpha);

    // 2 生成纹理并且激活单元0绑定 
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 传输纹理数据,开辟显存    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 4 释放数据
    stbi_image_free(data);

    // 5 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    // 6 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v
}

Texture::Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int unit)
{
    mUnit = unit;
    mWidth = width;
    mHeight = height;

    // 1 反转y轴
    stbi_set_flip_vertically_on_load(true);

    // 2 生成纹理并且激活单元0绑定 
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 传输纹理数据,开辟显存    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 4 释放数据
    stbi_image_free(data);

    // 5 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 6 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v
}

Texture::Texture(
    unsigned int unit,
    unsigned char* dataIn,
    uint32_t widthIn,
    uint32_t heightIn
) {
    mUnit = unit;

    // 1 反转y轴
    stbi_set_flip_vertically_on_load(true);

    uint32_t dataInSize = 0; 
    if (!heightIn) {
        dataInSize = widthIn;
    }
    else {
        dataInSize = widthIn * heightIn *4;
    }

    unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &mChannels, STBI_rgb_alpha);
   
    // 2 生成纹理并且激活单元0绑定 
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 传输纹理数据,开辟显存    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 4 释放数据
    stbi_image_free(data);

    // 5 设置纹理的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 6 设置纹理的包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit) {
    mWidth = width; 
    mHeight = height; 
    mUnit = unit;
    glGenTextures(1, & mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit); 
    glBindTexture(GL_TEXTURE_2D,mTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

Texture::Texture() {

}

Texture* Texture::createColorAttachment(
    unsigned int width,
    unsigned int height,
    unsigned int unit
) {
    return new Texture(width, height, unit);
}

Texture* Texture::createDepthStencilAttachment(
    unsigned int width,
    unsigned int height,
    unsigned int unit
) {
    Texture* dsTex = new Texture();

    unsigned int depthStencil;
    glGenTextures(1, &depthStencil);
    glBindTexture(GL_TEXTURE_2D, depthStencil);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    dsTex->mTexture = depthStencil; 
    dsTex->mWidth = width;
    dsTex->mHeight = height; 
    dsTex->mUnit = unit;
    return dsTex;

}

Texture::~Texture()
{
    if (mTexture != 0)
    {
        glDeleteTextures(1, &mTexture);
    }
}

void Texture::bind()
{
    // 先切换纹理单元，然后绑定texture对象
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}
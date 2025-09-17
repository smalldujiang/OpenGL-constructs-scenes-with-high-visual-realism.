#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Application/stb_image.h"

std::map<std::string, Texture*> Texture::mTextureCache;

Texture* Texture::createTexture(const std::string& path, unsigned int unit) {
    //1����Ƿ񻺴����·����Ӧ���������
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()){
    //����iterater(����������first-�� keysecond-�� value
        return iter->second;
    }
    //2 �����·����Ӧ��textureû�����ɹ�������������
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
    //1����Ƿ񻺴����·����Ӧ���������
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end()) {
        //����iterater(����������first-�� keysecond-�� value
        return iter->second;
    }
    //2 �����·����Ӧ��textureû�����ɹ�������������
    auto texture = new Texture(unit,dataIn, widthIn, heightIn); 
    mTextureCache[path] = texture;

    return texture;
}

Texture::Texture(const std::string& path, unsigned int unit)
{
    mUnit = unit;

    // 1 ��תy��
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), (int*)&mWidth, (int*)&mHeight, (int*)&mChannels, STBI_rgb_alpha);

    // 2 ���������Ҽ��Ԫ0�� 
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 ������������,�����Դ�    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 4 �ͷ�����
    stbi_image_free(data);

    // 5 ��������Ĺ��˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    // 6 ��������İ�����ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v
}

Texture::Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int unit)
{
    mUnit = unit;
    mWidth = width;
    mHeight = height;

    // 1 ��תy��
    stbi_set_flip_vertically_on_load(true);

    // 2 ���������Ҽ��Ԫ0�� 
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 ������������,�����Դ�    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 4 �ͷ�����
    stbi_image_free(data);

    // 5 ��������Ĺ��˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 6 ��������İ�����ʽ
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

    // 1 ��תy��
    stbi_set_flip_vertically_on_load(true);

    uint32_t dataInSize = 0; 
    if (!heightIn) {
        dataInSize = widthIn;
    }
    else {
        dataInSize = widthIn * heightIn *4;
    }

    unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &mChannels, STBI_rgb_alpha);
   
    // 2 ���������Ҽ��Ԫ0�� 
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3 ������������,�����Դ�    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 4 �ͷ�����
    stbi_image_free(data);

    // 5 ��������Ĺ��˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 6 ��������İ�����ʽ
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
    // ���л�����Ԫ��Ȼ���texture����
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}
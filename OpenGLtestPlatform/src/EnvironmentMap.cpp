#include "EnvironmentMap.h"
#include "stb_image/stb_image.h"

EnvironmentMap::EnvironmentMap(const std::string& path, const std::vector<std::string>& faces)
	:m_RendererID(0), m_faces(faces), m_path(path)
{
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
    stbi_set_flip_vertically_on_load(0);
	int Width, Height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load((path + faces[i]).c_str(), &Width, &Height, &nrChannels, 0);
        if (data)
        {

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            std::cout << "Cubemap texture Load Succeed: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << path << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    std::cout << "Load Environment Map Succeed" << std::endl;
}
EnvironmentMap::~EnvironmentMap()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void EnvironmentMap::Bind()const
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void EnvironmentMap::UnBind()const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
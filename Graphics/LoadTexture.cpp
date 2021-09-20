#include "LoadTexture.h"

#include <stb_image.h>

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger2.h"

#include <gli/texture2d.hpp>
#include <gli/load.hpp>
#include <gli/generate_mipmaps.hpp>


// only for 2d textures
RTexture2D* loadDdsTexture(char const* filename)
{
	gli::texture Texture = gli::load(filename);
	if (Texture.empty())
		return 0;

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum Target = GL.translate(Texture.target());

	GLuint TextureName = 0;
	glGenTextures(1, &TextureName);
	glBindTexture(Target, TextureName);
	glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);

	glm::tvec3<GLsizei> const Extent(Texture.extent());
	GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

	switch (Texture.target())
	{
	case gli::TARGET_1D:
		glTexStorage1D(
			Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
			Target, static_cast<GLint>(Texture.levels()), Format.Internal,
			Extent.x, Texture.target() == gli::TARGET_2D ? Extent.y : FaceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
			Target, static_cast<GLint>(Texture.levels()), Format.Internal,
			Extent.x, Extent.y,
			Texture.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
		break;
	default:
		assert(0);
		break;
	}

	for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
		for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
			for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
			{
				GLsizei const LayerGL = static_cast<GLsizei>(Layer);
				glm::tvec3<GLsizei> Extent(Texture.extent(Level));
				Target = gli::is_target_cube(Texture.target())
					? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
					: Target;

				switch (Texture.target())
				{
				case gli::TARGET_1D:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				default: assert(0); break;
				}
			}
	return new RTexture2D(filename, TextureName, glm::uvec2(0, 0), TFM_TRILINEAR, TFM_LINEAR, true);
;
}


const std::string HDR_FILE_EXTENSION = "hdr";
const std::string DDS_FILE_EXTENSION = "dds";


RTexture2D* loadTexture(const char* fileName, bool useCompression, bool mipmaping, RTexture2D* oldTexture)
{
	std::string fileNameStr(fileName);
	std::string extension = FilesHelper::getFileExtension(fileNameStr);
	bool hdrImage = extension == HDR_FILE_EXTENSION;

	LOG_INFO("Loading texture: " + fileNameStr);

	if (extension == DDS_FILE_EXTENSION)
	{
		return loadDdsTexture(fileName);
	}

	if (hdrImage)
		stbi_set_flip_vertically_on_load(true);
	else
		stbi_set_flip_vertically_on_load(false);

    int width, height, chanels;
	void* image;
	if (hdrImage)
		image = stbi_loadf(fileName, &width, &height, &chanels, 0);
	else
		image = stbi_load(fileName, &width, &height, &chanels, STBI_rgb_alpha);
    

    if(image == NULL)
    {
		LOG_ERROR("Failed to load texture: " + std::string(fileName));
        return NULL;
    }

    RTexture2D* texture;
    if (oldTexture == NULL)
    {
		if (hdrImage)
			texture = new RTexture2D(fileName, static_cast<float*>(image), chanels == 4 ? TF_RGBA_16F : TF_RGB_16F, glm::uvec2(width, height), true, useCompression);
		else
			texture = new RTexture2D(fileName, static_cast<unsigned char*>(image), TF_RGBA, glm::uvec2(width, height), true, useCompression);

		if (mipmaping)
		{
			texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
		}
		else
		{
			texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
		}

		if (hdrImage)
			texture->setClampMode(TCM_CLAMP_TO_EDGE);
		else
			texture->setClampMode(TCM_REPEAT);
    }
    else
    {
        texture = oldTexture;
		if (hdrImage)
			texture->setTexSubImage(static_cast<float*>(image), 0, 0, width, height);
		else
			texture->setTexSubImage(static_cast<unsigned char*>(image), 0, 0, width, height);
    }

    stbi_image_free(image);


    return texture;
}


RTextureCubeMap* loadTextureCubeMap(std::string* filesNames, const char* path, bool mipmaping, RTextureCubeMap* oldTexture)
{
	bool hdrImage = FilesHelper::getFileExtension(filesNames[0]) == HDR_FILE_EXTENSION;

    LOG_INFO("Loading cube map");

    stbi_set_flip_vertically_on_load(false);

    void* cubeMapFaces[6];
    int width, height, chanels;

    for (int i = 0; i < 6; ++i)
    {
		if (hdrImage)
			cubeMapFaces[i] = stbi_loadf(filesNames[i].c_str(), &width, &height, &chanels, 0);
		else
			cubeMapFaces[i] = stbi_load(filesNames[i].c_str(), &width, &height, &chanels, STBI_rgb_alpha);

        LOG_INFO("Loading texture: " + filesNames[i]);
        if(cubeMapFaces[i] == NULL)
        {
			LOG_ERROR("Failed to load texture: " + filesNames[i]);
            return NULL;
        }

        if (oldTexture != NULL)
        {
			if (hdrImage)
				oldTexture->setTexSubImage((float*)cubeMapFaces[i], (CubeMapFace)(CMF_POSITIVE_X + i), 0, 0, width, height);
			else
				oldTexture->setTexSubImage((unsigned char*)cubeMapFaces[i], (CubeMapFace)(CMF_POSITIVE_X + i), 0, 0, width, height);
        }
    }

    if (width != height)
        return NULL;

    RTextureCubeMap* texture;
	if (oldTexture == NULL)
	{
		if (hdrImage)
			texture = new RTextureCubeMap(path, (float**)cubeMapFaces, chanels == 4 ? TF_RGBA_16F : TF_RGB_16F, width, true);
		else
			texture = new RTextureCubeMap(path, (unsigned char**)cubeMapFaces, TF_RGBA, width, true);
	}
    else
        texture = oldTexture;

    if (mipmaping)
    {
        texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
    }
    else
    {
        texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    }

    texture->setClampMode(TCM_CLAMP_TO_EDGE);


    for (int i = 0; i < 6; ++i)
    {
        stbi_image_free(cubeMapFaces[i]);
    }


    return texture;
}

// Unused
GLuint loadTextureRect(const char* fileName)
{
    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);


    int width, height;
    unsigned char* image;// = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

	LOG_INFO("Loading texture: " + std::string(fileName));
    //std::cout << "SOIL result: " << SOIL_last_result() << std::endl;

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


    //SOIL_free_image_data(image);


    return texId;
}

#include "EnvironmentCaptureComponent.h"

#include "OGLDriver.h"
#include "Framebuffer.h"
#include "Prefab.h"
#include "RShader.h"

#include "../Game/GameConfig.h"

#include "../Scene/SceneObject.h"

#include "../Utils/ResourceManager.h"


EnvironmentCaptureComponent::EnvironmentCaptureComponent()
    : Component(CT_ENVIRONMENT_CAPTURE_COMPONENT),
    _environmentMap(NULL), _irradianceMap(NULL), _specularIrradianceMap(NULL),
    _rotationMatrix(1.0f),
	_irradianceFramebuffer(NULL), _irradianceShader(NULL), _cube(NULL), a(false)
{
	if (GameConfig::getInstance().pbrSupport)
	{
		initIrradianceFramebufferAndShader();
	}
}


EnvironmentCaptureComponent::EnvironmentCaptureComponent(RTextureCubeMap* environmentMap, RTextureCubeMap* irradianceMap, RTextureCubeMap* specularIrradianceMap)
    : Component(CT_ENVIRONMENT_CAPTURE_COMPONENT),
    _environmentMap(environmentMap), _irradianceMap(irradianceMap), _specularIrradianceMap(specularIrradianceMap),
    _rotationMatrix(1.0f),
	_irradianceFramebuffer(NULL), _irradianceShader(NULL), _cube(NULL), a(false)
{

	if (GameConfig::getInstance().pbrSupport)
	{
		initIrradianceFramebufferAndShader();
	}
}


EnvironmentCaptureComponent::~EnvironmentCaptureComponent()
{
	if (_irradianceFramebuffer != NULL)
	{
		OGLDriver::getInstance().deleteFramebuffer(_irradianceFramebuffer);
	}

	if (_prefilterEnvMapFramebuffer != NULL)
	{
		OGLDriver::getInstance().deleteFramebuffer(_prefilterEnvMapFramebuffer);
	}

	if (_cube != NULL)
	{
		delete _cube;
	}
}


void EnvironmentCaptureComponent::initIrradianceFramebufferAndShader()
{
	_environmentMap->setFiltering(TFM_TRILINEAR, TFM_LINEAR);

	_irradianceFramebuffer = OGLDriver::getInstance().createFramebuffer();
	_irradianceFramebuffer->addCubeMapTexture(TF_RGB_16F, 32);
	_irradianceFramebuffer->init();

	_irradianceShader = ResourceManager::getInstance().loadShader("Shaders/pbr/irradiance.vert", "Shaders/pbr/irradiance.frag");


	_prefilterEnvMapFramebuffer = OGLDriver::getInstance().createFramebuffer();
	_prefilterEnvMapFramebuffer->addCubeMapTexture(TF_RGBA_16F, 128, true);
	_prefilterEnvMapFramebuffer->init();

	_prefilterEnvMapShader = ResourceManager::getInstance().loadShader("Shaders/pbr/irradiance.vert", "Shaders/pbr/prefilterEnvironment.frag");


	_cube = new Cube(1, new Material);
	_cube->init();
}


void EnvironmentCaptureComponent::generateIrradianceMap()
{

	glm::mat4 projectionMatrix = glm::perspective(degToRad(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 viewMatrices[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};

	glDisable(GL_CULL_FACE);

	_irradianceShader->enable();
	_irradianceShader->setUniform(_irradianceShader->getUniformLocation("projectionMatrix"), projectionMatrix);
	_irradianceShader->bindTexture(_irradianceShader->getUniformLocation("Texture"), _environmentMap);

	_irradianceFramebuffer->bind();

	for (int i = 0; i < 6; ++i)
	{
		_irradianceShader->setUniform(_irradianceShader->getUniformLocation("viewMatrix"), viewMatrices[i]);

		_irradianceFramebuffer->bindCubeMapFaceToRender((CubeMapFace)(CMF_POSITIVE_X + i));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ModelNodeMesh* cubeMesh = _cube->getModelRootNode()->getMesh(0);
		cubeMesh->vbo->bind();
		cubeMesh->ibo->bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glDrawElements(GL_TRIANGLES,
			cubeMesh->indicesCount,
			GL_UNSIGNED_INT,
			(void*)(cubeMesh->firstVertex * sizeof(unsigned int)));

		glDisableVertexAttribArray(0);
	}

	glEnable(GL_CULL_FACE);

	_irradianceMap = static_cast<RTextureCubeMap*>(_irradianceFramebuffer->getTexture(0));
}


void EnvironmentCaptureComponent::generatePrefilteredEnvMap()
{
	const unsigned int MAX_MIPMAP_LEVELS = 4;

	glm::mat4 projectionMatrix = glm::perspective(degToRad(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 viewMatrices[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};

	glDisable(GL_CULL_FACE);

	_prefilterEnvMapShader->enable();
	_prefilterEnvMapShader->setUniform(_prefilterEnvMapShader->getUniformLocation("projectionMatrix"), projectionMatrix);
	_prefilterEnvMapShader->bindTexture(_prefilterEnvMapShader->getUniformLocation("Texture"), _environmentMap);

	_prefilterEnvMapFramebuffer->bind();

	for (int mipmap = 0; mipmap < MAX_MIPMAP_LEVELS; ++mipmap)
	{
		unsigned int mipWidth = 128 * std::pow(0.5, mipmap);
		unsigned int mipHeight = 128 * std::pow(0.5, mipmap);

		_prefilterEnvMapFramebuffer->setViewport(UintRect(0, 0, mipWidth, mipHeight));

		float roughness = (float)mipmap / float(MAX_MIPMAP_LEVELS - 1);
		_prefilterEnvMapShader->setUniform(_prefilterEnvMapShader->getUniformLocation("roughness"), roughness);

		for (int i = 0; i < 6; ++i)
		{
			_prefilterEnvMapShader->setUniform(_prefilterEnvMapShader->getUniformLocation("viewMatrix"), viewMatrices[i]);

			_prefilterEnvMapFramebuffer->bindCubeMapFaceToRender((CubeMapFace)(CMF_POSITIVE_X + i), 0, mipmap);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			ModelNodeMesh * cubeMesh = _cube->getModelRootNode()->getMesh(0);
			cubeMesh->vbo->bind();
			cubeMesh->ibo->bind();

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

			glDrawElements(GL_TRIANGLES,
				cubeMesh->indicesCount,
				GL_UNSIGNED_INT,
				(void*)(cubeMesh->firstVertex * sizeof(unsigned int)));

			glDisableVertexAttribArray(0);
		}
	}

	glEnable(GL_CULL_FACE);

	_specularIrradianceMap = static_cast<RTextureCubeMap*>(_prefilterEnvMapFramebuffer->getTexture(0));
}


RTextureCubeMap* EnvironmentCaptureComponent::getEnvironmentMap()
{
    return _environmentMap;
}


void EnvironmentCaptureComponent::generateRequiredPbrMaps()
{
	if (GameConfig::getInstance().pbrSupport)
	{
		if (_irradianceMap == NULL)
			generateIrradianceMap();

		if (_specularIrradianceMap == NULL)
			generatePrefilteredEnvMap();
	}

	if (_irradianceMap == NULL)
		_irradianceMap = _environmentMap;

	if (_specularIrradianceMap == NULL)
		_specularIrradianceMap = _environmentMap;
}


RTextureCubeMap* EnvironmentCaptureComponent::getIrradianceMap()
{
	return _irradianceMap;
}


RTextureCubeMap* EnvironmentCaptureComponent::getSpecularIrradianceMap()
{
	return _specularIrradianceMap;
}


glm::mat4 EnvironmentCaptureComponent::getRotationMatrix()
{
    return _rotationMatrix;
}


void EnvironmentCaptureComponent::changedTransform()
{
    _rotationMatrix = glm::inverse(glm::mat4_cast(_object->getRotationQuaternion()));
}

#include "PostProcessingBloom.h"


#include "../Utils/Strings.h"
#include "../Utils/ResourceManager.h"


PostProcessingBloom::PostProcessingBloom(VBO* quadVbo, float screenWidth, float screenHeight, RTexture* brightnessTexture,
										 bool mssaaAntialiasingEnabled, int msaaAntialiasingLevel)
	: PostProcessingEffect(PPT_BLOOM, quadVbo),
	_brightnessTexture(brightnessTexture)
{
	_shader = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingBloom.frag");


	std::vector<std::string> defines;
	std::unordered_map<std::string, std::string> constants;

	// BLUR_SHADER_MSAA
	defines.clear();
	defines.push_back(mssaaAntialiasingEnabled ? "MULTISAMPLE" : "NOT_MULTISAMPLE");
	constants.clear();
	constants["samplesCount"] = toString(msaaAntialiasingLevel);
	_blurShaderMsaa = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/blur.frag", defines, constants);

	// BLUR_SHADER
	defines.clear();
	defines.push_back("NOT_MULTISAMPLE");
	constants.clear();
	constants["samplesCount"] = toString(msaaAntialiasingLevel);
	_blurShader = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/blur.frag", defines, constants);


	for (int i = 0; i < 2; ++i)
	{
		_blurFramebuffers[i] = OGLDriver::getInstance().createFramebuffer();
		_blurFramebuffers[i]->addTexture(TF_RGBA_32F, screenWidth / 10.0f, screenHeight / 10.0f, false);
		_blurFramebuffers[i]->getTexture(0)->setFiltering(TFM_LINEAR, TFM_LINEAR);
		_blurFramebuffers[i]->setViewport(UintRect(0, 0, screenWidth / 10.0f, screenHeight / 10.0f));
	}

	addAdditionalInput(_blurFramebuffers[0]->getTexture(0));
}


PostProcessingBloom::~PostProcessingBloom()
{
	OGLDriver::getInstance().deleteFramebuffer(_blurFramebuffers[0]);
	OGLDriver::getInstance().deleteFramebuffer(_blurFramebuffers[1]);
}


void PostProcessingBloom::process()
{
	bool isHorizontal = true;
	bool isFirstIteration = true;
	int amount = 10;
	for (unsigned int i = 0; i < amount; i++)
	{
		_blurFramebuffers[isHorizontal]->bind();
		glClear(GL_COLOR_BUFFER_BIT);

		RShader* shader;
		if (isFirstIteration)
			shader = _blurShaderMsaa;
		else
			shader = _blurShader;

		shader->enable();

		shader->setUniform(UNIFORM_BLUR_IS_HORIZONTAL, isHorizontal);

		RTexture* texture = isFirstIteration ? _brightnessTexture : _blurFramebuffers[!isHorizontal]->getTexture(0);
		shader->bindTexture(UNIFORM_DIFFUSE_TEXTURE, texture);


		_quadVbo->bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(0);


		isHorizontal = !isHorizontal;
		if (isFirstIteration)
			isFirstIteration = false;
	}
}


void PostProcessingBloom::setParamUniforms()
{

}

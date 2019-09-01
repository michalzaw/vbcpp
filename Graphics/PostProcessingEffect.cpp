#include "PostProcessingEffect.h"


PostProcessingEffect::PostProcessingEffect(VBO* quadVbo, RShader* shader)
	: _quadVbo(quadVbo), _shader(shader),
	_mainInputTexture(nullptr), _outputFramebuffer(nullptr)
{

}


void PostProcessingEffect::process()
{

}


void PostProcessingEffect::setParamUniforms()
{

}


void PostProcessingEffect::draw()
{
	_outputFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_quadVbo->bind();

	_shader->enable();
	_shader->bindTexture(UNIFORM_POSTPROCESS_TEXTURE_1, _mainInputTexture);

	for (int i = 0; i < _additionalInputTextures.size(); ++i)
	{
		_shader->bindTexture((UniformName)(UNIFORM_POSTPROCESS_TEXTURE_2 + i), _additionalInputTextures[i]);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
}


void PostProcessingEffect::addAdditionalInput(RTexture* texture)
{
	_additionalInputTextures.push_back(texture);
}


void PostProcessingEffect::run(RTexture* mainInputTexture, Framebuffer* outputFramebuffer)
{
	_mainInputTexture = mainInputTexture;
	_outputFramebuffer = outputFramebuffer;

	process();
	draw();
}

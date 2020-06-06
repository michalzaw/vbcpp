#include "PostProcessingEffect.h"


PostProcessingEffect::PostProcessingEffect(PostProcessingType type, VBO* quadVbo, RShader* shader)
	: _type(type), _quadVbo(quadVbo), _shader(shader),
	_mainInputTexture(nullptr), _outputFramebuffer(nullptr)
{

}


void PostProcessingEffect::setShader(RShader* shader)
{
	_shader = shader;

	_textureUniformLocations[0] = _shader->getUniformLocation("texture1");
	_textureUniformLocations[1] = _shader->getUniformLocation("texture2");
	_textureUniformLocations[2] = _shader->getUniformLocation("texture3");
	_textureUniformLocations[3] = _shader->getUniformLocation("texture4");
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

	setParamUniforms();

	_shader->bindTexture(_textureUniformLocations[0], _mainInputTexture);

	for (int i = 0; i < _additionalInputTextures.size(); ++i)
	{
		_shader->bindTexture(_textureUniformLocations[1 + i], _additionalInputTextures[i]);
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


PostProcessingType PostProcessingEffect::getType()
{
	return _type;
}


void PostProcessingEffect::run(RTexture* mainInputTexture, Framebuffer* outputFramebuffer)
{
	_mainInputTexture = mainInputTexture;
	_outputFramebuffer = outputFramebuffer;

	process();
	draw();
}

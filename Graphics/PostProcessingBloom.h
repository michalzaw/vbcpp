#ifndef POSTPROCESSINGBLOOM_H_INCLUDED
#define POSTPROCESSINGBLOOM_H_INCLUDED


#include "PostProcessingEffect.h"


class PostProcessingBloom : public PostProcessingEffect
{
	public:
		RShader* _blurShaderMsaa;
		RShader* _blurShader;

		Framebuffer* _blurFramebuffers[2];

		RTexture* _brightnessTexture;

		GLint _isHorizontalUniformLocation;
		GLint _textureBlurUniformLocation;

		void process() override;
		void setParamUniforms() override;

	public:
		PostProcessingBloom(VBO* quadVbo, float screenWidth, float screenHeight, RTexture* brightnessTexture,
							bool mssaaAntialiasingEnabled = true, int msaaAntialiasingLevel = 4);
		~PostProcessingBloom();

};


#endif // POSTPROCESSINGBLOOM_H_INCLUDED

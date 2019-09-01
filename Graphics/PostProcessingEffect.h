#ifndef POSTPROCESSINGEFFECT_H_INCLUDED
#define POSTPROCESSINGEFFECT_H_INCLUDED


#include <vector>

#include "VBO.h"
#include "RShader.h"
#include "Framebuffer.h"


class PostProcessingEffect
{
	private:
		void draw();

	protected:
		VBO* _quadVbo;
		RShader* _shader;

		RTexture* _mainInputTexture;
		Framebuffer* _outputFramebuffer;
		std::vector<RTexture*> _additionalInputTextures;

		virtual void process();
		virtual void setParamUniforms();

	public:
		PostProcessingEffect(VBO* quadVbo, RShader* shader);

		void addAdditionalInput(RTexture* texture);

		void run(RTexture* mainInputTexture, Framebuffer* outputFramebuffer);

};


#endif // POSTPROCESSINGEFFECT_H_INCLUDED

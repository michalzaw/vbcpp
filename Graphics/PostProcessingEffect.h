#ifndef POSTPROCESSINGEFFECT_H_INCLUDED
#define POSTPROCESSINGEFFECT_H_INCLUDED


#include <vector>

#include "VBO.h"
#include "RShader.h"
#include "Framebuffer.h"


enum PostProcessingType
{
	PPT_MSAA,
	PPT_BLOOM,
	PPT_TONE_MAPPING,

	POST_PROCESSING_EFFECTS_COUNT
};


class PostProcessingEffect
{
	private:
		PostProcessingType _type;

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
		PostProcessingEffect(PostProcessingType type, VBO* quadVbo, RShader* shader = nullptr);
		virtual ~PostProcessingEffect() = default;

		void addAdditionalInput(RTexture* texture);

		PostProcessingType getType();

		void run(RTexture* mainInputTexture, Framebuffer* outputFramebuffer);

};


#endif // POSTPROCESSINGEFFECT_H_INCLUDED

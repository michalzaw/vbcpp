#ifndef POSTPROCESSINGTONEMAPPING_H_INCLUDED
#define POSTPROCESSINGTONEMAPPING_H_INCLUDED


#include "PostProcessingEffect.h"


class PostProcessingToneMapping : public PostProcessingEffect
{
	private:
		float _exposure;

	protected:
		void setParamUniforms() override
		{
			_shader->setUniform(UNIFORM_TONEMAPPING_EXPOSURE, _exposure);
		}

	public:
		PostProcessingToneMapping(VBO* quadVbo, RShader* shader)
			: PostProcessingEffect(PPT_TONE_MAPPING, quadVbo, shader),
			_exposure(1.87022f)
		{

		}

		void setExposure(float exposure)
		{
			_exposure = exposure;
		}

		float getExposure()
		{
			return _exposure;
		}

};


#endif // POSTPROCESSINGTONEMAPPING_H_INCLUDED

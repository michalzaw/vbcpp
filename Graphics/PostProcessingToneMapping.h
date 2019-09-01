#ifndef POSTPROCESSINGTONEMAPPING_H_INCLUDED
#define POSTPROCESSINGTONEMAPPING_H_INCLUDED


#include "PostProcessingEffect.h"

#include "../Utils/ResourceManager.h"


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
		PostProcessingToneMapping(VBO* quadVbo)
			: PostProcessingEffect(PPT_TONE_MAPPING, quadVbo),
			_exposure(1.87022f)
		{
			_shader = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingToneMapping.frag");
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

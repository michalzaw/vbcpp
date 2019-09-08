#ifndef POSTPROCESSINGTONEMAPPING_H_INCLUDED
#define POSTPROCESSINGTONEMAPPING_H_INCLUDED


#include "PostProcessingEffect.h"

#include "../Utils/ResourceManager.h"


class PostProcessingToneMapping : public PostProcessingEffect
{
	private:
		GLint _exposureUniformLocation;

		float _exposure;

	protected:
		void setParamUniforms() override
		{
			_shader->setUniform(_exposureUniformLocation, _exposure);
		}

	public:
		PostProcessingToneMapping(VBO* quadVbo)
			: PostProcessingEffect(PPT_TONE_MAPPING, quadVbo),
			_exposure(1.87022f)
		{
			setShader(ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingToneMapping.frag"));

			_exposureUniformLocation = _shader->getUniformLocation("exposure");
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

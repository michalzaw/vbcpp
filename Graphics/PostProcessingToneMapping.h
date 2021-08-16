#ifndef POSTPROCESSINGTONEMAPPING_H_INCLUDED
#define POSTPROCESSINGTONEMAPPING_H_INCLUDED


#include "PostProcessingEffect.h"

#include "../Utils/ResourceManager.h"


enum ToneMappingType
{
	TMT_REINHARD,
	TMT_CLASSIC,
	TMT_ACES
};


class PostProcessingToneMapping : public PostProcessingEffect
{
	private:
		ToneMappingType _toneMappingType;

		GLint _exposureUniformLocation;
		GLint _visibilityUniformLocation;
		float _exposure;
		float _visibility;

		RShader* loadShader()
		{
			std::vector<std::string> defines;
			switch (_toneMappingType)
			{
				case TMT_REINHARD:
					defines.push_back("REINHARD");
					break;
					
				case TMT_CLASSIC:
					defines.push_back("CLASSIC");
					break;

				case TMT_ACES:
					defines.push_back("ACES");
					break;
			}

			return ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingToneMapping.frag", defines);
		}

		void initUniformLocations()
		{
			_exposureUniformLocation = _shader->getUniformLocation("exposure");
			_visibilityUniformLocation = _shader->getUniformLocation("visibility");
		}

	protected:

		void setParamUniforms() override
		{
			_shader->setUniform(_exposureUniformLocation, _exposure);
			_shader->setUniform(_visibilityUniformLocation, _visibility);
		}

	public:
		PostProcessingToneMapping(VBO* quadVbo, ToneMappingType toneMappingType = TMT_CLASSIC)
			: PostProcessingEffect(PPT_TONE_MAPPING, quadVbo),
			_toneMappingType(toneMappingType), _exposure(1.87022f), _visibility(1.0f)
		{
			setShader(loadShader());
			initUniformLocations();
		}

		void setExposure(float exposure)
		{
			_exposure = exposure;
		}

		float getExposure()
		{
			return _exposure;
		}

		void setVisibility(float visibility)
		{
			_visibility = visibility;
		}

		float getVisibility()
		{
			return _visibility;
		}

		void setToneMappingType(ToneMappingType toneMappingType)
		{
			_toneMappingType = toneMappingType;

			setShader(loadShader());
			initUniformLocations();
		}

		ToneMappingType getToneMappingType()
		{
			return _toneMappingType;
		}

};


#endif // POSTPROCESSINGTONEMAPPING_H_INCLUDED

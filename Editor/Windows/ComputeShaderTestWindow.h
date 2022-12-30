#ifndef COMPUTESHADERTESTWINDOW_H_INCLUDED
#define COMPUTESHADERTESTWINDOW_H_INCLUDED


#include "../../ImGuiInterface/ImGuiWindow.h"

#include "../../Graphics/ShaderStorageBuffer.h"


class RShader;
class RTexture;


class ComputeShaderTestWindow final : public ImGuiWindow
{
	private:
		RShader* _computeShader;
		RTexture* _texture;

		ShaderStorageBuffer* _shaderStorageBuffer;

		virtual void drawWindow() override;

	public:
		explicit ComputeShaderTestWindow(bool isOpen = false);

};


#endif // COMPUTESHADERTESTWINDOW_H_INCLUDED
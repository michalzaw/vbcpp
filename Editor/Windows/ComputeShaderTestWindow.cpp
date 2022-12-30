#include "ComputeShaderTestWindow.h"


ComputeShaderTestWindow::ComputeShaderTestWindow(bool isOpen/* = false*/)
	: ImGuiWindow(nullptr, isOpen)
{
	_computeShader = ResourceManager::getInstance().loadComputeShader("Shaders/computeShader.comp");

	_texture = new RTexture2D("aaaa", (float*) nullptr, TF_RGBA_32F, glm::uvec2(512, 512), false);
	_texture->setClampMode(TCM_CLAMP_TO_EDGE);
	_texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
}


void ComputeShaderTestWindow::drawWindow()
{
	if (ImGui::Begin("Compute shader test", &_isOpen))
	{
		if (ImGui::Button("Run compute shader"))
		{
			_computeShader->enable();
			_computeShader->bindTextureImage(_texture, 0, 0, TIBM_READ_AND_WRITE);
			_computeShader->runComputeShader(512, 512, 1);
		}

		ImGui::Image((ImTextureID) _texture->getID(), ImVec2(_texture->getSize().x, _texture->getSize().y));
	}
	ImGui::End();
}

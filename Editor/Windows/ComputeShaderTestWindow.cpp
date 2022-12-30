#include "ComputeShaderTestWindow.h"

#include "../../Graphics/OGLDriver.h"


struct ShaderData
{
	float t;
	float speed;
	float width;

	ShaderData()
		: t(5.0f), speed(100.0f), width(1000.0f)
	{

	}
};


ShaderData shaderData;


ComputeShaderTestWindow::ComputeShaderTestWindow(bool isOpen/* = false*/)
	: ImGuiWindow(nullptr, isOpen)
{
	_computeShader = ResourceManager::getInstance().loadComputeShader("Shaders/compute/computeShader.comp");

	_texture = new RTexture2D("ComputeShaderResult", (float*) nullptr, TF_RGBA_32F, glm::uvec2(1000, 1000), false);
	_texture->setClampMode(TCM_CLAMP_TO_EDGE);
	_texture->setFiltering(TFM_LINEAR, TFM_LINEAR);

	_shaderStorageBuffer = OGLDriver::getInstance().createShaderStorageBuffesr(sizeof(ShaderData));

	_computeShader->setShaderStorageBlockBinding("ShaderData", 0);
	_shaderStorageBuffer->bindBufferBase(0);
	_shaderStorageBuffer->setData(shaderData);
}


void ComputeShaderTestWindow::drawWindow()
{
	if (ImGui::Begin("Compute shader test", &_isOpen))
	{
		bool dataChanged = false;
		if (ImGui::DragFloat("Time", &shaderData.t, 0.01f, 0.0f, 100.0f))
		{
			dataChanged = true;
		}
		if (ImGui::DragFloat("Speed", &shaderData.speed, 1.0f, 1.0f, 1000.0f))
		{
			dataChanged = true;
		}

		if (dataChanged)
		{
			_shaderStorageBuffer->setData(shaderData);
		}

		static glm::ivec3 numGroups = glm::ivec3(_texture->getSize().x / 10, _texture->getSize().y / 10, 1);
		ImGui::DragInt3("Number of groups", &numGroups[0]);

		static bool runComputeShader = false;
		ImGui::Checkbox("Run compute shader (loop)", &runComputeShader);

		if (ImGui::Button("Run compute shader") || runComputeShader)
		{
			_computeShader->enable();
			_computeShader->bindTextureImage(_texture, 0, 0, TIBM_READ_AND_WRITE);
			//_computeShader->setUniform(_computeShader->getUniformLocation("t"), t);
			_computeShader->runComputeShader(numGroups.x, numGroups.y, numGroups.z);
		}

		ImGui::Image((ImTextureID) _texture->getID(), ImVec2(_texture->getSize().x, _texture->getSize().y));
	}
	ImGui::End();
}

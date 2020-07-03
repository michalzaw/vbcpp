#ifndef AXISTOOL_H_INCLUDED
#define AXISTOOL_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>


namespace AxisTool
{
	// call BeginFrame right after ImGui_XXXX_NewFrame();
	void BeginFrame();

	void SetRect(float x, float y, float width, float height);

	void Show(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraDirection);
};


#endif // AXISTOOL_H_INCLUDED

#include "PhysicsDebugRenderer.h"

#include "Renderer.h"
#include "VBO.h"

#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"


PhysicsDebugRenderer::PhysicsDebugRenderer()
	: _numberOfVerticesToRendering(0),
	_clearRenderTargetBeforeRendering(true)
{
	_drawMode = btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints + btIDebugDraw::DBG_DrawConstraints;

	_shader = ResourceManager::getInstance().loadShader("Shaders/debugPhysics.vert", "Shaders/debugPhysics.frag");

	_vbo = OGLDriver::getInstance().createVBO(MAX_VERICES_COUNT * sizeof(DebugVertex), GL_DYNAMIC_DRAW);

	_vertices.push_back(DebugVertex{ glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) });
	_vbo->addVertexData(&_vertices[0], 1);
	_vertices.clear();

	_targetFramebuffer = OGLDriver::getInstance().getDefaultFramebuffer();
}


PhysicsDebugRenderer::~PhysicsDebugRenderer()
{

}


btIDebugDraw::DefaultColors PhysicsDebugRenderer::getDefaultColors() const
{
	return _defaultsColors;
}


void PhysicsDebugRenderer::setDefaultColors(const btIDebugDraw::DefaultColors& colors)
{
	_defaultsColors = colors;
}


void PhysicsDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	_vertices.push_back(DebugVertex{ glm::vec3(from.x(), from.y(), from.z()), glm::vec3(color.x(), color.y(), color.z()) });
	_vertices.push_back(DebugVertex{ glm::vec3(to.x(), to.y(), to.z()), glm::vec3(color.x(), color.y(), color.z()) });
}


void PhysicsDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
	_vertices.push_back(DebugVertex{ glm::vec3(from.x(), from.y(), from.z()), glm::vec3(fromColor.x(), fromColor.y(), fromColor.z()) });
	_vertices.push_back(DebugVertex{ glm::vec3(to.x(), to.y(), to.z()), glm::vec3(toColor.x(), toColor.y(), toColor.z()) });
}


void PhysicsDebugRenderer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	drawSphere(pointOnB, 0.01f, color);
	drawLine(pointOnB, pointOnB + normalOnB, color);
}


void PhysicsDebugRenderer::reportErrorWarning(const char* warningString)
{
	Logger::error("[Physics Debugger] - " + std::string(warningString));
}


void PhysicsDebugRenderer::draw3dText(const btVector3& location, const char* textString)
{
	
}


void PhysicsDebugRenderer::setDebugMode(int debugMode)
{
	_drawMode = debugMode;
}


int PhysicsDebugRenderer::getDebugMode() const
{
	return _drawMode;
}


void PhysicsDebugRenderer::flushLines()
{
	_numberOfVerticesToRendering = _vertices.size();
	int vertexSize = sizeof(DebugVertex);

	if (_vbo->getBufferSize() < _numberOfVerticesToRendering * vertexSize)
	{
		_numberOfVerticesToRendering = _vbo->getBufferSize() / vertexSize;
	}

	_vbo->updateVertexData(&(_vertices[0]), _numberOfVerticesToRendering);

	_vertices.clear();
}


void PhysicsDebugRenderer::setTargetFramebuffer(Framebuffer* framebuffer)
{
	_targetFramebuffer = framebuffer;
}


Framebuffer* PhysicsDebugRenderer::getTargetFramebuffer()
{
	return _targetFramebuffer;
}


void PhysicsDebugRenderer::setClearRenderTargetBeforeRenderingFlag(bool enabled)
{
	_clearRenderTargetBeforeRendering = enabled;
}


bool PhysicsDebugRenderer::getClearRenderTargetBeforeRenderingFlag()
{
	return _clearRenderTargetBeforeRendering;
}


void PhysicsDebugRenderer::renderAll()
{
	_targetFramebuffer->bind();

	if (_clearRenderTargetBeforeRendering)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glPointSize(50);
	glLineWidth(2);

	_vbo->bind();
	_shader->enable();

	CameraStatic* camera = Renderer::getInstance().getCurrentMainCamera();
	glm::mat4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix();

	_shader->setUniform(_shader->getUniformLocation("MVP"), MVP);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)(sizeof(float) * 3));

	glDrawArrays(GL_LINES, 0, _numberOfVerticesToRendering);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

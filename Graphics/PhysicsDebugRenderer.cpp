#include "PhysicsDebugRenderer.h"

#include <iostream>

#include "GraphicsManager.h"
#include "VBO.h"

#include "../Utils/ResourceManager.h"


PhysicsDebugRenderer::PhysicsDebugRenderer()
{
	_drawMode = btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints + btIDebugDraw::DBG_DrawConstraints;

	_shader = ResourceManager::getInstance().loadShader("Shaders/debugPhysics.vert", "Shaders/debugPhysics.frag");

	_vbo = OGLDriver::getInstance().createVBO(100000 * sizeof(DebugVertex));

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


void PhysicsDebugRenderer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}


void PhysicsDebugRenderer::reportErrorWarning(const char* warningString)
{

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
	int numberOfVertices = _vertices.size();
	int vertexSize = sizeof(DebugVertex);

	if (_vbo->getBufferSize() < numberOfVertices * vertexSize)
	{
		numberOfVertices = _vbo->getBufferSize() / vertexSize;
	}

	_vbo->updateVertexData(&(_vertices[0]), numberOfVertices);

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


void PhysicsDebugRenderer::renderAll(bool clearRenderTargetBeforeRendering)
{
	_targetFramebuffer->bind();

	if (clearRenderTargetBeforeRendering)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glPointSize(50);
	glLineWidth(2);

	_vbo->bind();
	_shader->enable();

	CameraStatic* camera = GraphicsManager::getInstance().getCurrentCamera();
	glm::mat4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix();

	_shader->setUniform(_shader->getUniformLocation("MVP"), MVP);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)(sizeof(float) * 3));

	glDrawArrays(GL_LINES, 0, _vbo->getQuantumOfVertices());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

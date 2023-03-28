#ifndef PHYSICSDEBUGRENDERER_H_INCLUDED
#define PHYSICSDEBUGRENDERER_H_INCLUDED


#include <vector>

#include <LinearMath/btIDebugDraw.h>

#include <glm/glm.hpp>


class RShader;
class VBO;
class Framebuffer;


struct DebugVertex
{
	glm::vec3 position;
	glm::vec3 color;
};


class PhysicsDebugRenderer : public btIDebugDraw
{
	private:
		static const int MAX_VERICES_COUNT = 1000000;

		DefaultColors _defaultsColors;
		int _drawMode;

		std::vector<DebugVertex> _vertices;
		int _numberOfVerticesToRendering;

		RShader* _shader;
		VBO* _vbo;

		Framebuffer* _targetFramebuffer;

		bool _clearRenderTargetBeforeRendering;

	public:
		PhysicsDebugRenderer();
		~PhysicsDebugRenderer();

		DefaultColors getDefaultColors() const override;
		void setDefaultColors(const DefaultColors& colors) override;

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override;
		void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
		void reportErrorWarning(const char* warningString) override;
		void draw3dText(const btVector3& location, const char* textString) override;

		void setDebugMode(int debugMode) override;
		int getDebugMode() const override;

		void flushLines() override;

		void setTargetFramebuffer(Framebuffer* framebuffer);
		Framebuffer* getTargetFramebuffer();
		void setClearRenderTargetBeforeRenderingFlag(bool enabled);
		bool getClearRenderTargetBeforeRenderingFlag();

		void renderAll();
};


#endif // PHYSICSDEBUGRENDERER_H_INCLUDED

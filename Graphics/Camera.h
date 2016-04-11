#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


class CMatrixCamera
{
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::mat4 m_ViewInvMatrix;
		glm::mat4 m_ProjectionInvMatrix;
		glm::mat4 m_ViewProjectionInvMatrix;

		bool m_ViewProjectionMatrixIs;
		bool m_ViewInvMatrixIs;
		bool m_ProjectionInvMatrixIs;
		bool m_ViewProjectionInvMatrixIs;

		void Changed();

	public:
		CMatrixCamera();
		CMatrixCamera(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix);
		~CMatrixCamera();


		void SetViewMatrix(glm::mat4& ViewMatrix);
		void SetProjectionMatrix(glm::mat4& ProjectionMatrix);
		void SetMatrices(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix);

		glm::mat4& GetViewMatrix();
		glm::mat4& GetProjectionMatrix();
		glm::mat4& GetViewProjectionMatrix();
		glm::mat4& GetViewInvMatrix();
		glm::mat4& GetProjectionInvMatrix();
		glm::mat4& GetViewProjectionInvMatrix();

};


class CParamsCamera
{
	private:
		glm::vec3 m_EyePosition;
		glm::vec3 m_ForwardVector;
		glm::vec3 m_UpVector;

		float m_FieldOfView;
		float m_Aspect;
		float m_zNear;
		float m_zFar;


		glm::vec3 m_RightVector;
		glm::vec3 m_RealUpVector;

		CMatrixCamera m_Matrices;

		bool m_RightVectorIs;
		bool m_RealUpVectorIs;
		bool m_MatricesIs;


		void Changed();

	public:
		CParamsCamera();
		CParamsCamera(glm::vec3 EyePosition, glm::vec3 ForwardVector, glm::vec3 UpVector,
					  float FieldOfView, float Aspect, float zNear, float zFar);
		~CParamsCamera();


		void SetEyePosition(glm::vec3 EyePosition);
		void SetForwardVector(glm::vec3 ForwardVector);
		void SetUpVector(glm::vec3 UpVector);

		void SetFieldOfView(float FieldOfView);
		void SetAspect(float Aspect);
		void SetZNear(float zNear);
		void SetZFar(float zFar);

		void SetParams(glm::vec3 EyePosition, glm::vec3 ForwardVector, glm::vec3 UpVector,
					   float FieldOfView, float Aspect, float zNear, float zFar);


		glm::vec3 GetEyePosition();
		glm::vec3 GetForwardVector();
		glm::vec3 GetUpVector();

		float GetFieldOfView();
		float GetAspect();
		float GetZNear();
		float GetZFar();

		glm::vec3 GetRightVector();
		glm::vec3 GetRealUpVector();
		CMatrixCamera& GetMatrices();

};


class CCamera
{
	private:
		glm::vec3 m_Position;
		float m_FieldOfView;
		float m_Aspect;
		float m_zNear;
		float m_zFar;

		float m_AngleX;
		float m_AngleY;
		float m_CameraDistance;				// Odleg³oœæ od punktu m_Position, (aby uzyskaæ kamerê TPP)

		CParamsCamera m_Params;

		bool m_ParamsIs;

		void Changed();

	public:
		CCamera();
		CCamera(glm::vec3 Position, float zNear, float zFar, float FieldOfView, float Aspect,
				float m_CameraDistance = 0.0f, float AngleX = 0.0f, float AngleY = 0.0f);
		~CCamera();


		void SetPosition(glm::vec3 Position);
		void SetFieldOfView(float FieldOfView);
		void SetAspect(float Aspect);
		void SetZNear(float zNear);
		void SetZFar(float zFar);
		void SetAngleX(float Angle);
		void SetAngleY(float Angle);
		void SetCameraDistance(float CameraDistance);

		void Set(glm::vec3 Position, float zNear, float zFar, float FieldOfView, float Aspect,
				 float m_CameraDistance = 0.0f, float AngleX = 0.0f, float AngleY = 0.0f);


		glm::vec3 GetPosition();
		float GetFieldOfview();
		float GetAspect();
		float GetZNear();
		float GetZFar();
		float GetAngleX();
		float GetAngleY();
		float GetCameraDistance();


		CParamsCamera& GetParams();
		CMatrixCamera& GetMatrices();


		void Move(glm::vec3 DeltaPosition);
		void RotateX(float DeltaAngle);
		void RotateY(float DeltaAngle);

};


#endif // CAMERA_H_INCLUDED

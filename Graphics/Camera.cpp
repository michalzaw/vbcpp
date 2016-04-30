#include "Camera.h"


/* -------------------------------------------------------------------------------------- *
 * CMatrixCamera																		  *
 * -------------------------------------------------------------------------------------- */
CMatrixCamera::CMatrixCamera()
{
	Changed();
}


CMatrixCamera::CMatrixCamera(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix)
	: m_ViewMatrix(ViewMatrix),
	m_ProjectionMatrix(ProjectionMatrix)
{
	Changed();
}


CMatrixCamera::~CMatrixCamera()
{

}


void CMatrixCamera::Changed()
{
	m_ViewProjectionMatrixIs = false;
	m_ViewInvMatrixIs = false;
	m_ProjectionInvMatrixIs = false;
	m_ViewProjectionInvMatrixIs = false;
}


void CMatrixCamera::SetViewMatrix(glm::mat4& ViewMatrix)
{
	m_ViewMatrix = ViewMatrix;

	Changed();
}


void CMatrixCamera::SetProjectionMatrix(glm::mat4& ProjectionMatrix)
{
	m_ProjectionMatrix = ProjectionMatrix;

	Changed();
}


void CMatrixCamera::SetMatrices(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix)
{
	m_ViewMatrix = ViewMatrix;
	m_ProjectionMatrix = ProjectionMatrix;

	Changed();
}


glm::mat4& CMatrixCamera::GetViewMatrix()
{
	return m_ViewMatrix;
}


glm::mat4& CMatrixCamera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}


glm::mat4& CMatrixCamera::GetViewProjectionMatrix()
{
	if (!m_ViewProjectionMatrixIs)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		m_ViewProjectionMatrixIs = true;
	}

	return m_ViewProjectionMatrix;
}


glm::mat4& CMatrixCamera::GetViewInvMatrix()
{
	if (!m_ViewInvMatrixIs)
	{
		m_ViewInvMatrix = glm::inverse(m_ViewMatrix);
		m_ViewInvMatrixIs = true;
	}

	return m_ViewInvMatrix;
}


glm::mat4& CMatrixCamera::GetProjectionInvMatrix()
{
	if (!m_ProjectionInvMatrixIs)
	{
		m_ProjectionInvMatrix = glm::inverse(m_ProjectionMatrix);
		m_ProjectionInvMatrixIs = true;
	}

	return m_ProjectionInvMatrix;
}


glm::mat4& CMatrixCamera::GetViewProjectionInvMatrix()
{
	if (!m_ViewProjectionInvMatrixIs)
	{
		m_ViewProjectionInvMatrix = glm::inverse(GetViewProjectionMatrix());
		m_ViewProjectionInvMatrixIs = true;
	}

	return m_ViewProjectionInvMatrix;
}


/* -------------------------------------------------------------------------------------- *
 * CParamsCamera																		  *
 * -------------------------------------------------------------------------------------- */
CParamsCamera::CParamsCamera()
{
	Changed();
}


CParamsCamera::CParamsCamera(glm::vec3 EyePosition, glm::vec3 ForwardVector, glm::vec3 UpVector,
							 float FieldOfView, float Aspect, float zNear, float zFar)
	: m_EyePosition(EyePosition),
	m_ForwardVector(ForwardVector),
	m_UpVector(UpVector),
	m_FieldOfView(FieldOfView),
	m_Aspect(Aspect),
	m_zNear(zNear),
	m_zFar(zFar)
{
	Changed();
}


CParamsCamera::~CParamsCamera()
{

}


void CParamsCamera::Changed()
{
	m_RightVectorIs = false;
	m_RealUpVectorIs = false;
	m_MatricesIs = false;
}


void CParamsCamera::SetEyePosition(glm::vec3 EyePosition)
{
	m_EyePosition = EyePosition;

	Changed();
}


void CParamsCamera::SetForwardVector(glm::vec3 ForwardVector)
{
	m_ForwardVector = ForwardVector;

	Changed();
}


void CParamsCamera::SetUpVector(glm::vec3 UpVector)
{
	m_UpVector = UpVector;

	Changed();
}


void CParamsCamera::SetFieldOfView(float FieldOfView)
{
	m_FieldOfView = FieldOfView;

	Changed();
}


void CParamsCamera::SetAspect(float Aspect)
{
	m_Aspect = Aspect;

	Changed();
}


void CParamsCamera::SetZNear(float zNear)
{
	m_zNear = zNear;

	Changed();
}


void CParamsCamera::SetZFar(float zFar)
{
	m_zFar = zFar;

	Changed();
}


void CParamsCamera::SetParams(glm::vec3 EyePosition, glm::vec3 ForwardVector, glm::vec3 UpVector,
							  float FieldOfView, float Aspect, float zNear, float zFar)
{
	m_EyePosition = EyePosition;
	m_ForwardVector = ForwardVector;
	m_UpVector =  UpVector;
	m_FieldOfView = FieldOfView;
	m_Aspect = Aspect;
	m_zNear = zNear;
	m_zFar = zFar;

	Changed();
}


glm::vec3 CParamsCamera::GetEyePosition()
{
	return m_EyePosition;
}


glm::vec3 CParamsCamera::GetForwardVector()
{
	return m_ForwardVector;
}


glm::vec3 CParamsCamera::GetUpVector()
{
	return m_UpVector;
}


float CParamsCamera::GetFieldOfView()
{
	return m_FieldOfView;
}


float CParamsCamera::GetAspect()
{
	return m_Aspect;
}


float CParamsCamera::GetZNear()
{
	return m_zNear;
}


float CParamsCamera::GetZFar()
{
	return m_zFar;
}


glm::vec3 CParamsCamera::GetRightVector()
{
	if (!m_RightVectorIs)
	{
		m_RightVector = glm::normalize(glm::cross(m_ForwardVector, m_UpVector));
		m_RightVectorIs = true;
	}

	return m_RightVector;
}


glm::vec3 CParamsCamera::GetRealUpVector()
{
	if (!m_RealUpVectorIs)
	{
		m_RealUpVector = glm::normalize(glm::cross(GetRightVector(), m_ForwardVector));
		m_RealUpVectorIs = true;
	}

	return m_RealUpVector;
}


CMatrixCamera& CParamsCamera::GetMatrices()
{
	if (!m_MatricesIs)
	{
		glm::mat4 View = glm::lookAt(m_EyePosition, m_EyePosition + m_ForwardVector, GetRealUpVector());
		glm::mat4 Proj = glm::perspective(m_FieldOfView, m_Aspect, m_zNear, m_zFar);

		m_Matrices.SetMatrices(View, Proj);
		m_MatricesIs = true;
	}

	return m_Matrices;
}


/* -------------------------------------------------------------------------------------- *
 * CCamera																		  *
 * -------------------------------------------------------------------------------------- */


CCamera::CCamera()
{
	Changed();
}


CCamera::CCamera(glm::vec3 Position, float zNear, float zFar, float FieldOfView, float Aspect,
				 float CameraDistance, float AngleX, float AngleY)
	: m_Position(Position),
	m_FieldOfView(FieldOfView),
	m_Aspect(Aspect),
	m_zNear(zNear),
	m_zFar(zFar),
	m_AngleX(AngleX),
	m_AngleY(AngleY),
	m_CameraDistance(CameraDistance)
{
	Changed();
}


CCamera::~CCamera()
{

}


void CCamera::Changed()
{
	m_ParamsIs = false;
}


void CCamera::SetPosition(glm::vec3 Position)
{
	m_Position = Position;

	Changed();
}


void CCamera::SetFieldOfView(float FieldOfView)
{
	m_FieldOfView = FieldOfView;

	Changed();
}


void CCamera::SetAspect(float Aspect)
{
	m_Aspect = Aspect;

	Changed();
}


void CCamera::SetZNear(float zNear)
{
	m_zNear = zNear;

	Changed();
}


void CCamera::SetZFar(float zFar)
{
	m_zFar = zFar;

	Changed();
}


void CCamera::SetAngleX(float Angle)
{
	m_AngleX = Angle;

	Changed();
}


void CCamera::SetAngleY(float Angle)
{
	m_AngleY = Angle;

	Changed();
}


void CCamera::SetCameraDistance(float CameraDistance)
{
	m_CameraDistance = CameraDistance;

	Changed();
}


void CCamera::Set(glm::vec3 Position, float zNear, float zFar, float FieldOfView, float Aspect,
				  float CameraDistance, float AngleX, float AngleY)
{
	m_Position = Position;
	m_FieldOfView = FieldOfView;
	m_Aspect = Aspect;
	m_zNear = zNear;
	m_zFar = zFar;
	m_AngleX = AngleX;
	m_AngleY = AngleY;
	m_CameraDistance = CameraDistance;

	Changed();
}


glm::vec3 CCamera::GetPosition()
{
	return m_Position;
}


float CCamera::GetFieldOfview()
{
	return m_FieldOfView;
}


float CCamera::GetAspect()
{
	return m_Aspect;
}


float CCamera::GetZNear()
{
	return m_zNear;
}


float CCamera::GetZFar()
{
	return m_zFar;
}


float CCamera::GetAngleX()
{
	return m_AngleX;
}


float CCamera::GetAngleY()
{
	return m_AngleY;
}


float CCamera::GetCameraDistance()
{
	return m_CameraDistance;
}


CParamsCamera& CCamera::GetParams()
{
	if (!m_ParamsIs)
	{
		glm::vec3 EyePosition, ForwardVector;
		glm::vec3 UpVector(0.0f, 1.0f, 0.0f);


		ForwardVector = glm::vec3(cos(m_AngleX) * sin(m_AngleY),
						sin(m_AngleX),
						cos(m_AngleX) * cos(m_AngleY));


		if (m_CameraDistance == 0.0f)
		{
			EyePosition = m_Position;
		}
		else
		{
			EyePosition = m_Position - ForwardVector * m_CameraDistance;
		}


		m_Params.SetParams(EyePosition, ForwardVector, UpVector, m_FieldOfView, m_Aspect, m_zNear, m_zFar);


		m_ParamsIs = true;
	}

	return m_Params;
}


CMatrixCamera& CCamera::GetMatrices()
{
	return GetParams().GetMatrices();
}


void CCamera::Move(glm::vec3 DeltaPosition)
{
	SetPosition(m_Position + DeltaPosition);
}


void CCamera::RotateX(float DeltaAngle)
{
	SetAngleX(m_AngleX + DeltaAngle);
}


void CCamera::RotateY(float DeltaAngle)
{
	SetAngleY(m_AngleY + DeltaAngle);
}

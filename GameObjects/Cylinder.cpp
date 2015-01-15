//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "Cylinder.h"

using namespace DirectX;

//----------------------------------------------------------------------

Cylinder::Cylinder()
{
	Initialize(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f));
}

//----------------------------------------------------------------------

Cylinder::Cylinder(
	XMFLOAT3 position,
	float radius,
	XMFLOAT3 direction
	)
{
	Initialize(position, radius, direction);
}

//----------------------------------------------------------------------

void Cylinder::Initialize(
	XMFLOAT3 position,
	float radius,
	XMFLOAT3 direction
	)
{
	m_position = position;
	m_radius = radius;
	m_length = XMVectorGetX(XMVector3Length(XMLoadFloat3(&direction)));
	XMStoreFloat3(&m_axis, XMVector3Normalize(XMLoadFloat3(&direction)));

	XMVECTOR up = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	float angle1 = XMVectorGetX(
		XMVector3AngleBetweenVectors(up, XMLoadFloat3(&m_axis))
		);

	XMMATRIX mat1 = XMMatrixIdentity();

	if ((angle1 * angle1) > 0.025)
	{
		XMVECTOR axis1 = XMVector3Cross(up, XMLoadFloat3(&m_axis));

		mat1 = XMMatrixRotationAxis(axis1, angle1);
	}
	XMStoreFloat4x4(&m_rotationMatrix, mat1);

	XMStoreFloat4x4(
		&m_modelMatrix,
		XMMatrixScaling(m_radius, m_radius, m_length) *
		mat1 *
		XMMatrixTranslation(m_position.x, m_position.y, m_position.z)
		);
}

//--------------------------------------------------------------------------------

void Cylinder::UpdatePosition()
{
	XMStoreFloat4x4(
		&m_modelMatrix,
		XMMatrixScaling(m_radius, m_radius, m_length) *
		XMLoadFloat4x4(&m_rotationMatrix) *
		XMMatrixTranslation(m_position.x, m_position.y, m_position.z)
		);
}
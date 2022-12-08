/*
BSD 2-Clause License

Copyright (c) 2022, tyomaAni
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "slowlib.h"
#include "slowlib.base/scene/slCamera.h"

slCamera::slCamera()
{
	m_near = 0.01f;
	m_far = 2500.f;
	m_fov = 0.683264f;
	m_aspect = 1.3333333f;
	m_up.set(0.f, 1.f, 0.f);
}

slCamera::~slCamera()
{

}

void slCamera::Update()
{
	slMath::perspectiveLH(m_projectionMatrix, m_fov, m_aspect, m_near, m_far);
	slMath::lookAtLH(m_viewMatrix, m_position, m_target, m_up);
	
	slMath::mul(m_projectionMatrix, m_viewMatrix, m_viewProjectionMatrix);
	
	/*m_viewMatrixInvert = m_viewMatrix;
	slMath::invert(m_viewMatrixInvert);

	auto pi = m_projectionMatrix;
	slMath::invert(pi);
	slMath::mul(m_viewMatrixInvert, pi, m_viewProjectionInvertMatrix);

	m_frust.CalculateFrustum(m_projectionMatrix, m_viewMatrix);*/

	//m_direction = Direction::NorthEast;
}


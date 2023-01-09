/*
BSD 2-Clause License

Copyright (c) 2023, tyomaAni
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

#include "../../DemoApp.h"
#include "ExampleBasics3DModel.h"

class ExampleBasics3DModel_MyModel
{
	slGS* m_gs = 0;
	slArray<slGPUMesh*> m_meshBuffers; 
	slMaterial m_material;
	slMat4 m_W;
	slMat4 m_WVP;
public:
	ExampleBasics3DModel_MyModel(slGS* gs) :m_gs(gs) {}
	~ExampleBasics3DModel_MyModel()
	{
		for (size_t i = 0; i < m_meshBuffers.m_size; ++i)
		{
			slDestroy(m_meshBuffers.m_data[i]);
		}
	}

	class cb : public slMeshLoaderCallback
	{
	public:
		cb() {}
		virtual ~cb() {}

		virtual void OnMaterial(slMaterial* m, slString* name) override {}

		// It will be called from mesh loader.
		virtual void OnMesh(slMesh* newMesh, slString* name, slString* materialName) override {
			if (newMesh)
			{
				slGPUMesh* gpumesh = m_model->m_gs->SummonMesh(newMesh);
				if (gpumesh)
				{
					m_model->m_meshBuffers.push_back(gpumesh); //Save GPU mesh buffer
				}
				slDestroy(newMesh);
			}
		}

		ExampleBasics3DModel_MyModel* m_model = 0;
	}
	m_cb;

	bool Load(const char* p)
	{
		m_cb.m_model = this;
		slFramework::SummonMesh(p, &m_cb);

		m_material.m_maps[0].m_texture = 0;

		return m_meshBuffers.m_size > 0;
	}

	void Draw(slCamera* camera)
	{
		m_WVP = camera->m_projectionMatrix * camera->m_viewMatrix * m_W;
		m_material.m_sunPosition.set(0.f, 1.f, 0.f);
		
		// set shader by yourself
		m_gs->SetShader(slShaderType::Solid, 0);

		slFramework::SetMatrix(slMatrixType::World, &m_W);
		slFramework::SetMatrix(slMatrixType::WorldViewProjection, &m_WVP);
		for (size_t i = 0; i < m_meshBuffers.m_size; ++i)
		{
			m_gs->SetMesh(m_meshBuffers.m_data[i]);
			m_gs->SetMaterial(&m_material);
			m_gs->Draw();
		}
	}

	void SetPosition(const slVec3& p)
	{
		m_W.m_data[3] = p;
		m_W.m_data[3].w = 1.f;
	}
};

ExampleBasics3DModel::ExampleBasics3DModel(DemoApp*app):DemoExample(app)
{
}

ExampleBasics3DModel::~ExampleBasics3DModel()
{
}

void ExampleBasics3DModel::OnWindowSize(slWindow* w)
{
	DemoExample::OnWindowSize(w);
	if(m_camera)
		m_camera->m_aspect = w->GetCurrentSize()->x / w->GetCurrentSize()->y;
}

bool ExampleBasics3DModel::Init()
{
	m_camera = slFramework::SummonCamera();
	m_camera->m_position = slVec3(10.f, 10.f, 10.f);
	m_camera->m_aspect = (float)m_app->GetWindow()->GetCurrentSize()->x / (float)m_app->GetWindow()->GetCurrentSize()->y;
	m_camera->Update();

	// Set View Projection matrix for 3D line
	slFramework::SetMatrix(slMatrixType::ViewProjection, &m_camera->m_viewProjectionMatrix);

	// It's normal to use new\delete but I like to use my method
	m_model = slCreate<ExampleBasics3DModel_MyModel>(m_gs);
	if (!m_model->Load(slFramework::GetPathA("../data/models/4_objs.obj").c_str()))
	{
		slLog::PrintError("%s: can't load model\n", SL_FUNCTION);
		return false;
	}

	return true;
}

void ExampleBasics3DModel::Shutdown()
{
	SLSAFE_DESTROY(m_model);
	SLSAFE_DESTROY(m_camera);
}

void ExampleBasics3DModel::OnDraw()
{
	if (slInput::IsKeyHit(slInput::KEY_ESCAPE))
	{
		m_app->StopExample();
		return;
	}

	m_camera->Update();

	if (slInput::IsKeyHold(slInput::KEY_A))
		m_camera->m_position.x += 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_D))
		m_camera->m_position.x -= 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_W))
		m_camera->m_position.z += 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_S))
		m_camera->m_position.z -= 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_Q))
		m_camera->m_position.y += 10.0 * (double)(*m_app->m_dt);
	if (slInput::IsKeyHold(slInput::KEY_E))
		m_camera->m_position.y -= 10.0 * (double)(*m_app->m_dt);

	m_gs->BeginGUI();
	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();
	
	m_gs->SetShader(slShaderType::Line3D, 0);
	m_gs->DrawLine3D(slVec3(-1.f, 0.f, 0.f), slVec3(1.f, 0.f, 0.f), ColorRed);
	m_gs->DrawLine3D(slVec3(0.f, -1.f, 0.f), slVec3(0.f, 1.f, 0.f), ColorYellow);
	m_gs->DrawLine3D(slVec3(0.f, 0.f, -1.f), slVec3(0.f, 0.f, 1.f), ColorLime);

	m_model->Draw(m_camera);

	m_gs->EndDraw();
	m_gs->SwapBuffers();
}
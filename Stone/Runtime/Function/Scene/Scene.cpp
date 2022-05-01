#include "Scene.h"
#include "Resource/Components/Tag.h"

#include "Function/Event/EventSystem.h"
#include "Function/Scene/EditCamera.h"
#include "Function/Scene/Light.h"
#include "Function/Scene/Material.h"

#include "Resource/Components/Mesh.h"
#include "Resource/Components/Tag.h"
#include "Resource/Data/Implement/VCG/VCGMesh.h"

#include "Function/Render/Interface/Renderer.h"
#include "Function/Render/Interface/Shader.h"
namespace Stone
{
	Scene::Scene()
	{
		m_CurrentCamera = &PublicSingletonInstance(EditorCamera);
	}
	void Scene::renderTick()
	{
		m_CurrentCamera->bind();
		PublicSingletonInstance(GLobalLight).bind(1);
		PublicSingletonInstance(MaterialPool).getMaterial("BasicMaterial")->bind(2);
		PublicSingleton<ShaderPool>::getInstance().get("MeshShader")->bind();
		auto view = m_Registry.view<MeshComponent<VCGMesh>>();
		for (auto entity : view)
		{
			auto mesh = view.get<MeshComponent<VCGMesh>>(entity);
			PublicSingleton<Renderer>::getInstance().render(mesh.m_Mesh);
		}
	}
	void Scene::initialize()
	{
	}
	SceneObject Scene::CreateObject(const std::string& name)
	{
		SceneObject entity = { m_Registry.create(), this};
		auto& Tag = entity.AddComponent<TagComponent>();
		Tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
}

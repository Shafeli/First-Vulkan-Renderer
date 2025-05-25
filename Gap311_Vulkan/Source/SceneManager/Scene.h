#pragma once
#include "SceneGraph/SceneGraph.h"

/////////////////////////////////////////////////
// State interface
/////////////////////////////////////////////////

//TODO: Idea: Adding a clock to time the amount of seconds on this state

namespace Brokkr
{

    class CoreSystems;
    class GameEntityManager;

    class Scene
    {

    public:
        explicit Scene(CoreSystems* pEngineSystems)
            : m_pCoreSystems(pEngineSystems)
            , m_sceneGraph(pEngineSystems)
        {
            // What needs to be initialized

        }

        /////////////////////////////////////////////////
        // override state methods
        /////////////////////////////////////////////////
        virtual ~Scene();
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Exit() = 0;
        virtual void Enter() = 0;
        virtual void Destroy() = 0;

    protected:

        // System handles for easy access to things that are commonly used
        CoreSystems* m_pCoreSystems = nullptr;

        SceneGraph m_sceneGraph;
        std::vector<std::shared_ptr<Mesh<Brokkr::MeshVertex>>> m_sceneMeshes;


    private:


    };

    inline Scene::~Scene()
    {
        m_sceneGraph.CleanupNode(m_sceneGraph.GetRootNode());
    }
}

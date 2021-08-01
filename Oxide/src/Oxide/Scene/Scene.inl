
namespace Oxide {

    template<Derived<Oxide::Actor> T>
    Ref<T> Scene::AddActor() {
        Ref<T> newActor = Ref<T>(new T());
        m_Actors[newActor->TypeName].push_back(newActor);
        return newActor;
    }

    template<Derived<Oxide::ORenderer> T>
    Ref<T> Scene::AddRenderer() {
        Ref<T> newRenderer = Ref<T>(new T(this));
        m_Renderers[newRenderer->TypeName] = newRenderer;
        return newRenderer;
    }
    
};
#pragma once

namespace Brokkr
{
    class Component
    {
    public:
        virtual ~Component() = default;

        virtual bool Init() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;  // : This method could be called when the component is destroyed or removed from memory.
        virtual void Render() {}
        virtual void Attach() {}   // : This method could be called when the component is attached to an entity or added to the game world.
        virtual void Detach() {}   // : This method could be called when the component is detached from an entity or removed from the game world.
        virtual void Enable() {}   // : This method could be called when the component is enabled.
        virtual void Disable() {}  // : This method could be called when the component is disabled.

    };
}

//virtual void Render() : This method could be used to hold the Texture key and
//could get the transform Rect to pass to rendering pipe line to handle. But
//this sounds more like a Update() then a Draw() with my RenderPipeline


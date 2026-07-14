#pragma once
#include "Component.h"
#include "SpriteComponent.h"
#include <vector>
#include <string>

struct AnimationClip
{
    std::string name;
    SDL_Texture* texture = nullptr; // The spritesheet image itself
    std::vector<SDL_Rect> frames;   // List of coordinates for the animated sprites
    float fps = 12.0f;              
    bool loop = true;      
};

struct AnimationComponent : public Component
{
    const AnimationClip* currentClip = nullptr;
    int frameIndex = 0;
    float time = 0.0f;
    bool isPlaying = false;

    SpriteComponent* currSpriteComponent = nullptr;

    void Init() override
    {
        frameIndex = 0;
        time = 0.0f;
    }

    void Update(float dt) override {}

    void Play(const AnimationClip* clip)
    {
        if (currentClip == clip) return; // Currently an animation is playing

        currentClip = clip;
        frameIndex = 0;
        time = 0.0f;
        isPlaying = true;
    }

    void Pause()
    {
        isPlaying = false;
    }

    void Continue()
    {
        if (currentClip) isPlaying = true;
    }

    std::string GetCurrentAnimationName()
    {
        return currentClip ? currentClip->name : "";
    }
};

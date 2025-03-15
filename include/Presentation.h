#pragma once
#include "SetOfAnimation.h"

class Presentation
{
    public:
        std::vector<SetOfAnimation> present;
        int currentPresentation;

        Presentation(std::vector<SetOfAnimation> _present = {}, int _currentPresentation = 0): present(_present), currentPresentation(_currentPresentation) {}

        void CreateNewPresentation();
        void SetCurAnimation(int positionPresentation, float curAnimation); 
        void EraseAnimation(int positionPresentation, int positionSet, NewAnimation animation); 
        void InsertAnimationToSet(int positionPresentation, int positionSet, NewAnimation animation); 
        void CopySetToLast(int positionPresentation);
        bool DrawPresentation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontValue, const Font &fontText, const bool &isLightMode, const float &speed);
};
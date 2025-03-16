#pragma once
#include "SetOfAnimation.h"

class Presentation
{
    public:
        std::vector<SetOfAnimation> present;
        int currentPresentation = 0;

        Presentation(std::vector<SetOfAnimation> _present = {}, int _currentPresentation = 0): present(_present), currentPresentation(_currentPresentation) {}


        void Clear();

        void CreateNewPresentation();

        void CreateNewSet(int positionPresentation);

        void SetType(int positionPresentation, int type);
        void SetType(int positionPresentation, int positionSet, int type);

        void SetStartAnimation(int positionPresentation, float startAnimation);
        void SetStartAnimation(int positionPresentation, int positionSet, float startAnimation);

        void SetCurAnimation(int positionPresentation, float curAnimation); 
        void SetCurAnimation(int positionPresentation, int positionSet, float curAnimation); 

        void EraseAnimation(int positionPresentation, int positionSet, NewAnimation animation); 
        void InsertAnimationToSet(int positionPresentation, int positionSet, NewAnimation animation); 

        void CopySetToLast(int positionPresentation);

        void Swap(int positionPresentation, int i, int j);
        void Merge(int positionPresentation, int i, int j);

        NewAnimation GetAnimation(int positionPresentation, int positionSet, int i);

        void DrawPresentation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontValue, const Font &fontText, const bool &isLightMode, const float &speed);
};
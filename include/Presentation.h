#pragma once
#include "SetOfAnimation.h"

class Presentation
{
    public:
        std::vector<SetOfAnimation> present;
        std::vector<std::string> list;
        Vector2 position;
        float width, height;
        int currentPresentation = 0;
        int numberOfPresentation = 0;

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
        void EraseAnimation(int positionPresentation, NewAnimation animation);
        void InsertAnimationToSet(int positionPresentation, int positionSet, NewAnimation animation); 

        void CopySetToLast(int positionPresentation);

        void Swap(int positionPresentation, int i, int j);
        void Merge(int positionPresentation, int i, int j);

        NewAnimation GetAnimation(int positionPresentation, int positionSet, int i);

        void DrawPresentation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontValue, const Font &fontText, const bool &isLightMode, const float &speed, int curRemoteState);

        int CountNumberOfAnimation();
        void InitBoardText(const std::vector<std::string> &_list, Vector2 _postition, float _width, float _height);
        void DrawCodeBoxMultiHighlight(const std::vector<std::string>& list, Vector2 position, float width, float height, const std::vector<int>& highlightLines, const Font& fontText);

        void SetAllhighlightLines(int startPresentation, int endPresentation, std::vector<int> listHighlights);
        void SetCurToStartAnimation(int startPresentation, int endPresentation);
        void SetCurAnimation(int startPresentation, int endPresentation, int curr);
        void UnFillCurPresentation();
        void FillCurPresentation();
        void FindPosition(int newFrame, int &currentPresentation, int &positionSet);
        void SetCurToStartAnimationInOnePresentation(int i, int j);
        void NormPresentation(int curRemoteState);

 
};
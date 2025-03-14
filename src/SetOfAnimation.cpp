// #include "SetOfAnimation.h"

// bool SetOfAnimtion::DrawSetOfAnimation(const bool &isLightMode, const float &speed)
// {
//     bool done = currentSet == int(setOfAnimtion.size());
//     if (done)
//     {
//         currentSet = int(setOfAnimtion.size()) - 1;
//     }
//     bool groupDone = true;
//     for (int i = 0; i <= currentSet; ++i)
//     {   
//         for (NewAnimation &animation: setOfAnimtion[i])
//         {
//             groupDone &= animation.DrawAnimation(hollowCircle, solidCircle, arrowEdge, GetFontDefault(), isLightMode, speed);
//         }
//     }
//     currentSet += groupDone;
//     return done;
// }
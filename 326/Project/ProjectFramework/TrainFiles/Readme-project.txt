//Modification on TrainView.cpp, TrainView.h, CallBacks.cpp, World.cpp, World.h, TrainWindow.cpp and TrainWindow.h is made
//need to put \Utilities\bitmap.h and \Utilities\bitmap.c into the .sln for compiling the project

/*
GUI buttons and silders usage:
Run: run/not run the train, animations also run by it
<< / >> : manully move forward/ backward
ArcLength: constant speed on each segment and ties distance approximation
speed: speed of train
World/Train/Top: angle of viewing the world
Spline Type: Linear, Cardinal Cubic, Carmull-Rom and B-Splines are provided
Tension: only work with Spline Type = Cardinal Cubic
Add point / delete point / R+X / R-X / R+Z / R-Z: control on control points
Track Type: type of tracks, Nothing will allow users to ride in the air without any track
Unflatness: when it = 0, will be deflaut flat ground, otherwise randomly unflat ground
Carriages: 0~4 can be added in non-linear track mode and with CC Tension>=0.5 
Fog/Rain/Snow/Normal: weather control
Physics: auto speed control by refering to height(y position) of the train
*/
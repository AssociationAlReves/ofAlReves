#pragma once

// this must match the display resolution of your projector

#if __APPLE__

#define PROJECTOR_RESOLUTION_X 1024 //1024 //1280
#define PROJECTOR_RESOLUTION_Y 768 //768 //800
#define USE_KINECT

#else

#define PROJECTOR_RESOLUTION_X 1280 //512 //1024 //1280
#define PROJECTOR_RESOLUTION_Y 800// 32 //768 //800
#define USE_KINECT

#endif

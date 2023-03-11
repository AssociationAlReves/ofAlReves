# ofAlReves
A set of OpenFrameworks scenes used for "Compagnie Al Reves", Aix en Provence.
Here is a short of the final show : https://vimeo.com/137587683

# How to run

Tested with OF 0.11.2 and Apple M1.

Scenes are managed by the great ofxAppUtils. Use function keys (F1, F2, ...) to access specific scenes.
Use mouse wheel to zoom and left button to rotate.
Note: a different fork of ofxAppUtils has been modified to enable transformer cam access.

# OSC notes
In live stage, when there is no network, on mac you can right clic in WiFi menu to create a local network.

# Addons
 - ofxGui
 - ofxOpenCv
 - ofxXmlSettings
 - ofxAppUtils
 - ofxCv
 - ofxTween
 - ofxKinect
 - ofxOsc

# Prerequisites for Windows
- Install QuickTime
- If not working, **move** C:\Program Files\QuickTime\QTSystem\QTCF.dll to system32 or SysWOW64

# Scenes and commands

## ofxBgScene

- Plain background scene
- Zoom out a lot to display background as circle
- keys :  
  - ' ': clear
  - 'r': reset
  - 'c': toggle colors (blue/while <-> orange)
  - 'b': set background color (ie: blue outside big bg circle)

## ofxTerrain

Line terrain scene inspired by Adrien M and Clair B work, with holes and bumps

![image](https://user-images.githubusercontent.com/2776898/224487750-a9b9ae55-d70e-46d5-a7a2-8d1ce1508541.png)

- Line terrain scene inspired by Adrien M and Clair B work, with holes and bumps
- keys : 
  - 'r': reset
  - 'j': add hole at mouse position
  - 'J': add hill at mouse position
  - 'k': add small hill at mouse position
  - 'c': change cursor style (default or red dot)
  - 'm', 'M': show/hide cursor

## ofVasaDalleQuad

Fun with Vasarely inspired path

![image](https://user-images.githubusercontent.com/2776898/224488272-45b5a16e-ea70-4597-88d9-bea6da1b6895.png)

- keys : 
  - 'v': trace path at mouse position (they fade out after some time)
  - ' ': stop fade out (keep what's drawn)
  - 'c': clear
  - 'l': toggle lighting
  - '1' to '5': create unique or fill
  
## ofxVasaSquareField

Field or squares rotated by mouse.

<img width="1111" alt="image" src="https://user-images.githubusercontent.com/2776898/224489563-837a0bea-94cd-4f9c-a143-2c7e91262e7f.png">

- keys : 
  - 'c' or 'x' : hard reset with quick rotation effect
  - 'r': full reset
  - 'w': smooth reset with slow rotation effect
  - 'c': clear
  - 'n': toggle mode



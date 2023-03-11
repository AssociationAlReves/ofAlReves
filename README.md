# ofAlReves
A set of OpenFrameworks scenes used for "Compagnie Al Reves", Aix en Provence.
Here is a short of the final show : https://vimeo.com/137587683

# How to run

Tested with OF 0.11.2 and Apple M1.

Scenes are managed by the great ofxAppUtils. Use function keys (F1, F2, ...) to access specific scenes.
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


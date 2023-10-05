#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
	ofGLWindowSettings settings;
	settings.setGLVersion(4, 3);
	settings.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}

/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "NuiApi.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

using namespace std;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public OpenGLAppComponent, public Timer
{
public:
	INuiSensor *sensor;
	HANDLE imageStream = NULL;
	HANDLE depthStream = NULL;

	GLfloat *point_positions;
	GLfloat *point_colors;

	int width = 640;
	int height = 480;

	GLuint vboId; // Vertex buffer ID
	GLuint cboId; // Color buffer ID

    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
    }

    ~MainContentComponent()
    {
        shutdownOpenGL();
    }

	void timerCallback() override
	{
		openGLContext.triggerRepaint();
	}

	void initialise() override
	{
		HRESULT res;

		res = NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);

		int numCams;
		res = NuiGetSensorCount(&numCams);
		jassert(res == S_OK);

		res = NuiCreateSensorByIndex(numCams - 1, &sensor);
		jassert(res == S_OK);
		
		res = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &imageStream);
		jassert(res == S_OK);

		res = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &depthStream);
		jassert(res == S_OK);

		startTimer(100);
	}

    void shutdown() override
    {
		stopTimer();
		NuiShutdown();
    }

    void render() override
    {
        OpenGLHelpers::clear (Colours::black);


		HRESULT res;
		const NUI_IMAGE_FRAME *imgFrame = nullptr;
		res = NuiImageStreamGetNextFrame(imageStream, 200, &imgFrame);
		//jassert(res == S_OK);
		if (res != S_OK) {
			return;
		}
		const NUI_IMAGE_FRAME *depthFrame = nullptr;
		res = NuiImageStreamGetNextFrame(depthStream, 200, &depthFrame);
		//jassert(res == S_OK);
		if (res != S_OK) {
			return;
		}


		NUI_LOCKED_RECT *lockedImgRect = nullptr;
		NUI_LOCKED_RECT *lockedDepthRect = nullptr;
		res = imgFrame->pFrameTexture->LockRect(0, lockedImgRect, NULL, 0);
		jassert(res == S_OK);
		res = depthFrame->pFrameTexture->LockRect(0, lockedDepthRect, NULL, 0);
		jassert(res == S_OK);


		for (int i = 0; i < 640; i++) {
			for (int j = 0; j < 480; j++) {

				int offsetColor = (640 * i + 480 * j) * 12;
				float r = lockedImgRect->pBits[offsetColor];
				float g = lockedImgRect->pBits[offsetColor + 4];
				float b = lockedImgRect->pBits[offsetColor +  8];

				int offsetDepth = (640 * i + 480 * j) * 4;
				INT16 mm = lockedDepthRect->pBits[offsetDepth];
				float x, y, z;

				x = i;
				y = j;
				z = (float) mm * 10;

				glColor3f(r, g, b);
				glBegin(GL_POINTS);
				glVertex3f(x, y, z);
				glEnd();
			}
		}

		imgFrame->pFrameTexture->UnlockRect(0);
		depthFrame->pFrameTexture->UnlockRect(0);
		delete lockedImgRect;
		delete lockedDepthRect;
		delete imgFrame;
		delete depthFrame;
    }

    void paint (Graphics& g) override
    {
        // You can add your component specific drawing code here!
        // This will draw over the top of the openGL background.
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================

    // private member variables



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED

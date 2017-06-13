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
class MainContentComponent   : public OpenGLAppComponent
{
public:
	INuiSensor *sensor;
	HANDLE streamHandle = NULL;

    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
    }

    ~MainContentComponent()
    {
        shutdownOpenGL();
    }

	void initialise() override
	{
		HRESULT res;

		if (res = NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR)) {
			jassert(res == S_OK);
		}

		int numCams;
		if (res = NuiGetSensorCount(&numCams)) {
			jassert(res == S_OK);
		}

		if(res = NuiCreateSensorByIndex(numCams - 1, &sensor)) {
			jassert(res == S_OK);
		}

		
		if (res = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_1280x960, 0 , NUI_IMAGE_STREAM_FRAME_LIMIT_MAXIMUM, NULL, &streamHandle)) {
			jassert(res == S_OK);
		}


	}

    void shutdown() override
    {
		NuiShutdown();
    }

    void render() override
    {
        OpenGLHelpers::clear (Colours::black);

		HRESULT res;
		const NUI_IMAGE_FRAME **imgFrame;
		if (res = NuiImageStreamGetNextFrame(&streamHandle, 200, imgFrame)) {
			jassert(res == S_OK);
		}

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

#pragma once

#include "miniaudio.h"
#include <memory>

class IOStream
{
    public:
        IOStream();
        ~IOStream();

        void getDeviceInfo();
        int getInputDeviceCount();
        int getOutputDeviceCount();
        void initDeviceConfig();

        void initInput(unsigned int inputDeviceIndex);
        void initOutput(unsigned int outputDeviceIndex); 

        void initDevice();

        void startStream();
        void stopStream();
        bool error_occurred = false;
    private:
        ma_context context;

	    ma_device_config deviceConfig;
        ma_device device;

        ma_device_info* pPlaybackDeviceInfos;
        ma_uint32 playbackDeviceCount;

        ma_device_info* pCaptureDeviceInfos;
        ma_uint32 captureDeviceCount;

        void handleError();
};

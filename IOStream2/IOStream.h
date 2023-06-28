#pragma once

#include "miniaudio.h"
#include <memory>

class IOStream
{
public:
    IOStream();
    ~IOStream();

    void initContext();
    void uninitContext();

    void initDeviceCallback();
    void getDeviceInfo();
    int getInputDeviceCount();
    int getOutputDeviceCount();
    void initDeviceConfig();

    void initInput(int inputDeviceIndex);
    void initOutput(int outputDeviceIndex);

    void initDevice();

    void startStream();
    void stopStream();
    bool error_occurred = false;

private:
    ma_context context;

    ma_device_config deviceConfig;
    ma_device device;

    ma_device_info *pPlaybackDeviceInfos;
    ma_uint32 playbackDeviceCount = 0;

    ma_device_info *pCaptureDeviceInfos;
    ma_uint32 captureDeviceCount = 0;

    void handleError();
};

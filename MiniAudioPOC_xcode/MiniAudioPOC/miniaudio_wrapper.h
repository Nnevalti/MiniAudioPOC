//
//  miniaudio_wrapper.h
//  MiniAudioPOC
//
//  Created by SoundX on 04/05/2023.
//

#ifndef miniaudio_wrapper_h
#define miniaudio_wrapper_h

#import <Foundation/Foundation.h>

#ifdef __cplusplus
extern "C" {
#endif

//#ifdef __APPLE__
//    #define MA_NO_RUNTIME_LINKING
//#endif
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// miniaudio function declarations go here

#include <stdio.h>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    MA_ASSERT(pDevice->capture.format == pDevice->playback.format);
    MA_ASSERT(pDevice->capture.channels == pDevice->playback.channels);

    /* In this example the format and channel count are the same for both input and output which means we can just memcpy(). */
    MA_COPY_MEMORY(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
}

int startLoop()
{

    ma_result result;
    ma_context context;
    ma_device_info* pPlaybackDeviceInfos;
    ma_uint32 playbackDeviceCount;
    ma_device_info* pCaptureDeviceInfos;
    ma_uint32 captureDeviceCount;
    ma_uint32 iDevice;

    if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
        printf("Failed to initialize context.\n");
        return -2;
    }

    result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, &pCaptureDeviceInfos, &captureDeviceCount);
    if (result != MA_SUCCESS) {
        printf("Failed to retrieve device information.\n");
        return -3;
    }

    printf("Playback Devices\n");
    for (iDevice = 0; iDevice < playbackDeviceCount; ++iDevice) {
        printf("    %u: %s %s\n", iDevice, pPlaybackDeviceInfos[iDevice].name, pPlaybackDeviceInfos[iDevice].isDefault ? "(Default)" : "");
    }

    printf("\n");

    printf("Capture Devices\n");
    for (iDevice = 0; iDevice < captureDeviceCount; ++iDevice) {
        printf("    %u: %s %s\n", iDevice, pCaptureDeviceInfos[iDevice].name, pCaptureDeviceInfos[iDevice].isDefault ? "(Default)" : "");
    }

    // get user input to choose device input and output index
    ma_uint32 inputDeviceIndex;
    ma_uint32 outputDeviceIndex;
    printf("Choose input device index: ");
    scanf("%d", &inputDeviceIndex);
    printf("Choose output device index: ");
    scanf("%d", &outputDeviceIndex);

    ma_device_config deviceConfig;
    ma_device device;

    deviceConfig = ma_device_config_init(ma_device_type_duplex);
//    deviceConfig.capture.pDeviceID  = &pCaptureDeviceInfos[inputDeviceIndex].id;
    deviceConfig.capture.pDeviceID  = NULL;
    deviceConfig.capture.format     = ma_format_s16;
    deviceConfig.capture.channels   = 2;
    deviceConfig.capture.shareMode  = ma_share_mode_shared;

//    deviceConfig.playback.pDeviceID = &pPlaybackDeviceInfos[outputDeviceIndex].id;
    deviceConfig.playback.pDeviceID = NULL;
    deviceConfig.playback.format    = ma_format_s16;
    deviceConfig.playback.channels  = 2;
    deviceConfig.dataCallback       = data_callback;
    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        return result;
    }

    ma_device_start(&device);


    printf("Press Enter to quit...\n");
    getchar();

    ma_device_uninit(&device);
    ma_context_uninit(&context);

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* miniaudio_wrapper_h */

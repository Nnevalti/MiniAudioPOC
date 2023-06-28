#include "miniaudio.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    /* In this example the format and channel count are the same for both input and output which means we can just memcpy(). */
    std::memcpy(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
}

int main(int argc, char** argv)
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
    // ma_uint32 inputDeviceIndex;
    // ma_uint32 outputDeviceIndex;
    // printf("Choose input device index: ");
    // scanf("%d", &inputDeviceIndex);
    // printf("Choose output device index: ");
    // scanf("%d", &outputDeviceIndex);

    ma_device_config deviceConfig;
    ma_device device;

    deviceConfig = ma_device_config_init(ma_device_type_duplex);

    deviceConfig.capture.pDeviceID  = &pCaptureDeviceInfos[1].id;
    deviceConfig.capture.format     = ma_format_s16;
    deviceConfig.capture.channels   = 2;
    deviceConfig.capture.shareMode  = ma_share_mode_shared;

    deviceConfig.playback.pDeviceID = &pPlaybackDeviceInfos[2].id;
    deviceConfig.playback.format    = ma_format_s16;
    deviceConfig.playback.channels  = 2;
    deviceConfig.dataCallback       = data_callback;
    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        return result;
    }

    ma_device_start(&device);


	char input;
    std::cout << "\nRunning ... press <enter> to quit" << std::endl;
    std::cin.get(input);

    ma_device_stop(&device);

    sleep(2);

    ma_device_start(&device);

    std::cout << "\nRunning again... press <enter> to quit" << std::endl;
    std::cin.get(input);

    ma_context_uninit(&context);

    (void)argc;
    (void)argv;
    return 0;
}

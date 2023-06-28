#include <iostream>
#include <memory>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "IOStream.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    MA_ASSERT(pDevice->capture.format == pDevice->playback.format);
    MA_ASSERT(pDevice->capture.channels == pDevice->playback.channels);

    /* In this example the format and channel count are the same for both input and output which means we can just memcpy(). */
    MA_COPY_MEMORY(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
}

IOStream::IOStream()
{
    ma_result result;

	result = ma_context_init(NULL, 0, NULL, &context);
	if (result != MA_SUCCESS)
	{
        printf("Failed to initialize context.\n");
		handleError();
    }
}

IOStream::~IOStream()
{
    ma_context_uninit(&context);
}

void IOStream::getDeviceInfo()
{
    ma_result result;

	result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, &pCaptureDeviceInfos, &captureDeviceCount);
    if (result != MA_SUCCESS) {
        printf("Failed to retrieve device information.\n");
        handleError();
    }
}

int IOStream::getInputDeviceCount() { return captureDeviceCount; }

int IOStream::getOutputDeviceCount() { return playbackDeviceCount; }


void IOStream::initDeviceConfig()
{
	deviceConfig = ma_device_config_init(ma_device_type_duplex);
    deviceConfig.sampleRate = 44100;
}

void IOStream::initInput(unsigned int inputDeviceIndex)
{
	ma_uint32 iDevice;

	printf("Capture Devices\n");
    for (iDevice = 0; iDevice < captureDeviceCount; ++iDevice) {
        printf("    %u: %s %s\n", iDevice, pCaptureDeviceInfos[iDevice].name, pCaptureDeviceInfos[iDevice].isDefault ? "(Default)" : "");
    }

	deviceConfig.capture.pDeviceID  = &pCaptureDeviceInfos[inputDeviceIndex].id;
    // deviceConfig.capture.pDeviceID  = NULL;
    deviceConfig.capture.format     = ma_format_f32;
    deviceConfig.capture.channels   = 2;
    // deviceConfig.capture.shareMode  = ma_share_mode_shared;
}

void IOStream::initOutput(unsigned int outputDeviceIndex)
{
	ma_uint32 iDevice;

	printf("Playback Devices\n");
    for (iDevice = 0; iDevice < playbackDeviceCount; ++iDevice) {
        printf("    %u: %s %s\n", iDevice, pPlaybackDeviceInfos[iDevice].name, pPlaybackDeviceInfos[iDevice].isDefault ? "(Default)" : "");
    }

	deviceConfig.playback.pDeviceID = &pPlaybackDeviceInfos[outputDeviceIndex].id;
    // deviceConfig.playback.pDeviceID = NULL;
    deviceConfig.playback.format    = ma_format_f32;
    deviceConfig.playback.channels  = 2;
}

void IOStream::initDevice()
{
    ma_result result;

    deviceConfig.dataCallback = data_callback;
	result = ma_device_init(NULL, &deviceConfig, &device);
	if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
		printf("Failed to initialize device.\n");
        handleError();
	}
}

void IOStream::startStream()
{
    ma_device_state state = ma_device_get_state(&device);
    if (state == ma_device_state_stopped) {
        ma_device_start(&device);
    } else {
        std::cout << "Device is already running" << std::endl;
    }
    std::cout << "Started stream" << std::endl;
}

void IOStream::stopStream()
{
    ma_device_state state = ma_device_get_state(&device);
    if (state == ma_device_state_started) {
        ma_device_stop(&device);
    } else {
        std::cout << "Device is already stopped" << std::endl;
    }
	ma_device_stop(&device);
}

void IOStream::handleError()
{
    error_occurred = true;
    ma_context_uninit(&context);
}

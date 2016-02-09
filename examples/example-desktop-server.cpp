// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

#include <uplink.h>
#include "desktop-server.h"
#include "desktop-ui.h"

static const bool sendPingPongColorFeedback = true;
static const bool dumpStatsPeriodically = true;

using namespace uplink;

//------------------------------------------------------------------------------

struct ExampleServerSession : DesktopServerSession
{
    ExampleServerSession(int socketDescriptor, Server* server)
        : DesktopServerSession(socketDescriptor, server)
    {

    }

    void toggleExposureAndWhiteBalance()
    {
        SessionSetup sessionSetup;

        static bool toggle = true;

        if (toggle)
        {
            sessionSetup.addSetColorCameraExposureModeAction(ColorCameraExposureMode_Locked);
            sessionSetup.addSetColorCameraWhiteBalanceModeAction(ColorCameraWhiteBalanceMode_Locked);


            std::cout << "LOCKED!" << std::endl;

            uplink_log_info("Locked exposure and white balance.");
        }
        else
        {
            sessionSetup.addSetColorCameraExposureModeAction(ColorCameraExposureMode_ContinuousAuto);
            sessionSetup.addSetColorCameraWhiteBalanceModeAction(ColorCameraWhiteBalanceMode_ContinuousAuto);

            uplink_log_info("Automatic exposure and white balance.");
        }

        server()._currentSession->sendSessionSetup(sessionSetup);

        toggle = !toggle;
    }

    virtual void onCustomCommand(const String& command)
    {
        if (command == "RecordButtonPressed")
        {
            std::cout << "Start Recording Pressed!" << std::endl;
        }
        else if (command == "AutoLevelButtonPressed")
        {
            toggleExposureAndWhiteBalance();
        }
    }

    virtual bool onMessage(const Message& message)
    {
        // Do not call blocking functions from this point on. Network performance will suffer greatly otherwise.

        switch (message.kind())
        {
            //case uplink::MessageKind_MessageFragment:
            //{
            //    const MessageFragment& messageFragment = message.as<MessageFragment>();

            //    std::cout << messageFragment.toString() << std::endl;
            //
            //    break;
            //}

        case MessageKind_DeviceMotionEvent:
        {
            std::cout << "IMU" << std::endl;
            break;
        }

        case MessageKind_CameraFrame:
        {
            const CameraFrame& cameraFrame = message.as<CameraFrame>();

            if (!cameraFrame.colorImage.isEmpty())
            {
                server().ui().setColorImage(
                    (const uint8*)cameraFrame.colorImage.planes[0].buffer,
                    int(cameraFrame.colorImage.width),
                    int(cameraFrame.colorImage.height)
                    );
            }


            if (!cameraFrame.depthImage.isEmpty())
            {
                uint16* depthBuffer = (uint16*)cameraFrame.depthImage.planes[0].buffer;
                int     depthWidth = int(cameraFrame.depthImage.width);
                int     depthHeight = int(cameraFrame.depthImage.height);

                // Convert shifts to depth values.
                shift2depth(depthBuffer, depthWidth * depthHeight);

                server().ui().setDepthImage(
                    depthBuffer,
                    depthWidth,
                    depthHeight
                    );
            }

            // Send ping-pong feedback image.
            // FIXME: This const-cast sucks.
            if (sendPingPongColorFeedback && !cameraFrame.colorImage.isEmpty())
                //sendFeedbackImage(const_cast<Image&>(cameraFrame.colorImage));
                sendImage(const_cast<Image&>(cameraFrame.colorImage));

            static unsigned long long count = 1; // FIXME: Use a real steady-rate timer.

            //if (dumpStatsPeriodically && 0 == count % 150)
            //{
            //    uplink_log_info("Camera receiving rate: %f Hz", server()._currentSession->messageStats[MessageKind_CameraFrame].receiving.rate.windowedRate());
            //    uplink_log_info("Motion receiving rate: %f Hz", server()._currentSession->messageStats[MessageKind_DeviceMotionEvent].receiving.rate.windowedRate());
            //    uplink_log_info("Feedback image sending rate: %f Hz", server()._currentSession->channels[ChannelId_Feedback].stats.sending.rate.windowedRate());
            //}

            ++count;

            break;
        }

        default:
        {
            std::cout << "Other" << std::endl;
            break;
        }
        }

        return true;
    }
};

//------------------------------------------------------------------------------

struct ExampleSessionSetup : SessionSetup
{
    ExampleSessionSetup(bool colorOnly)
    {
        addSetColorModeAction(ColorMode_VGA);

        if (colorOnly)
        {
            addSetDepthModeAction(DepthMode_None);
        }
        else // Depth too.
        {
            addSetDepthModeAction(DepthMode_VGA);
            addSetRegistrationModeAction(RegistrationMode_RegisteredDepth);
            addSetFrameSyncModeAction(FrameSyncMode_Depth);
        }

        //addSetColorSendingStrategyAction(ColorSendingStrategy_AllFrames);
        //addSetColorSendingSporadicityAction(1);

        //addSetCameraChannelSettingsAction(ChannelSettings(DroppingStrategy_DropAllButLatestOne));

        //addSetMotionModeAction(MotionMode_None);
        //addSetMotionRateAction(100.f);

        //addSetColorCameraExposureModeAction(ColorCameraExposureMode_ContinuousAuto);
        //addSetColorCameraWhiteBalanceModeAction(ColorCameraWhiteBalanceMode_ContinuousAuto);

        //addSetDepthCameraCodecAction(ImageCodecId_CompressedShifts);
        //addSetColorCameraCodecAction(ImageCodecId_JPEG);
        //addSetColorFeedbackCodecAction(ImageCodecId_JPEG);

        ChannelSettings channelSettings;
        channelSettings.droppingStrategy = DroppingStrategy_RandomOne;
        channelSettings.droppingThreshold = 90;
        channelSettings.bufferingStrategy = BufferingStrategy_Some;

        addSetRGBDFrameChannelAction(channelSettings);

        addSetSendMotionAction(false);
        addSetMotionRateAction(100);

        addSetColorCameraExposureModeAction(ColorCameraExposureMode_ContinuousAuto);
        addSetColorCameraWhiteBalanceModeAction(ColorCameraWhiteBalanceMode_ContinuousAuto);

        addSetDepthCameraCodecAction(ImageCodecId_CompressedShifts);
        addSetColorCameraCodecAction(ImageCodecId_JPEG);
        addSetFeedbackImageCodecAction(ImageCodecId_JPEG);
    }
};

//------------------------------------------------------------------------------

struct ExampleServerDelegate : ServerDelegate
{
    void sendClearAllButtonsCommand()
    {
        _server->_currentSession->sendCustomCommand("button:clear:*");
    }

    void sendButtonCreateCommand(std::string buttonPngFilepath, std::string commandName)
    {
        CustomCommand customCommand;
        customCommand.command += "button:create:";
        customCommand.command += char(0);
        customCommand.command += commandName;
        customCommand.command += '\0';

        std::ifstream f(buttonPngFilepath, std::ios::binary);
        if (!f.is_open()) {
            std::cerr << "ERROR: wrong button file path" << std::endl;
            getchar();
        }
        std::string imageBytes((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        customCommand.command.insert(customCommand.command.end(), imageBytes.begin(), imageBytes.end());

        _server->_currentSession->sendCustomCommand(customCommand);
    }

    virtual ServerSession* newSession(int socketDescriptor, Server* server)
    {
        _server = server;

        return new ExampleServerSession(socketDescriptor, server);
    }

    virtual void onConnect(uintptr_t sessionId)
    {
        sendClearAllButtonsCommand();
        sendButtonCreateCommand("record-button.png", "RecordButtonPressed");
        sendButtonCreateCommand("auto-level-button.png", "AutoLevelButtonPressed");

        bool rgbOnly = true;

        _server->_currentSession->sendSessionSetup(
            ExampleSessionSetup(rgbOnly)
            );
    }

    Server* _server;
};

//------------------------------------------------------------------------------

int
main()
{
    using namespace uplink;

    ExampleServerDelegate serverDelegate;

    // DesktopServer server("CaptureReceiverExample", UPLINK_SERVER_DEFAULT_TCP_PORT, &serverDelegate);
    DesktopServer server("CaptureReceiverExample", 6666, &serverDelegate);

    if (!server.startListening())
        return 1;

    server.ui().run();
}

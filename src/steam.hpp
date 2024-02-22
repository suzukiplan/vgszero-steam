/**
 * VGS-Zero SDK for Steam
 * License under GPLv3: https://github.com/suzukiplan/vgszero/blob/master/LICENSE-VGS0.txt
 * (C)2024, SUZUKI PLAN
 */
#include "SDL.h"
#include "../sdk/public/steam/steam_api.h"

class CSteam
{
  private:
    bool initialized;
    bool overlay;
    void (*putlog)(const char*, ...);
    InputActionSetHandle_t act;
    InputAnalogActionHandle_t actMove;
    InputDigitalActionHandle_t actFire;
    InputDigitalActionHandle_t actJump;
    InputDigitalActionHandle_t actPause;
    STEAM_CALLBACK_MANUAL(CSteam, onGameOverlayActivated, GameOverlayActivated_t, callbackGameOverlayActivated);

  public:
    CSteam(void (*putlog)(const char*, ...))
    {
        this->putlog = putlog;
        this->initialized = false;
        this->overlay = false;
        this->deactivate();
    }

    ~CSteam()
    {
        if (this->initialized) {
            putlog("Teminating Steam...");
            SteamAPI_Shutdown();
        }
    }

    void init()
    {
        putlog("Initializing Steam...");
        if (!SteamAPI_Init()) {
            putlog("SteamAPI_Init failed");
        } else {
            this->initialized = true;
            if (!SteamUserStats()->RequestCurrentStats()) {
                putlog("SteamUserStats::RequestCurrentStats failed!");
            }
            callbackGameOverlayActivated.Register(this, &CSteam::onGameOverlayActivated);
            if (!SteamInput()->Init(true)) {
                putlog("SteamInput::Init failed!");
            }
        }
    }

    uint8_t getJoypad(bool* connected)
    {
        SteamInput()->RunFrame();
        InputHandle_t inputHandles[STEAM_INPUT_MAX_COUNT];
        int num = SteamInput()->GetConnectedControllers(inputHandles);
        *connected = 0 < num;
        if (!*connected) {
            return 0;
        }
        auto inputHandle = inputHandles[0];
        if (!this->activate(inputHandle)) {
            return 0;
        }
        uint8_t result = 0;
        auto fire = SteamInput()->GetDigitalActionData(inputHandle, actFire);
        auto jump = SteamInput()->GetDigitalActionData(inputHandle, actJump);
        auto pause = SteamInput()->GetDigitalActionData(inputHandle, actPause);
        auto move = SteamInput()->GetAnalogActionData(inputHandle, actMove);
        result |= fire.bState ? VGS0_JOYPAD_T2 : 0;
        result |= jump.bState ? VGS0_JOYPAD_T1 : 0;
        result |= pause.bState ? VGS0_JOYPAD_ST : 0;
        result |= move.x < 0 ? VGS0_JOYPAD_LE : 0;
        result |= 0 < move.x ? VGS0_JOYPAD_RI : 0;
        result |= move.y < 0 ? VGS0_JOYPAD_DW : 0;
        result |= 0 < move.y ? VGS0_JOYPAD_UP : 0;
        return result;
    }

    inline bool isOverlay() { return this->overlay; }

  private:
    void deactivate()
    {
        this->act = 0;
        this->actFire = 0;
        this->actJump = 0;
        this->actPause = 0;
        this->actMove = 0;
    }

    bool activate(InputHandle_t inputHandle)
    {
        if (!this->act) {
            this->act = SteamInput()->GetActionSetHandle("InGameControls");
            if (!this->act) {
                return false;
            }
        }
        if (!this->actFire) {
            this->actFire = SteamInput()->GetDigitalActionHandle("fire");
            if (!this->actFire) {
                return false;
            }
        }
        if (!this->actJump) {
            this->actJump = SteamInput()->GetDigitalActionHandle("jump");
            if (!this->actJump) {
                return false;
            }
        }
        if (!this->actPause) {
            this->actPause = SteamInput()->GetDigitalActionHandle("pause");
            if (!this->actPause) {
                return false;
            }
        }
        if (!this->actMove) {
            this->actMove = SteamInput()->GetAnalogActionHandle("Move");
            if (!this->actMove) {
                return false;
            }
        }
        return true;
    }
};

void CSteam::onGameOverlayActivated(GameOverlayActivated_t* args)
{
    this->overlay = args->m_bActive;
}
#include <nds.h>
#include <dsiwifi7.h>
#include <wifi_debug.h>

void return_to_dsi_menu() {
    Wifi_Deinit();

    // This will skip the power-off/sleep mode screen when returning to HOME Menu
    i2cWriteRegister(0x4A, 0x70, 0x01); // Bootflag = Warmboot/SkipHealthSafety
    i2cWriteRegister(0x4A, 0x11, 0x01); // Reset to DSi/3DS HOME Menu
}

void vblank_handler(void) {
    if (fifoCheckValue32(FIFO_USER_01)) {
        return_to_dsi_menu();
    }
}

void vcount_handler() {
    inputGetAndSend();
}

volatile bool exitflag = false;

void powerButtonCB() {
    exitflag = true;
}

int main() {
    // clear sound registers
    dmaFillWords(0, (void*)0x04000400, 0x100);

    REG_SOUNDCNT |= SOUND_ENABLE;
    writePowerManagement(PM_CONTROL_REG, (readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE) | PM_SOUND_AMP);
    powerOn(POWER_SOUND);

    readUserSettings();
    ledBlink(0);

    irqInit();
    // Start the RTC tracking IRQ
    initClockIRQ();
    fifoInit();
    touchInit();

    // mmInstall(FIFO_MAXMOD);

    SetYtrigger(80);

    // installWifiFIFO();
    installSoundFIFO();

    installSystemFIFO();

    irqSet(IRQ_VCOUNT, vcount_handler);
    irqSet(IRQ_VBLANK, vblank_handler);

    irqEnable(IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

    setPowerButtonCB(powerButtonCB);

    installWifiFIFO();

    // Keep the ARM7 mostly idle
    while (!exitflag) {
        if (0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
            exitflag = true;
        }

        swiWaitForVBlank();
    }
    return 0;
}

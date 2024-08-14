// ==WindhawkMod==
// @id              windows11-taskbar-hider
// @name            Windows 11 Taskbar Hider and Full Screen Utilizer
// @description     Hides the Windows 11 taskbar and allows full screen utilization
// @version         1.0
// @author          Jeremi Lorenti
// @github          https://github.com/JeremiLorenti/windows11-taskbar-hider
// @homepage        https://jeremilorenti.com
// @include         explorer.exe
// @architecture    x86-64
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Windows 11 Taskbar Hider and Full Screen Utilizer

This mod allows you to completely hide the Windows 11 taskbar and utilize the full screen. It's reversible, so you can easily show the taskbar again when needed.

## Usage

1. IMPORTANT: You MUST set Windows taskbar behavior to "Automatically hide the taskbar" BEFORE enabling this mod. This setting is required for the mod to allow full screen resolution without preserving taskbar space.
2. Enable the mod to hide the taskbar and use full screen.
3. Disable the mod to show the taskbar and restore normal screen area.
4. Even with the taskbar hidden, you can still access the Start menu by pressing the Win key on your keyboard.

Note: If you don't set the taskbar to auto-hide before enabling the mod, you may not get the full screen utilization as intended.
*/

// ==WindhawkModSettings==
/*
- hideTaskbar: true
  $name: Hide Taskbar and Use Full Screen
  $description: When enabled, the taskbar will be hidden and the full screen will be utilized.
*/
// ==/WindhawkModSettings==

#include <windows.h>
#include <windhawk_utils.h>

struct {
    bool hideTaskbar;
} settings;

// Function to find the taskbar window
HWND FindTaskbarWindow() {
    return FindWindow(L"Shell_TrayWnd", NULL);
}

// Function to adjust the work area
void AdjustWorkArea(bool useFullScreen) {
    MONITORINFO mi = { sizeof(mi) };
    HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
    
    if (GetMonitorInfo(hMonitor, &mi)) {
        RECT rcWork = mi.rcMonitor;
        
        if (!useFullScreen) {
            // Restore default work area (subtract taskbar height)
            SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
        } else {
            // Set work area to full screen
            rcWork = mi.rcMonitor;
            SystemParametersInfo(SPI_SETWORKAREA, 0, &rcWork, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
        }
    }
}

// Function to hide or show the taskbar and adjust work area
void SetTaskbarVisibilityAndWorkArea(bool hide) {
    HWND hTaskbar = FindTaskbarWindow();
    if (hTaskbar) {
        if (hide) {
            // Hide the taskbar
            ShowWindow(hTaskbar, SW_HIDE);
            // Use full screen
            AdjustWorkArea(true);
        } else {
            // Show the taskbar
            ShowWindow(hTaskbar, SW_SHOW);
            // Restore normal work area
            AdjustWorkArea(false);
        }
    }
}

// Load settings
void LoadSettings() {
    settings.hideTaskbar = Wh_GetIntSetting(L"hideTaskbar");
}

// Initialize the mod
BOOL Wh_ModInit() {
    Wh_Log(L"Initializing Windows 11 Taskbar Hider and Full Screen Utilizer");

    LoadSettings();
    SetTaskbarVisibilityAndWorkArea(settings.hideTaskbar);

    return TRUE;
}

// Uninitialize the mod
void Wh_ModUninit() {
    Wh_Log(L"Uninitializing Windows 11 Taskbar Hider and Full Screen Utilizer");

    // Always show the taskbar and restore work area when unloading the mod
    SetTaskbarVisibilityAndWorkArea(false);
}

// Handle settings changes
void Wh_ModSettingsChanged() {
    Wh_Log(L"Settings changed");

    LoadSettings();
    SetTaskbarVisibilityAndWorkArea(settings.hideTaskbar);
}
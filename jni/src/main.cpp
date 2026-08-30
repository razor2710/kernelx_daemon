#include "draw.h"
#include "res/weiyan.h"
#include "res/cJSON.h"
#include "res/cJSON.c"
#include "res/Encrypt.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

int Pattern2;
int Pattern;
string Record;
char buffer[80] = {0};
float VersionNumber = 2.0f;

int main() {
    int Pattern;
    std::cout << "- Start Program (0) Exit Program (1) Reinstall (2)" << std::endl;
    std::cout << "- Option: ";
    std::cin >> Pattern;

    while (Pattern < 0 || Pattern > 2) {
        std::cout << "Invalid option, please try again: ";
        std::cin >> Pattern;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << std::endl << "Enable anti-recording (1[yes]/0[no]): ";
    cin >> Record;
    anti_recording = (Record != "0");

    sleep(1);
    screen_config();
    init_screen_x = screen_x + screen_y;
    init_screen_y = screen_y + screen_x;
    if (!init_vulkan(init_screen_x, init_screen_y, anti_recording))
        return 0;

    ImGui_init();
    FPS_limit.setAffinity();
    FPS_limit.SetFps(FPS);
    FPS_limit.AotuFPS_init();

    usleep(500);
    new std::thread(AimBotAuto);
    usleep(600);
    new std::thread(volume_monitor);

    while (true) {
        FPS_limit.SetFps(FPS);
        FPF_display = FPS_limit.AotuFPS();
        VK_Begin();
        Draw_Main(ImGui::GetForegroundDrawList());
        tick();
        VK_End();
    }

    shutdown();
    return 0;
}

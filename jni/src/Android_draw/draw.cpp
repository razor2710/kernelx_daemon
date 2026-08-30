#include "draw.h"
#include "悬浮窗图片/touxiang.h"
#include "内核读写/include.h"
#include "凯撒文件库/封装绘图.h"
#include "手持图片/内存图片/1.h"
#include "凯撒文件库/结构体.h"
#include "手持图片/小韩手持.h"
//保存配置
#include <fstream>

//音量键
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/input.h>
#include <dirent.h>




//调用
int 条件 = 0;
int 部位 = 0;
int PlayerId;
float 底部;
float guodu=150.0f;
bool voice=true;
bool isSetSize=false;
static bool IsBall = true;
timer FPS限制;
float FPF显示;
ImFont* 凯撒;
int FPS;



int Orientation = 0;
int screen_x = 0, screen_y = 0;
int init_screen_x = 0, init_screen_y = 0;






void 保存配置(const char *name = "CAESAR配置文件"){
    if (numSave == nullptr) {
        string SaveFile = "/data/local/tests/unrestricted/";
        SaveFile += "/";
        SaveFile += name;
        numSave = fopen(SaveFile.c_str(), "wb+");
    }

    std::string 配置;
    配置 += "FPS:" + std::to_string(FPS) + ";\n";
    配置 += "缩放:" + std::to_string(Config.预警绘制.缩放) + ";\n";
    配置 += "自瞄范围:" + std::to_string(Config.自动瞄准.自瞄范围) + ";\n";
    配置 += "压枪参数:" + std::to_string(Config.自动瞄准.压枪参数) + ";\n";
    配置 += "瞄准速度:" + std::to_string(Config.自动瞄准.瞄准速度) + ";\n";
    配置 += "锁定强度:" + std::to_string(Config.自动瞄准.锁定强度) + ";\n";
    配置 += "预判参数:" + std::to_string(Config.自动瞄准.预判参数) + ";\n";
    配置 += "自瞄距离:" + std::to_string(Config.自动瞄准.自瞄距离) + ";\n";
    配置 += "雷达X轴:" + std::to_string(Config.预警绘制.雷达X轴) + ";\n";
    配置 += "雷达Y轴:" + std::to_string(Config.预警绘制.雷达Y轴) + ";\n";
    配置 += "雷达大小:" + std::to_string(Config.预警绘制.雷达大小) + ";\n";
    配置 += "子弹速度:" + std::to_string(Config.自动瞄准.子弹速度) + ";\n";
    配置 += "触摸范围:" + std::to_string(Config.自动瞄准.触摸范围) + ";\n";
    配置 += "触摸位置Y:" + std::to_string(Config.自动瞄准.触摸位置Y) + ";\n";
    配置 += "触摸位置X:" + std::to_string(Config.自动瞄准.触摸位置X) + ";\n";
    配置 += "子弹下坠预判:" + std::to_string(Config.自动瞄准.子弹下坠预判) + ";\n";
    
    std::ofstream outputFile("/data/local/tests/unrestricted//CAESAR配置", ios::out | ios::trunc);
    if (outputFile.is_open()) {
        outputFile << 配置;
        outputFile.close();
    }

    // ⭐ THIS IS THE FIX: Only seek and write if the file actually opened!
    if (numSave != nullptr) {
        fseek(numSave, 0, SEEK_SET);
        fwrite(&Config.人物绘制.绘制方框, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.绘制射线, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.绘制骨骼, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.绘制名称, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.绘制距离, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.绘制血量, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.绘制雷达, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.手持武器, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.盒子绘制, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.盒子物资, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.载具绘制, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.手雷绘制, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.背敌预警, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.隐藏人机, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.隐藏Draw, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.自瞄开关, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.触摸位置, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.隐藏范围, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.音量快捷, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.持续锁定, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.忽略倒地, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.忽略人机, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.防录屏, sizeof(bool), 1, numSave);
        fwrite(&Config.人物绘制.无后台, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.性能模式, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.显示帧率, sizeof(bool), 1, numSave);
        fwrite(&Config.自动瞄准.触摸驱动, sizeof(bool), 1, numSave);
        fflush(numSave);
        fsync(fileno(numSave));
    }
}



void 读取配置(const char *name = "CAESAR配置文件"){

if (numSave == nullptr) {
string SaveFile = "/data/local/tests/unrestricted/";
SaveFile += "/";
SaveFile += name;
numSave = fopen(SaveFile.c_str(), "rb+");
}
if (numSave != nullptr) {
fseek(numSave, 0, SEEK_SET);
fread(&Config.人物绘制.绘制方框, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.绘制射线, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.绘制骨骼, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.绘制名称, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.绘制距离, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.绘制血量, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.绘制雷达, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.手持武器, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.盒子绘制, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.盒子物资, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.载具绘制, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.手雷绘制, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.背敌预警, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.隐藏人机, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.隐藏Draw, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.自瞄开关, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.触摸位置, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.隐藏范围, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.音量快捷, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.持续锁定, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.忽略倒地, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.忽略人机, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.防录屏, sizeof(bool), 1, numSave);
fread(&Config.人物绘制.无后台, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.性能模式, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.显示帧率, sizeof(bool), 1, numSave);
fread(&Config.自动瞄准.触摸驱动, sizeof(bool), 1, numSave);


std::ifstream inputFile("/data/local/tests/unrestricted//CAESAR配置");
  std::string line;
  while (std::getline(inputFile, line)) {
      if (line.find("FPS:") != std::string::npos) {
    FPS = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("缩放:") != std::string::npos) {
    Config.预警绘制.缩放 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("自瞄范围:") != std::string::npos) {
    Config.自动瞄准.自瞄范围 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("压枪参数:") != std::string::npos) {
    Config.自动瞄准.压枪参数 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("瞄准速度:") != std::string::npos) {
    Config.自动瞄准.瞄准速度 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("锁定强度:") != std::string::npos) {
    Config.自动瞄准.锁定强度 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("预判参数:") != std::string::npos) {
    Config.自动瞄准.预判参数 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("自瞄距离:") != std::string::npos) {
    Config.自动瞄准.自瞄距离 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("雷达X轴:") != std::string::npos) {
    Config.预警绘制.雷达X轴 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("雷达Y轴:") != std::string::npos) {
    Config.预警绘制.雷达Y轴 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("雷达大小:") != std::string::npos) {
    Config.预警绘制.雷达大小 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("子弹速度:") != std::string::npos) {
    Config.自动瞄准.子弹速度 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("触摸范围:") != std::string::npos) {
    Config.自动瞄准.触摸范围 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("触摸位置Y:") != std::string::npos) {
    Config.自动瞄准.触摸位置Y = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("触摸位置X:") != std::string::npos) {
    Config.自动瞄准.触摸位置X = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}else  if (line.find("子弹下坠预判:") != std::string::npos) {
    Config.自动瞄准.子弹下坠预判 = std::stof(line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1));
}
}


} else {
FPS = 60;
Config.预警绘制.缩放 = 28;
Config.自动瞄准.自瞄范围 = 150;
Config.自动瞄准.压枪参数 = 1.0;
Config.自动瞄准.瞄准速度 = 20.0;
Config.自动瞄准.锁定强度 = 8.0;
Config.自动瞄准.预判参数 = 1.3;
Config.自动瞄准.自瞄距离 = 150.0;
Config.预警绘制.雷达X轴 = 300.0f;
Config.预警绘制.雷达Y轴 = 400.0f;
Config.预警绘制.雷达大小 = 150.0f;
Config.自动瞄准.子弹速度 = 300.0;
Config.自动瞄准.触摸范围 = 150.0;
Config.自动瞄准.触摸位置Y= 1500.0f;
Config.自动瞄准.触摸位置X = 650.0f;
Config.自动瞄准.子弹下坠预判 = 0.1;
}
}


void ImGuiMenustyle() {
FPS = 60;
Config.预警绘制.缩放 = 28;
Config.自动瞄准.自瞄范围 = 150;
Config.自动瞄准.压枪参数 = 1.0;
Config.自动瞄准.瞄准速度 = 20.0;
Config.自动瞄准.锁定强度 = 8.0;
Config.自动瞄准.预判参数 = 1.3;
Config.自动瞄准.自瞄距离 = 150.0;
Config.预警绘制.雷达X轴 = 300.0f;
Config.预警绘制.雷达Y轴 = 400.0f;
Config.预警绘制.雷达大小 = 150.0f;
Config.自动瞄准.子弹速度 = 300.0;
Config.自动瞄准.触摸范围 = 150.0;
Config.自动瞄准.触摸位置Y= 1500.0f;
Config.自动瞄准.触摸位置X = 650.0f;
Config.自动瞄准.子弹下坠预判 = 0.1;
}


//=====================小韩调用音量键=======================//
int 数据() {
    DIR *dir = opendir("/dev/input/");
    if (dir == NULL) return -1;
    struct dirent *ptr = NULL;
    int count = 0;
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, "event"))
            count++;
    }
    closedir(dir);
    return count ? count : -1;
}

void 处理输入事件(struct input_event ev) {
if (ev.type == EV_KEY && ev.value == 1) {
if (ev.code == KEY_VOLUMEUP) { // 音量+
IsBall = true;
MemuSwitch = true;
} else if (ev.code == 114&&ev.value==1) {
IsBall = false;
}
}
}

int 音量() {
    int EventCount = 数据();
    if (EventCount < 0) {
        printf("未找到输入设备\n");
        return -1;
    }

    int *fdArray = (int *)malloc(EventCount * sizeof(int));
    fd_set fds;
    struct timeval tv;
    int maxfd = 0;

    for (int i = 0; i < EventCount; i++) {
        char temp[128];
        sprintf(temp, "/dev/input/event%d", i);
        fdArray[i] = open(temp, O_RDONLY | O_NONBLOCK);
        if(fdArray[i] > maxfd) maxfd = fdArray[i];
    }

    struct input_event ev;

    while (1) {
        FD_ZERO(&fds);
        for (int i = 0; i < EventCount; i++) {
            FD_SET(fdArray[i], &fds);
        }

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int ret = select(maxfd + 1, &fds, NULL, NULL, &tv);
        if (ret == -1) {
            perror("select error");
            break;
        } else if (ret == 0) {
            continue;
        } else {
            for (int i = 0; i < EventCount; i++) {
                if (FD_ISSET(fdArray[i], &fds)) {
                    memset(&ev, 0, sizeof(ev));
                    if (read(fdArray[i], &ev, sizeof(ev)) == sizeof(ev)) {
                        处理输入事件(ev);
                    }
                }
                usleep(5000);
            }
            usleep(5000);
        }
        usleep(5000);
    }

    for (int i = 0; i < EventCount; i++) {
        if (fdArray[i] >= 0) close(fdArray[i]);
    }
    free(fdArray);

    return 0;
}


void ImGuiMenustyle();
void NumIoLoad(const char *name);
//=====================渲染=====================//
ANativeWindow *native_window;
android::ANativeWindowCreator::DisplayInfo displayInfo{0};
VulkanGraphics VK;
Screen full_screen;





//============================读取配置==============================
string exec(string command) {
char buffer[128];
string result = "";
// Open pipe to file
FILE* pipe = popen(command.c_str(), "r");
if (!pipe) {
return "popen failed!";
}
// read till end of process:
while (!feof(pipe)) {
// use buffer to read and add to result
if (fgets(buffer, 128, pipe) != nullptr){
result += buffer;
}
}
pclose(pipe);
return result;
}


int init_vulkan(int _screen_x, int _screen_y, bool 防录屏) {
native_window = android::ANativeWindowCreator::Create("Ssage", _screen_x, _screen_y, 防录屏);;
ANativeWindow_acquire(native_window);
VK.Create(native_window, _screen_x, _screen_y);
return 1;
}


void 绘制手持(TextureInfo textureInfo, int x, int y) {
    int 图片大小 = 10; // 图片大小
    float 新宽度比例 = 1.0f; // 假设我们想要图像宽度增加一倍
    ImGui::GetBackgroundDrawList()->AddImage(textureInfo.textureId, ImVec2(x - textureInfo.w / 2.22 - 图片大小, y - 图片大小), ImVec2(x - textureInfo.w / 2.22 + textureInfo.w / 3 * 新宽度比例, y + textureInfo.h / 3));
}


void ImGui_init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(22.0f, 18.0f);
        style.FramePadding = ImVec2(14.0f, 10.0f);
        style.ItemSpacing = ImVec2(12.0f, 10.0f);
        style.ItemInnerSpacing = ImVec2(8.0f, 7.0f);
        style.WindowRounding = 14.0f;
        style.ChildRounding = 12.0f;
        style.FrameRounding = 9.0f;
        style.PopupRounding = 12.0f;
        style.ScrollbarRounding = 10.0f;
        style.GrabRounding = 8.0f;
        style.TabRounding = 9.0f;
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        ImVec4* c = style.Colors;
        c[ImGuiCol_Text] = ImVec4(0.96f,0.96f,0.98f,1.0f);
        c[ImGuiCol_WindowBg] = ImVec4(0.045f,0.050f,0.060f,0.98f);
        c[ImGuiCol_ChildBg] = ImVec4(0.060f,0.065f,0.075f,0.96f);
        c[ImGuiCol_FrameBg] = ImVec4(0.090f,0.095f,0.110f,1.0f);
        c[ImGuiCol_FrameBgHovered] = ImVec4(0.14f,0.115f,0.085f,1.0f);
        c[ImGuiCol_FrameBgActive] = ImVec4(0.19f,0.135f,0.070f,1.0f);
        c[ImGuiCol_Button] = ImVec4(0.105f,0.095f,0.080f,1.0f);
        c[ImGuiCol_ButtonHovered] = ImVec4(0.78f,0.31f,0.045f,1.0f);
        c[ImGuiCol_ButtonActive] = ImVec4(1.0f,0.48f,0.075f,1.0f);
        c[ImGuiCol_Header] = ImVec4(0.13f,0.095f,0.060f,1.0f);
        c[ImGuiCol_HeaderHovered] = ImVec4(0.78f,0.31f,0.045f,0.85f);
        c[ImGuiCol_HeaderActive] = ImVec4(1.0f,0.48f,0.075f,0.95f);
        c[ImGuiCol_CheckMark] = ImVec4(1.0f,0.50f,0.08f,1.0f);
        c[ImGuiCol_SliderGrab] = ImVec4(0.90f,0.37f,0.05f,1.0f);
        c[ImGuiCol_SliderGrabActive] = ImVec4(1.0f,0.58f,0.12f,1.0f);
        c[ImGuiCol_Tab] = ImVec4(0.075f,0.070f,0.065f,1.0f);
        c[ImGuiCol_TabHovered] = ImVec4(0.80f,0.32f,0.05f,0.90f);
        c[ImGuiCol_TabActive] = ImVec4(0.58f,0.23f,0.04f,1.0f);
        c[ImGuiCol_Border] = ImVec4(0.20f,0.21f,0.24f,0.70f);
    }
    ImGui_ImplAndroid_Init(native_window);
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    VK.Setup();  
    ColorInitialization();
    FloatBall = VK.ImAgeHeadFile(touxiang, sizeof(touxiang)).textureId;    
// 加载字体
    io.Fonts->AddFontFromMemoryTTF((void *) 小韩_H, 小韩_H_size, 35.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    凯撒 = io.Fonts->AddFontFromMemoryTTF((void *) OPPOSans_H, OPPOSans_H_size, 35.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    ImGuiMenustyle(); //初始存储 
    读取配置();
    颜色初始化();
加载图片();
}


void VK_Begin() {
screen_config();
ImGui_ImplVulkan_NewFrame();
ImGui_ImplAndroid_NewFrame(init_screen_x, init_screen_y);
ImGui::NewFrame();
}
void VK_End() {
// 确保ImGui::Render不会引发错误
ImGui::Render();
VK.Render(ImGui::GetDrawData()); // 使用VK进行渲染
}


void shutdown() {
    VK.PrepareShutdown();
    ImGui_ImplVulkan_Shutdown();    
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();            
    VK.Cleanup();    
    ANativeWindow_release(native_window);
    android::ANativeWindowCreator::Destroy(native_window);
}


void screen_config() {
displayInfo = android::ANativeWindowCreator::GetDisplayInfo();
screen_x = displayInfo.width;
screen_y = displayInfo.height;
Orientation = displayInfo.orientation;
}


#define AIMBOT_SEPARATED
#include "凯撒文件库/aim.h"
int DrawInt() {
进程 = GetPID((char*)"com.tencent.ig");
进程 = GetPID((char*)"com.rekoo.pubgm");
进程 = GetPID((char*)"com.vng.pubgmobile");
进程 = GetPID((char*)"com.pung.krmobile");
if(进程<=0){
}
driver->initialize(进程);
基址头 = driver->get_module_base((char*)"libUE4.so");
return 0;
}



void Draw_Main(ImDrawList *draw) {
// 头文件里面写的就是函数实现，因为比较多，而且需要if判断，我就把他们写到别的文件这样程序主代码看起来可读性更高
py = screen_y/2;  
px = screen_x/2;

// 检查自动瞄准开关是否开启
if(Config.自动瞄准.自瞄开关){
    // 如果隐藏范围配置为true，则不绘制任何范围
    if(Config.自动瞄准.隐藏范围){
        // 不执行任何绘制操作，范围将不会显示
    } else {
        // 绘制自瞄范围
        ImGui::GetForegroundDrawList()->AddCircle({px, py}, Config.自动瞄准.自瞄范围, ImColor(255,188,0,255), 0, 1.5f);
    }
}

if (Config.自动瞄准.触摸位置) {
    ImGui::GetForegroundDrawList()->AddRectFilled({0, 0}, {px * 2, py * 2}, ImColor(0, 0, 0, 110));

    ImVec2 center = {Config.自动瞄准.触摸位置Y, py * 2 - Config.自动瞄准.触摸位置X};
    float radius = Config.自动瞄准.触摸范围 / 2;

    ImGui::GetForegroundDrawList()->AddCircleFilled(center, radius, TouchingColor);

    std::string text = "勿放控件\n长按拖动";

    ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

    ImVec2 text_pos = ImVec2(center.x - text_size.x / 2, center.y - text_size.y / 2);

    ImGui::GetForegroundDrawList()->AddText(NULL, 40.0f, text_pos, ImColor(255, 255, 255), text.c_str());
}



if (Config.人物绘制.绘制雷达){
ImGui::GetForegroundDrawList()->AddRect({Config.预警绘制.雷达X轴 - Config.预警绘制.雷达大小,Config.预警绘制.雷达Y轴-Config.预警绘制.雷达大小},{Config.预警绘制.雷达X轴 +Config.预警绘制.雷达大小,Config.预警绘制.雷达Y轴+Config.预警绘制.雷达大小},ImColor(255,255,255,125),5,0,1.3f);
}

/*Gname= 0xec73720+0x110
矩阵270= 0xecc4090+20+270 上赛季的
矩阵590= 0xf1d5f70+0xC0+0x590
矩阵9D0= 0xf1d5f70+0xC0+0x9D0
世界地址=  0xf1fb900+0x810+0x78
对象列阵=0x30    ActorArray=0xA0   对象数量=0xA8
自身结构体=0xf1fb900+0x810+0x78+0x38+0x78+0x30+0x28c8
int 自身队伍 = driver->Read<int>(自身结构体 + 0x998);//自身队编
//频道@WYNBTT 删除S全家
//频道@WYNBTT 删除S全家
//频道@WYNBTT 删除S全家
FOV=0x4b18+0x548+0x544
Firing = driver->Read<int>(自身结构体 + 0x1830); // 开火
Aiming = driver->Read<0结构体 + 0x1134); // 开镜
//频道@WYNBTT 删除S全家
//频道@WYNBTT 删除S全家
Vector3A Z=0x208+0x1c8
Vector3A D=0x208+0x1c8
敌人向量=0x208+0x1c8   

if (driver->Read<float>(对象结构体 + 0x2B78) == 479.5f) {
玩家名字=0x960
人机判断=0xa59
血量=0xE60   0xE64

int 敌人阵营 = driver->Read<int>(对象结构体 + 0x998); // 从内存中读取敌人的阵营
long int Mesh = driver->Read<uint64_t>(对象结构体 + 0x510);

long int human = Mesh + 0x210;

long int Bone = driver->Read<uint64_t>(Mesh + 0x9a8) + 0x30;//骨骼
if (Bone <= 0xffff) 
continue;
int BoneCount = driver->Read<uint64_t>(Mesh + 0x9a8 + 0x8);//骨骼
上面为主要
下为杂项
动作=0x1058
载具指针=0xeb0 状态=0x15c8
载具油量大油量=0x438
小油量=0x43c
载具血量大=0x350
小=0x354
子弹射击速度= 0x2608 + 0x5d8 + 0x1370 + 0x560
本局玩家偏移方法:世界->总数->本局玩家(以下同理)
总数:		0x428,
本局玩家:		0x818,
本局剩余队伍:		0xbb8,
本局剩余玩家:		0xbb4,
对局时间:		0x500,

int 手持武器 = 0x2608 + 0x5d8 + 0x1370 + 0x1E0
int 手持子弹 = 0x2608 + 0x5d8 + 0x1370 + 0x1018
int 子弹最大 = 0x2608 + 0x5d8 + 0x1370 + 0x1090
PUBG护甲锁链
long int 锁链 = 驱动->read<uintptr_t>(驱动->read<uintptr_t>(驱动->read<uintptr_t>(绘制.对象结构体 + 0x3B0) + 0x40) + 0x3E8);（4.3的）*/




//=========================
//类名 矩阵 世界 等数据
Gname = driver->Read<uint64_t>(driver->Read<uint64_t>(基址头 + 0xec73720) + 0x110);
long int 矩阵地址 = driver->Read<uint64_t>(driver->Read<uint64_t>(基址头 + 0xf1d5f70) + 0xC0) + 0x590;
long int 世界地址 = driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(基址头 + 0xf1fb900) + 0x810) + 0x78); // 游戏世界
long int 对象列阵 = driver->Read<uint64_t>(世界地址 + 0x30);
long int ActorArray = driver->Read<uint64_t>(对象列阵 + 0xA0);
int 对象数量 = driver->Read<int>(对象列阵 + 0xA8);
自身结构体 = driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(基址头 + 0xf1fb900) + 0x810) + 0x78) + 0x38) + 0x78) + 0x30) + 0x28c8);
int 自身队伍 = driver->Read<int>(自身结构体 + 0x998);//自身队编
// 自身fov，开火，开镜，雷达旋转值

//======================================

memset(matrix, 0, 16);
driver->Read(矩阵地址, matrix, 16 * 4);

玩家数量 = 0; 
人机数量 = 0;
AimCount = 0;
AimCount1 = 0;
AimObjCount = 0;


//==========================================================
for (int i = 0; i < 对象数量; i++) {
long int 对象结构体 = driver->Read<uint64_t>(ActorArray + 0x8 * i);// 遍历数量次数
char* s_name = GetClassName(对象结构体);

Vector3A Z;
driver->Read(driver->Read<uint64_t>(自身结构体 + 0x208) + 0x1C8, &Z, sizeof(Z)); 
Vector3A D;
driver->Read(driver->Read<uint64_t>(对象结构体 + 0x208) + 0x1C8, &D, sizeof(D));

auto CurrentVehicle = driver->Read<uint64_t>(自身结构体 + 0xE70);
Vector3A Movement;
if (CurrentVehicle) {
driver->Read(driver->Read<uint64_t>(对象结构体 + CurrentVehicle) + 0x1D8, &Movement, sizeof(Movement));// 敌人向量
} else {
driver->Read(driver->Read<uint64_t>(对象结构体 + 0x208) + 0x2C0, &Movement, sizeof(Movement));// 敌人向量
}
camera = matrix[3] * D.X + matrix[7] * D.Y + matrix[11] * D.Z + matrix[15];
r_x = px + (matrix[0] * D.X + matrix[4] * D.Y + matrix[8] * D.Z + matrix[12]) / camera * px;
r_y = py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z - 5) + matrix[13]) / camera * py;
r_w = py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z + 205) + matrix[13]) / camera * py;

float X = r_x - (r_y - r_w) / 4;
float Y = r_y;
float W = (r_y - r_w) / 2;

float MIDDLE = X + W / 2;
float BOTTOM = Y + W;
float TOP = Y - W;

float 距离 = sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;
float Distance = sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;

float left = (X + W / 2) - W / 2.6f;
float right = X + W / 1.12f;

//====================过滤物质=============

if (driver->Read<float>(对象结构体 + 0x2B78) == 479.5f) {

// 玩家名称
getUTF8(PlayerName, driver->Read<uint64_t>(对象结构体 + 0x960));
// 人机判断
bool 人机判断 = driver->Read<bool>(对象结构体 + 0xa59);
// 血量
float MinHealth = driver->Read<float>(对象结构体 + 0xE60);
float MaxHealth = driver->Read<float>(对象结构体 + 0xE64);
float iHealth = (MinHealth / MaxHealth) * 100;
//==============================================
//自身武器 = driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(自身结构体 + 0x500) + 0x1038) + 0x178); // 手持武器id
int 手持武器 = driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(对象结构体 + 0x2608) + 0x5d8) + 0x1370) + 0x1E0);
int 手持子弹 = driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(对象结构体 + 0x2608) + 0x5d8) + 0x1370) + 0x1018);
int 子弹最大 = driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(driver->Read<uint64_t>(对象结构体 + 0x2608) + 0x5d8) + 0x1370) + 0x1090);


float angle = driver->Read<float>(自身结构体 + 0xd8)-90;
Vector2A Radar = rotateCoord(angle, (Z.X - D.X) / Config.预警绘制.缩放, (Z.Y -D.Y) / Config.预警绘制.缩放);


//=============================//
int 敌人阵营 = driver->Read<int>(对象结构体 + 0x998); // 从内存中读取敌人的阵营
if (自身队伍 == 敌人阵营) { 
    continue;
}

if(Config.人物绘制.隐藏人机 && 人机判断){
continue;
}

long int Mesh = driver->Read<uint64_t>(对象结构体 + 0x510);

long int human = Mesh + 0x210;

long int Bone = driver->Read<uint64_t>(Mesh + 0x9a8) + 0x30;//骨骼
if (Bone <= 0xffff) 
continue;
int BoneCount = driver->Read<uint64_t>(Mesh + 0x9a8 + 0x8);//骨骼

FTransform meshtrans = getBone(human);
FMatrix c2wMatrix = TransformToMatrix(meshtrans);
int 头部, 胸部, 盆骨, 
    左肩膀, 右肩膀,
    左手肘, 右手肘, 
    左手腕, 右手腕, 
    左大腿, 右大腿,
    左膝盖, 右膝盖, 
    左脚腕, 右脚腕;       
if (BoneCount == 65) {
                头部 = 5;
                胸部 = 4;
                盆骨 = 0;
                左肩膀 = 11;
                右肩膀 = 32;
                左手肘 = 12;
                右手肘 = 33;
                左手腕 = 63;
                右手腕 = 62;
                左大腿 = 52;
                右大腿 = 56;
                左膝盖 = 53;
                右膝盖 = 57;
                左脚腕 = 54;
                右脚腕 = 58;                                
            } else if (BoneCount == 67) {
                头部 = 5;
                胸部 = 4;
                盆骨 = 0;
                左肩膀 = 13;
                右肩膀 = 34;
                左手肘 = 14;
                右手肘 = 35;
                左手腕 = 16;
                右手腕 = 37;
                左大腿 = 54;
                右大腿 = 58;
                左膝盖 = 55;
                右膝盖 = 59;
                左脚腕 = 56;
                右脚腕 = 60;
            } else if (BoneCount == 29) {
                头部 = 5;
                胸部 = 4;
                盆骨 = 0;
                左肩膀 = 7;
                右肩膀 = 13;
                左手肘 = 8;
                右手肘 = 14;
                左手腕 = 9;
                右手腕 = 15;
                左大腿 = 18;
                右大腿 = 21;
                左膝盖 = 19;
                右膝盖 = 22;
                左脚腕 = 20;
                右脚腕 = 23;
                }
// 头部
FTransform headtrans = getBone(Bone + 头部 * 48);
FMatrix boneMatrix = TransformToMatrix(headtrans);
Vector3A relLocation = MarixToVector(MatrixMulti(boneMatrix, c2wMatrix));
relLocation.Z += 7;// 脖子长度
Head = WorldToScreen(relLocation, matrix, camera);

// 胸部
FTransform chesttrans = getBone(Bone + 胸部 * 48);
FMatrix boneMatrix1 = TransformToMatrix(chesttrans);
Vector3A relLocation1 = MarixToVector(MatrixMulti(boneMatrix1, c2wMatrix));
Chest = WorldToScreen(relLocation1, matrix, camera);

// 盆骨
FTransform pelvistrans = getBone(Bone + 盆骨 * 48);
FMatrix boneMatrix2 = TransformToMatrix(pelvistrans);
Vector3A LrelLocation1 = MarixToVector(MatrixMulti(boneMatrix2, c2wMatrix));
Pelvis = WorldToScreen(LrelLocation1, matrix, camera);

// 左肩膀
FTransform lshtrans = getBone(Bone + 左肩膀 * 48);
FMatrix boneMatrix3 = TransformToMatrix(lshtrans);
 Vector3A relLocation2 = MarixToVector(MatrixMulti(boneMatrix3, c2wMatrix));
Left_Shoulder = WorldToScreen(relLocation2, matrix, camera);

// 右肩膀
FTransform rshtrans = getBone(Bone + 右肩膀 * 48);
FMatrix boneMatrix4 = TransformToMatrix(rshtrans);
Vector3A relLocation3 = MarixToVector(MatrixMulti(boneMatrix4, c2wMatrix));
Right_Shoulder = WorldToScreen(relLocation3, matrix, camera);

// 左手肘
FTransform lelbtrans = getBone(Bone + 左手肘 * 48);
FMatrix boneMatrix5 = TransformToMatrix(lelbtrans);
Vector3A relLocation4 = MarixToVector(MatrixMulti(boneMatrix5, c2wMatrix));
Left_Elbow = WorldToScreen(relLocation4, matrix, camera);

// 右手肘
FTransform relbtrans = getBone(Bone + 右手肘 * 48);
FMatrix boneMatrix6 = TransformToMatrix(relbtrans);
Vector3A relLocation5 = MarixToVector(MatrixMulti(boneMatrix6, c2wMatrix));
Right_Elbow = WorldToScreen(relLocation5, matrix, camera);
 
// 左手腕
FTransform lwtrans = getBone(Bone + 左手腕 * 48);
FMatrix boneMatrix7 = TransformToMatrix(lwtrans);
Vector3A relLocation6 = MarixToVector(MatrixMulti(boneMatrix7, c2wMatrix));
Left_Wrist = WorldToScreen(relLocation6, matrix, camera);

// 右手腕
FTransform rwtrans = getBone(Bone + 右手腕 * 48);
FMatrix boneMatrix8 = TransformToMatrix(rwtrans);
Vector3A relLocation7 = MarixToVector(MatrixMulti(boneMatrix8, c2wMatrix));
Right_Wrist = WorldToScreen(relLocation7, matrix, camera);

// 左大腿
FTransform Llshtrans = getBone(Bone + 左大腿 * 48);
FMatrix boneMatrix9 = TransformToMatrix(Llshtrans);
Vector3A LrelLocation2 = MarixToVector(MatrixMulti(boneMatrix9, c2wMatrix));
Left_Thigh = WorldToScreen(LrelLocation2, matrix, camera);

// 右大腿
FTransform Lrshtrans = getBone(Bone + 右大腿 * 48);
FMatrix boneMatrix10 = TransformToMatrix(Lrshtrans);
Vector3A LrelLocation3 = MarixToVector(MatrixMulti(boneMatrix10, c2wMatrix));
Right_Thigh = WorldToScreen(LrelLocation3, matrix, camera);

// 左膝盖
FTransform Llelbtrans = getBone(Bone + 左膝盖 * 48);
FMatrix boneMatrix11 = TransformToMatrix(Llelbtrans);
Vector3A LrelLocation4 = MarixToVector(MatrixMulti(boneMatrix11, c2wMatrix));
Left_Knee = WorldToScreen(LrelLocation4, matrix, camera);

// 右膝盖
FTransform Lrelbtrans = getBone(Bone + 右膝盖 * 48);
FMatrix boneMatrix12 = TransformToMatrix(Lrelbtrans);
Vector3A LrelLocation5 = MarixToVector(MatrixMulti(boneMatrix12, c2wMatrix));
Right_Knee = WorldToScreen(LrelLocation5, matrix, camera);

// 左脚腕
FTransform Llwtrans = getBone(Bone + 左脚腕 * 48);
FMatrix boneMatrix13 = TransformToMatrix(Llwtrans);
Vector3A LrelLocation6 = MarixToVector(MatrixMulti(boneMatrix13, c2wMatrix));
Left_Ankle = WorldToScreen(LrelLocation6, matrix, camera);

// 右脚腕
FTransform Lrwtrans = getBone(Bone + 右脚腕 * 48);
FMatrix boneMatrix14 = TransformToMatrix(Lrwtrans);
Vector3A LrelLocation7 = MarixToVector(MatrixMulti(boneMatrix14, c2wMatrix));
Right_Ankle = WorldToScreen(LrelLocation7, matrix, camera);

// 自瞄头部
FTransform headtransA = getBone(Bone + 5 * 48);
FMatrix boneMatrixA1 = TransformToMatrix(headtransA);
Vector3A relLocationA1 = MarixToVector(MatrixMulti(boneMatrixA1, c2wMatrix));
AimHead = WorldToScreen(relLocationA1, matrix, camera);
// 自瞄胸部
FTransform chesttransA = getBone(Bone + 4 * 48);
FMatrix boneMatrixA2 = TransformToMatrix(chesttransA);
Vector3A relLocationA2 = MarixToVector(MatrixMulti(boneMatrixA2, c2wMatrix));
AimChest = WorldToScreen(relLocationA2, matrix, camera);
// 自瞄盆骨
FTransform pelvistransA = getBone(Bone + 1 * 48);
FMatrix boneMatrixA3 = TransformToMatrix(pelvistransA);
Vector3A LrelLocationA3 = MarixToVector(MatrixMulti(boneMatrixA3, c2wMatrix));
AimPelvis = WorldToScreen(LrelLocationA3, matrix, camera);



if (W > 0){
if ((Config.自动瞄准.忽略倒地 == false || (Config.自动瞄准.忽略倒地 && MinHealth > 0)) && (Config.自动瞄准.忽略人机 == false || (Config.自动瞄准.忽略人机 && !人机判断)) && 距离 <= Config.自动瞄准.自瞄距离) {
Aim[AimCount].WodDistance = Distance;
Aim[AimCount].AimMovement = Movement;
if (Config.自动瞄准.瞄准部位 == 1.0f){
Aim[AimCount].ObjAim = relLocationA1;
Aim[AimCount].ScreenDistance = sqrt(pow(px - AimHead.X, 2) + pow(py - AimHead.Y, 2));
}else if (Config.自动瞄准.瞄准部位 == 0.0f){
Aim[AimCount].ObjAim = relLocationA2;
Aim[AimCount].ScreenDistance = sqrt(pow(px - AimChest.X, 2) + pow(py - AimChest.Y, 2));
}else if (Config.自动瞄准.瞄准部位 == 2.0f){
Aim[AimCount].ObjAim = LrelLocationA3;
Aim[AimCount].ScreenDistance = sqrt(pow(px - AimPelvis.X, 2) + pow(py - AimPelvis.Y, 2));
}
//绘制自瞄线
if (Config.自动瞄准.自瞄开关) {
    // 检查是否达到了最小瞄准次数
    if (findminat() == AimCount) {
        // 根据配置选择瞄准部位
        if (Config.自动瞄准.瞄准部位 == 1.0f) {
            // 瞄准头部
            ImGui::GetForegroundDrawList()->AddLine({px, py}, {Head.X, Head.Y}, ImColor(255, 255, 0), 1.5f);
        } else if (Config.自动瞄准.瞄准部位 == 0.0f) {
            // 瞄准胸部
            ImGui::GetForegroundDrawList()->AddLine({px, py}, {Chest.X, Chest.Y}, ImColor(255, 255, 0), 1.5f);
        } else if (Config.自动瞄准.瞄准部位 == 2.0f) {
            // 瞄准骨盆
            ImGui::GetForegroundDrawList()->AddLine({px, py}, {Pelvis.X, Pelvis.Y}, ImColor(255, 255, 0), 1.5f);
        }
    }
}
AimCount++;
}
}


 if (Config.人物绘制.绘制雷达) {
    if (距离 <= 300) {
    if (Radar.X >= -100 && Radar.X <= 100 && Radar.Y >= -100 && Radar.Y <= 100) {
    
    ImGui::GetForegroundDrawList()->AddCircleFilled({Config.预警绘制.雷达X轴 + Radar.X, Config.预警绘制.雷达Y轴 + Radar.Y}, 20 / 2, ImColor(255, 0, 0));
    
            std::string ld = "[" + std::to_string((int)距离) + "]"; 
            
            auto textSize = ImGui::CalcTextSize(ld.c_str(), nullptr, 20);

            ImGui::GetForegroundDrawList()->AddText({Config.预警绘制.雷达X轴 + Radar.X - textSize.x / 2, Config.预警绘制.雷达Y轴 + Radar.Y - 20 - 10}, ImColor(255, 255, 255), ld.c_str());
        }
    }
}




if (Config.人物绘制.背敌预警) {
    std::string ssd = std::to_string((int)距离) + "m";
    auto textSize = ImGui::CalcTextSize(ssd.c_str(), nullptr, true);

    float radius = 30.0f;
    ImColor borderColor(255, 255, 255, 225);
    ImColor fillColor = !人机判断 ? ImColor(0, 0, 0, 200) : ImColor(队伍颜色(敌人阵营));

    ImVec2 circleCenter;
    if (X + W / 2 < 0) {
        circleCenter = ImVec2(5, Head.Y);
    } else if (W > 0 && X > px * 2) {
        circleCenter = ImVec2(px * 2 - 50, Head.Y);
    } else if (W > 0 && Y + W < 0) {
        circleCenter = ImVec2(Head.X - 30, 10);
    } else if (W < 0) {
        circleCenter = ImVec2(px * 2 - Head.X - 30, py * 2 - 30);
    }

    // 绘制背景圆环
    ImGui::GetForegroundDrawList()->AddCircleFilled(circleCenter, radius, fillColor, 12);
    ImGui::GetForegroundDrawList()->AddCircle(circleCenter, radius, borderColor, 12);

    // 计算文本位置，使其位于圆环内部中心
    ImVec2 textPos = circleCenter;
    textPos.x -= textSize.x / 2;
    textPos.y -= textSize.y / 2;

    // 绘制文本
    ImGui::GetForegroundDrawList()->AddText(nullptr, 35, textPos, ImColor(255, 255, 255, 255), ssd.c_str());
}





if(W > 0) {
if (Config.人物绘制.绘制方框) {
ImGui::GetForegroundDrawList()->AddRect({X + 5.0f, TOP + 5.0f}, {X + W - 5.0f, BOTTOM - 5.0f}, ImColor(255, 255, 255, 255),0, 0, 1.3f);
}


if (Config.人物绘制.绘制射线) {
ImVec2 lineStart = {px, 0.0f};
// 圆球的中心坐标（与射线的起始点相同）
ImVec2 circleCenter = lineStart;
// 圆球的半径
float circleRadius = 10.0f;
// 绘制射线
ImGui::GetForegroundDrawList()->AddLine(lineStart, {Head.X, TOP - 50}, ImColor(255, 255, 255, 255), 1.5f);
// 绘制实心圆球
ImGui::GetForegroundDrawList()->AddCircleFilled(circleCenter, circleRadius, ImColor(255, 255, 255, 255), 0);

}


if (Config.人物绘制.绘制血量) {
    int CurHP = std::max(0, std::min((int)MinHealth, 100));
   if (iHealth == 0) {
        CurHP = 100;
    }
    ImColor healthColor = 队伍颜色(敌人阵营);

    // 计算偏移量（将偏移量方向反过来）
    float offset = -(140.0f / 2) * tan(5.0f * IM_PI / 180.0f);  // 偏移量取反

    ImVec2 topLeft(MIDDLE - 140.0f / 2 + offset, TOP - 25);
    ImVec2 topRight(MIDDLE + 140.0f / 2 + offset, TOP - 25);
    ImVec2 bottomLeft(MIDDLE - 140.0f / 2, TOP - 4);
    ImVec2 bottomRight(MIDDLE + 140.0f / 2, TOP - 4);

    ImVec2 fillTopLeft(MIDDLE - 140.0f / 2 + offset, TOP - 25);
    ImVec2 fillTopRight(MIDDLE - 140.0f / 2 + 140.0f * (float)CurHP / 100.0f + offset, TOP - 25);
    ImVec2 fillBottomLeft(MIDDLE - 140.0f / 2, TOP - 4);
    ImVec2 fillBottomRight(MIDDLE - 140.0f / 2 + 140.0f * (float)CurHP / 100.0f, TOP - 4);

    ImColor strokeColor(0, 0, 0, 255);

    // 绘制平行四边形的描边
    ImGui::GetForegroundDrawList()->AddQuad(
        topLeft, topRight, bottomRight, bottomLeft, strokeColor, .10f
    );

    // 绘制平行四边形填充条
    ImGui::GetForegroundDrawList()->AddQuadFilled(
        fillTopLeft, fillTopRight, fillBottomRight, fillBottomLeft, healthColor
    );
    if (iHealth == 0) {
ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 23.0f, ImVec2(MIDDLE - 110.0f, TOP - 23.0f), ImColor(255, 255, 255, 255), "倒地");
    }
}

// 绘制名称
if (Config.人物绘制.绘制名称) {
    string nameText = to_string((int)敌人阵营);
    if (人机判断) {
        nameText += "•RoBot";
    } else {
        nameText += "•";
        nameText += PlayerName;
    }
    // 计算文本的大小
    auto textSize = ImGui::CalcTextSize(nameText.c_str(), 0, 25);

    // 计算文本的起始位置，使其从血条左边开始绘制
    ImVec2 textPos(MIDDLE - 140.0f / 2, TOP - 10 - (textSize.y / 2)); 

    // 绘制文本
    ImGui::GetForegroundDrawList()->AddText(NULL, 24, textPos, ImColor(255, 255, 255, 255), nameText.c_str());
}



    
    if (Config.人物绘制.绘制骨骼) {
    ImColor lineColor = ImColor(255, 0, 0, 255); // 默认颜色为红色

    if (!人机判断) {
        // 如果是玩家，使用红色
        lineColor = ImColor(255, 0, 0, 255);
    } else {
        // 如果是人机，使用白色
        lineColor = ImColor(255, 255, 255, 255);
    }
    // 绘制骨骼线条，使用lineColor作为颜色
    ImGui::GetForegroundDrawList()->AddLine({Chest.X, Chest.Y}, {Pelvis.X, Pelvis.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Chest.X, Chest.Y}, {Left_Shoulder.X, Left_Shoulder.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Chest.X, Chest.Y}, {Right_Shoulder.X, Right_Shoulder.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Left_Shoulder.X, Left_Shoulder.Y}, {Left_Elbow.X, Left_Elbow.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Right_Shoulder.X, Right_Shoulder.Y}, {Right_Elbow.X, Right_Elbow.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Left_Elbow.X, Left_Elbow.Y}, {Left_Wrist.X, Left_Wrist.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Right_Elbow.X, Right_Elbow.Y}, {Right_Wrist.X, Right_Wrist.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Pelvis.X, Pelvis.Y}, {Left_Thigh.X, Left_Thigh.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Pelvis.X, Pelvis.Y}, {Right_Thigh.X, Right_Thigh.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Left_Thigh.X, Left_Thigh.Y}, {Left_Knee.X, Left_Knee.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Right_Thigh.X, Right_Thigh.Y}, {Right_Knee.X, Right_Knee.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Left_Knee.X, Left_Knee.Y}, {Left_Ankle.X, Left_Ankle.Y}, lineColor, 1.0f);
    ImGui::GetForegroundDrawList()->AddLine({Right_Knee.X, Right_Knee.Y}, {Right_Ankle.X, Right_Ankle.Y}, lineColor, 1.0f);
}





if (Config.人物绘制.绘制距离) { 
std::string s;
s += std::to_string((int)距离);
s += "M";
绘制加粗文本(25,MIDDLE - 20,BOTTOM + 10,ImColor(255,255,255,255),黑色,s.c_str());
}


if (Config.人物绘制.手持武器){
  if (手持武器 != 0) {  // 检查手持武器是否为0
        std::string s;  // 声明字符串变量
        s += GetHol(手持武器);  // 假设 GetHol 返回武器名字，注意这里传入的是手持武器的ID
        s += " [";  // 添加括号开始
        s += std::to_string((int)子弹最大);  // 添加子弹最大值
        s += "/";  // 添加分隔符
        s += std::to_string((int)手持子弹);  // 添加手持子弹数
        s += "]";  // 添加括号结束

        
    

auto textSize = ImGui::GetFont()->CalcTextSizeA(25, FLT_MAX, -1, s.c_str(), NULL, NULL); 
ImGui::GetForegroundDrawList()->AddText(NULL, 25.f, { MIDDLE - (textSize.x / 2), TOP - 38.0f - (textSize.y / 2) }, ImColor(255,255,255,255), s.c_str());
}
float png_y = 9.0f;
if (手持图片[手持武器])
ImGui::GetForegroundDrawList()->AddImage(手持图片[手持武器], {MIDDLE - 60, TOP - 80 - png_y}, {MIDDLE + 60, TOP - 40 - png_y});

}
}
if (人机判断) {
人机数量++;
}else{
玩家数量++;
}

} else {
if(W > 0) {
//类名功能
if (Config.人物绘制.盒子绘制) {
    if (strstr(s_name, "XTPlayerDeadInventoryBox_C") != nullptr || strstr(s_name, "PlayerDeadBox_C") != nullptr) {
        // 使用std::string构建文本内容
        std::string name = "舔包啊 [";
        name += std::to_string((int)距离); // 将距离转换为整数并添加到字符串中
        name += "米]";

        // 计算文本尺寸
        auto textSize = ImGui::CalcTextSize(name.c_str(), nullptr, false);

        // 绘制圆形（位于矩形的中间）
        ImGui::GetForegroundDrawList()->AddCircleFilled({MIDDLE, Y}, 5.0f, ImColor(255, 0, 0, 255));

        // 绘制背景矩形
        ImGui::GetForegroundDrawList()->AddRectFilled({MIDDLE - (82.0f / 2), Y}, {MIDDLE + (82.0f / 2), Y + 20.0f}, ImColor(147, 189, 64, 255));

        // 绘制矩形边框
        ImGui::GetForegroundDrawList()->AddRect({MIDDLE - (82.0f / 2), Y}, {MIDDLE + (82.0f / 2), Y + 20.0f}, ImColor(255, 255, 255, 255), 0, 0, 1.3f);

        // 绘制文本（使用指定的字体大小和位置）
        ImGui::GetForegroundDrawList()->AddText(NULL, 18.0f, ImVec2(MIDDLE - (textSize.x / 2) + 38, Y + (38 - textSize.y) / 2), ImColor(255, 255, 255, 255), name.c_str());

    }
}

#include "手持图片/车辆绘制.h"



if (Config.人物绘制.手雷绘制) {
    ImColor 投掷物颜色; // 初始颜色不设置
    // 检查是否包含任何投掷物名称
   bool hasGrenade = strstr(s_name, "BP_Projectile_BurnGrenade_C") != nullptr ||
                      strstr(s_name, "BP_Projectile_StunGrenade_C") != nullptr ||
                      strstr(s_name, "BP_Projectile_SmokeBomb_C") != nullptr ||
                      strstr(s_name, "BP_Projectile_FragGrenade_C") != nullptr;
                                                                  

    if (hasGrenade && 距离 <= 200.0f) { // 只显示250米以内的投掷物
        // 根据投掷物类型添加名称
        std::string ss;
        ss.reserve(256); // 预分配足够空间以避免多次分配

        if (strstr(s_name, "BP_Projectile_BurnGrenade_C")) ss += "燃烧瓶";
        if (strstr(s_name, "BP_Projectile_StunGrenade_C")) ss += "蒸鸡蛋";
        if (strstr(s_name, "BP_Projectile_SmokeBomb_C")) ss += "烟雾";
        if (strstr(s_name, "BP_Projectile_FragGrenade_C")) ss += "手雷来了";


        // 添加距离
        ss += std::to_string((int)距离) + "m";

        // 根据时间变化设置圆环颜色，使其在红白绿三个颜色之间闪烁
        float time = ImGui::GetTime();
        if (fmodf(time, 0.3f) < 0.1f) {
            投掷物颜色 = ImColor(147, 189, 64, 255); // 绿色
        } else if (fmodf(time, 0.3f) < 0.2f) {
            投掷物颜色 = ImColor(255, 255, 255, 255); // 白色
        } else {
            投掷物颜色 = ImColor(255, 0, 0, 255); // 红色
        }

        // 画成一个更大的圈（圆环），并将线变粗
        float circleRadius = W / 5;
        float circleLineWidth = 3.0f; // 增加线宽
        // 绘制圆环外圈
        ImGui::GetForegroundDrawList()->AddCircle({MIDDLE, Y}, circleRadius, 投掷物颜色, 0, circleLineWidth);

        // 计算文本尺寸并将文本放在圆环的正下方
        ImVec2 textSize2 = ImGui::CalcTextSize(ss.c_str());
        ImVec2 textPos = ImVec2(MIDDLE - (textSize2.x / 2), Y + circleRadius + 10); // 假设文本距离圆环底部10像素
        ImGui::GetForegroundDrawList()->AddText(textPos, ImColor(255, 0, 0, 255), ss.c_str());

        std::string str2 = "附近有投掷物！！！";
        ImGui::GetForegroundDrawList()->AddText(ImVec2(px - 120.0f, 160.0f), ImColor(255, 0, 0, 255), str2.c_str());
}
}
}
}
}
MaxPlayerCount = AimCount;
int AllCount = 人机数量 + 玩家数量; // 附近的玩家总数（包括人机和真人）

// 设置文本颜色
ImColor textColor;
if (玩家数量 > 0 && 人机数量 > 0) {
    // 有真人玩家和人机时颜色为红色
    textColor = ImColor(255, 0, 0, 255); // 红色
} else if (玩家数量 > 0) {
    // 只有真人玩家时颜色也为红色（或者你可以选择其他颜色以示区分）
    textColor = ImColor(255, 0, 0, 255); // 红色
} else if (人机数量 > 0) {
    // 只有人机时颜色为黄色
    textColor = ImColor(255, 188, 0, 255); // 黄色
} else {
    // 没人时颜色为白色
    textColor = ImColor(255, 255, 255, 255); // 白色
}

// 显示附近玩家总数
if (链接驱动 == true) {
    ImGui::PushFont(凯撒);
    std::string strTotalPlayers = " 0 "; // 初始化为“ 0 ”，两边有空格
    if (AllCount > 0) {
        if (AllCount < 10) { // 如果是一位数
            strTotalPlayers = " " + std::to_string(AllCount) + " "; // 将数字放在中间
        } else { // 如果是两位数或更多
            strTotalPlayers = std::to_string(AllCount); // 正常显示
            px -= 2.0f; // 往左边移2个单位
        }
    }
    ImGui::GetForegroundDrawList()->AddText(NULL, 60, ImVec2(px - 36.5f, 35 + 20), ImColor(textColor), strTotalPlayers.c_str()); // 向下移动20个单位
    ImGui::PopFont();
}

ImGui::PushFont(凯撒);
// 显示其他文本
std::string str2 = "CAESAR";
ImGui::GetForegroundDrawList()->AddText(NULL, 53, ImVec2(px - 93.0f, 90.0f + 20), ImColor(textColor), str2.c_str()); // 向下移动20个单位
ImGui::PopFont();
}
//========================图片读取==================
void DrawLogo(ImVec2 center, float size) {
ImGui::SetCursorPos({0, 180});
ImGui::SetCursorPos({0, 180});
ImDrawList *draw_list = ImGui::GetWindowDrawList();
draw_list->AddImage(FloatBall,{center.x - size /1,center.y - size / 1},{center.x + size / 1,center.y + size / 1});
}
//-----------------------------------------------------------------------------------------------------------------------------------




#include "悬浮窗/中文.h"








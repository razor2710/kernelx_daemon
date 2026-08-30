struct AimStruct {
Vector3A ObjAim;
Vector3A MyObjAim;
Vector3A AimMovement;
float ScreenDistance = 0;
float WodDistance = 0; 
char Name[32];
}Aim[100];
#ifdef AIMBOT_SEPARATED
bool IsAimLongAim = false;
char AimName[32];
int Aimchoose = (int) Config.自动瞄准.优先级;

// 遍历自瞄对象
int findminat()
{
    float min = Config.自动瞄准.自瞄范围;
    int minAt = 999;
    for (int i = 0; i < MaxPlayerCount; i++)
    {

if (Aim[i].ScreenDistance < min && Aim[i].ScreenDistance != 0)
{

    min = Aim[i].ScreenDistance;
    minAt = i;
}
    }
    if (minAt == 999)
    {
Gmin = -1;
return -1;
    }

    Gmin = minAt;
    WorldDistance = Aim[minAt].WodDistance;
    return minAt;
}
float GetPitch(float Fov)
{
if (Fov > 75 && Fov <= 130)          // 不开镜
    {
        return 0.8f * Config.自动瞄准.压枪参数;
    }
    else if (Fov == 70 || Fov == 75)    // 机瞄 ，侧瞄
    {     
        return 2.5f * Config.自动瞄准.压枪参数;       
    }
    else if (Fov == 55 || Fov == 60)    // 红点 ，全息
    {
        return 2.5f * Config.自动瞄准.压枪参数;
    }   
    else if ((int)Fov == 44)    // 2倍
    {
        return 5.1f * Config.自动瞄准.压枪参数;
    }
    else if ((int)Fov == 26)    // 3倍
    {
        return 7.1f * Config.自动瞄准.压枪参数;
    }
    else if ((int)Fov == 20)    // 4倍
    {
        return 8.4f * Config.自动瞄准.压枪参数;
    }
    else if ((int)Fov == 13)    // 6倍
    {       
        return 13.9f * Config.自动瞄准.压枪参数;
    }
	return 2.5f * Config.自动瞄准.压枪参数;   // 8倍
}
Vector2A vpvp;
float fwjl = Config.自动瞄准.自瞄范围;
float xzb = Config.自动瞄准.触摸位置X;
float yzb = Config.自动瞄准.触摸位置Y;
void AimBotAuto() {

    

    bool isDown = false;
    // 是否按下触摸
    double leenx = 0.0f;
    // x轴速度
    double leeny = 0.0f;
    // y轴速度
    double de = 1.5f;
    // 顶部不可触摸区域

    double tx = Config.自动瞄准.触摸位置X, ty = Config.自动瞄准.触摸位置Y;
    // 触摸点位置

    float SpeedMin = 2.0f;
    // 临时触摸速度

    double w = 0.0f, h = 0.0f, cmp = 0.0f;
    // 宽度 高度 正切

    double ScreenX, ScreenY;

    if (screen_x < screen_y)
    {
        ScreenX = screen_x;
        ScreenY = screen_y;
    }
    else
    {
        ScreenX = screen_y;
        ScreenY = screen_x;
    }

    // 分辨率(竖屏)PS:滑屏用的坐标是竖屏状态下的

    double ScrXH = ScreenX / 2.0f;
    // 一半屏幕X

    double ScrYH = ScreenY / 2.0f;
    // 一半屏幕X

    static float TargetX = 0;
    static float TargetY = 0;
    // 触摸目标位置

    Vector3A obj;

    float NowCoor[3];

    //自瞄触摸初始化
    TouchScreenHandle();	
    while (1)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // 生成随机浮点数
        float randomNumber = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / 20.0) - 10.0;

        yzb = Config.自动瞄准.触摸位置Y + randomNumber;

        randomNumber = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / 20.0) - 10.0;

        xzb = Config.自动瞄准.触摸位置X + randomNumber;

        usleep(1000000 / 120);

        ImGuiIO &iooi = ImGui::GetIO();

        if (Config.自动瞄准.触摸位置 && iooi.MouseDown[0] && iooi.MousePos.x <= Config.自动瞄准.触摸位置Y + Config.自动瞄准.触摸范围 && iooi.MousePos.y <= screen_y - Config.自动瞄准.触摸位置X + Config.自动瞄准.触摸范围 && iooi.MousePos.x >= Config.自动瞄准.触摸位置Y - Config.自动瞄准.触摸范围 && iooi.MousePos.y >= screen_y - Config.自动瞄准.触摸位置X - Config.自动瞄准.触摸范围)
        {
            usleep(30000);
            if (Config.自动瞄准.触摸位置 && iooi.MouseDown[0] && iooi.MousePos.x <= Config.自动瞄准.触摸位置Y + Config.自动瞄准.触摸范围 && iooi.MousePos.y <= screen_y - Config.自动瞄准.触摸位置X + Config.自动瞄准.触摸范围 && iooi.MousePos.x >= Config.自动瞄准.触摸位置Y - Config.自动瞄准.触摸范围 && iooi.MousePos.y >= screen_y - Config.自动瞄准.触摸位置X - Config.自动瞄准.触摸范围)
            {
                while (Config.自动瞄准.触摸位置 && iooi.MouseDown[0] && iooi.MousePos.x <= Config.自动瞄准.触摸位置Y + Config.自动瞄准.触摸范围 && iooi.MousePos.y <= screen_y - Config.自动瞄准.触摸位置X + Config.自动瞄准.触摸范围 && iooi.MousePos.x >= Config.自动瞄准.触摸位置Y - Config.自动瞄准.触摸范围 && iooi.MousePos.y >= screen_y - Config.自动瞄准.触摸位置X - Config.自动瞄准.触摸范围)
                {
                    Config.自动瞄准.触摸位置Y = iooi.MousePos.x;
                    Config.自动瞄准.触摸位置X = screen_y - iooi.MousePos.y;
                    TouchingColor = ImColor(0, 220, 0, 150);
                    usleep(1500);
                }
                TouchingColor = ImColor(255, 0, 0, 150);
            }
        }
		

        if (!Config.自动瞄准.自瞄开关)
        {
			IsAimLongAim = false;
            if (isDown == true)
            {
                usleep(1000);
                tx = xzb, ty = yzb;
                // 恢复变量
                Touch_Up(6);
                // 抬起
                isDown = false;
            }
            usleep(Config.自动瞄准.锁定强度 * 1000);
            usleep(randomNumber + 20);
            continue;
        }

        findminat();
        // 获取目标

        if (Gmin == -1)
        {
			IsAimLongAim = false;
            if (isDown == true)
            {

                tx = xzb, ty = yzb;
                // 恢复变量
                Touch_Up(6);
                // 抬起
                isDown = false;
            }

            continue;
        }

        float fov = driver->Read<float>(driver->Read<uint64_t>(driver->Read<uint64_t>(自身结构体 + 0x4b18) + 0x548) + 0x544);

        float ToReticleDistance = Aim[Gmin].ScreenDistance;
        float FlyTime = Aim[Gmin].WodDistance / Config.自动瞄准.子弹速度;
        float DropM = 100.0f * Config.自动瞄准.子弹下坠预判* FlyTime;
           


// 使用示例  

        int Firing;

        Firing = driver->Read<int>(自身结构体 + 0x1830); // 开火



        int Aiming = driver->Read<int>(自身结构体 + 0x1134); // 开镜

        NowCoor[0] = Aim[Gmin].ObjAim.X;
        NowCoor[1] = Aim[Gmin].ObjAim.Y;
        NowCoor[2] = Aim[Gmin].ObjAim.Z;
        obj.X = NowCoor[0] + (Aim[Gmin].AimMovement.X * FlyTime * Config.自动瞄准.预判参数);
        obj.Y = NowCoor[1] + (Aim[Gmin].AimMovement.Y * FlyTime * Config.自动瞄准.预判参数);
        obj.Z = NowCoor[2] + (Aim[Gmin].AimMovement.Z * FlyTime * Config.自动瞄准.预判参数) + DropM;

        if (Firing)
            obj.Z -= Aim[Gmin].WodDistance * Config.自动瞄准.压枪参数 * GetWeaponId(MyWeapon);

        float cameras = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];

        vpvp = WorldToScreen(obj, matrix, cameras);
        float AimDs = sqrt(pow(px - vpvp.X, 2) + pow(py - vpvp.Y, 2));


if (Config.自动瞄准.自瞄开关) {
    // 初始时使用静态范围
    AimbotFov = Config.自动瞄准.自瞄范围;

    // 如果开镜或开火，切换到动态范围 AimDs
    if (Aiming == 1 || Firing == 1) {
        AimbotFov = AimDs;
    }
}




        zm_y = vpvp.X;
        zm_x = ScreenX - vpvp.Y;


        float Aimspeace = Config.自动瞄准.瞄准速度;

        if (zm_x <= 0 || zm_x >= ScreenX || zm_y <= 0 || zm_y >= ScreenY)
        {
			IsAimLongAim = false;
            if (isDown == true)
            {
                usleep(1000);
                tx = xzb, ty = yzb;
                // 恢复变量
                Touch_Up(6);
                // 抬起
                isDown = false;
            }
            usleep(Config.自动瞄准.锁定强度 * 1000);
            usleep(randomNumber + 20);
            continue;
        }


        if (ToReticleDistance <= Config.自动瞄准.自瞄范围 || AimDs <= Config.自动瞄准.自瞄范围)
        {
            switch ((int)Config.自动瞄准.优先方式)
            {
            case 0:
                if (Firing != 1)
                {
			IsAimLongAim = false;                
                    if (isDown == true)
                    {
//             			IsAimLongAim = false;
                        usleep(1000);
                        usleep(randomNumber + 20);
                        tx = xzb, ty = yzb;
                        // 恢复变量
                        Touch_Up(6);
                        isDown = false;
                    }
                    usleep(Config.自动瞄准.锁定强度 * 1000);
                    continue;
                }
                break;
            case 1:
            

            
                if (Aiming != 1)
                {
                			IsAimLongAim = false;
                    if (isDown == true)
                    {
                        usleep(1000);
                        usleep(randomNumber + 20);
                        tx = xzb, ty = yzb;
                        // 恢复变量
                        Touch_Up(6);
                        isDown = false;
                    }
                    usleep(Config.自动瞄准.锁定强度 * 1000);
                    continue;
                }
                break;
            case 2:
                if (Firing == 0 && Aiming == 0)
                {
                			IsAimLongAim = false;
                    if (isDown == true)
                    {
                        usleep(1000);
                        usleep(randomNumber + 20);
                        tx = xzb, ty = yzb;
                        // 恢复变量
                        Touch_Up(6);
                        isDown = false;
                    }
                    usleep(Config.自动瞄准.锁定强度 * 1000);
                    continue;
                }
                break;            
            }


            float Acc = getScopeAcc((int)(90 / fov));

            if (isDown == false)
            {
                usleep(1000);
                usleep(randomNumber + 20);
                if (Config.自动瞄准.屏幕位置 == 0.0f)
                    Touch_Move(6, tx, ty);
                else
                    Touch_Move(6, screen_y - tx, screen_x - ty);
                isDown = true;
                usleep(1000);
            }

            if (zm_x > ScrXH)
            {
                TargetX = -(ScrXH - zm_x) / Config.自动瞄准.瞄准速度 * Acc;
                if (TargetX + ScrXH > ScrXH * 2)
                {
                    TargetX = 0;
                }
            }
            else if (zm_x < ScrXH)
            {
                TargetX = (zm_x - ScrXH) / Config.自动瞄准.瞄准速度 * Acc;
                if (TargetX + ScrXH < 0)
                {
                    TargetX = 0;
                }
            }

            if (zm_y > ScrYH)
            {
                TargetY = -(ScrYH - zm_y) / Config.自动瞄准.瞄准速度 * Acc;
                if (TargetY + ScrYH > ScrYH * 2)
                {
                    TargetY = 0;
                }
            }
            else if (zm_y < ScrYH)
            {
                TargetY = (zm_y - ScrYH) / Config.自动瞄准.瞄准速度 * Acc;
                if (TargetY + ScrYH < 0)
                {
                    TargetY = 0;
                }
            }

            if (TargetY >= 35 || TargetX >= 35 || TargetY <= -35 || TargetX <= -35)
            {                                                  
            if (isDown)
                {
                usleep(1000);
                usleep(randomNumber + 20);
                tx = xzb, ty = yzb;
                    Touch_Up(6);
                    isDown = false;
                }
                
                
      
                usleep(Config.自动瞄准.锁定强度 * 1000);
                continue;
            }

            tx += TargetX;
            ty += TargetY;

            if (tx >= xzb + Config.自动瞄准.触摸范围 || tx <= xzb - Config.自动瞄准.触摸范围 || ty >= yzb + Config.自动瞄准.触摸范围 || ty <= yzb - Config.自动瞄准.触摸范围)
            {

                // 只要滑屏达到了边界，直接还原至中心
                Touch_Up(6);
                // 抬起
                tx = xzb, ty = yzb;
                // 恢复变量

                usleep(randomNumber + 20);
                // 延迟
                if (!Config.自动瞄准.屏幕位置)
                    Touch_Move(6, tx, ty);
                else
                    Touch_Move(6, screen_y - tx, screen_x - ty);
                // 按下
                isDown = true;

                tx += TargetX;
                ty += TargetY;

                usleep(1000);
            }

            if (!Config.自动瞄准.屏幕位置)
                Touch_Move(6, tx, ty);
            else
                Touch_Move(6, screen_y - tx, screen_x - ty);

            isDown = true;

            usleep(Config.自动瞄准.锁定强度 * 1000);
            usleep(randomNumber + 20);
        }
        else
        {
			IsAimLongAim = false;
            if (isDown)
            {
                tx = xzb, ty = yzb;
                // 恢复变量
                Touch_Up(6);
                // 抬起
                isDown = false;
                // 延时
                usleep(Config.自动瞄准.锁定强度 * 1000);
            }
        }
    }
}
#endif
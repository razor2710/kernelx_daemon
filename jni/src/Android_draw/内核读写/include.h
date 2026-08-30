#include "函数.h"
#include <touch.h>
int Aimbotposition1;
int Aimbotposition2;
int aimCount = 0;
static float Ios[50];
string SaveDir;
FILE *numSave = nullptr;
float inttouchx = 300;
float inttouchy = 500;

typedef unsigned long S;

ImColor TouchingColor = ImColor(255, 0, 0, 150);
int L;
int 遍历;
Vector2A GGChest;
float DropM;
char PlayerName[100]; 

int 玩家数量 = 0;
int 敌人甲,自身甲,敌人包,自身包,敌人头,自身头;
int 附近玩家数量=0;
int 狗子数量 = 0;
int 人机数量 = 0;
/* 自瞄定义开始 */
float zm_x,zm_y;
void log(int num);
int AimObjCount = 0;
int AimCount1 = 0;
int WorldDistance;
int AimCount = 0;
float getScopeAcc(int Scope);
float diff[3];

int MaxPlayerCount = 0;
/* 人物数量 */
int ToReticleDistance;
/* 到准星距离 */
int Gmin=-1;
#define TIME 0.1
float LastCoor[3] = {0};
//上一个坐标
float NewCoor[3] = {0};
Vector2A AimHead;
Vector2A AimChest;
Vector2A AimPelvis;

Vector2A Head; Vector2A Chest; Vector2A Pelvis; Vector2A Left_Shoulder; Vector2A Right_Shoulder; Vector2A Left_Elbow; Vector2A Right_Elbow;  Vector2A Left_Wrist; Vector2A Right_Wrist; Vector2A Left_Thigh; Vector2A Right_Thigh;Vector2A Left_Knee;Vector2A Right_Knee;Vector2A Left_Ankle;Vector2A Right_Ankle;Vector2A Le_Ankle;Vector2A Rt_Ankle;
ImColor whiteColor = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 1.0f));

ImColor RandomColor() {
int R, G, B, A = 140;
R = (random() % 255);
G = (random() % 255);
B = (random() % 255);
return ImColor(R, G, B, A);
}

ImColor ColorArr[100];
void ColorInitialization() {
for(int i = 0; i < 100; i++) {
ColorArr[i] = RandomColor();
}
}

// 修复名称冲突: tm 改为了 alpha
static float alpha = 127/255.f;
static ImVec4 arr[] = {{144/255.f,238/255.f,144/255.f,alpha},{135/255.f,206/255.f,255/255.f,alpha},{255/255.f,0/255.f,0/255.f,alpha},{0/255.f,255/255.f,0/255.f,alpha},{0 /255.f,255/255.f, 127/255.f,alpha}
                      ,{255/255.f,182/255.f,193/255.f,alpha},{218/255.f,112/255.f,214/255.f,alpha},{248/255.f,248/255.f,255/255.f,alpha},{0/255.f,255/255.f,255/255.f,alpha},{255/255.f,165/255.f,0/255.f,alpha}
                      ,{153/255.f,204/255.f,255/255.f,alpha},{204/255.f,255/255.f,153/255.f,alpha},{255/255.f,255/255.f,153/255.f,alpha},{255/255.f,153/255.f,153/255.f,alpha},{153/255.f,153/255.f,204/255.f,alpha}
                      ,{204/255.f,204/255.f,204/255.f,alpha},{102/255.f,204/255.f,153/255.f,alpha},{255/255.f,102/255.f,0/255.f,alpha},{102/255.f,204/255.f,204/255.f,alpha},{153/255.f,204/255.f,255/255.f,alpha}
};
static int length = sizeof(arr)/16;


float GetWeaponId(int WeaponId) {
switch (WeaponId){
//突击步枪
case 101008:
return 1.2; // M762
break;
case 101001:
return 1.1; // AKM
break;
case 101004:
return 0.8; // M416
break;
case 101003:
return 0.8; // SCAR-L
break;
case 101002:
return 0.85; // M16A4
break;
case 101009:
return 1.05; // Mk47
break;
case 101006:
return 0.75; // AUG
break;
case 101005:
return 1.05; // Groza
break;
case 101010:
return 1.15; // G36C
break;
case 101007:
return 0.95; // QBZ
break;
case 101011:
return 1.0; // AC-VAL
break;
case 101012:
return 0.90; // 蜜獾突击步枪
break;
//机关枪
case 105001:
return 1.3; // M249
break;
case 105002:
return 1.3; // DP-28
case 105010:
return 1.05; // MG3
break;
//冲锋枪
case 102001:
return 0.6; // UZI
break;
case 102003:
return 0.6; // Vector
break;
case 100103:
return 0.65; // PP-19
break;
case 102007:
return 0.6; // MP5K
break;
case 102002:
return 0.6; // UMP 5
break;
case 102004:
return 0.6; // 汤姆逊
break;
case 102105:
return 0.5; // P90
break;
case 102005:
return 0.6; // 野牛
break;
default:
return 0.95; // 未收录
break;
}
return 1.0; // 未获取
}

float getScopeAcc(int Scope){
switch (Scope) {
case 0:
return 1.0f;
break;
case 1:
return .98f;
break;
case 2:
return .95f;
break;
case 3:
return .94f;
break;
case 4:
return .92f;
break;
case 5:
return .9f;
break;
case 6:
return .88f;
break;
case 7:
return .86f;
break;
case 8:
return .85f;
break;
default:
return .9f;
break;
}
}



//解密
struct ActorsEncryption {
    uint64_t Enc_1, Enc_2;
    uint64_t Enc_3, Enc_4;
};

struct Encryption_Chunk {
    uint32_t val_1, val_2, val_3, val_4;
    uint32_t val_5, val_6, val_7, val_8;
};

uint64_t DecryptActorsArray(uint64_t uLevel, int Actors_Offset, int EncryptedActors_Offset) {
    if (uLevel < 0x10000000)
        return 0;

    if (driver->Read<uint64_t>(uLevel + Actors_Offset) > 0)
        return uLevel + Actors_Offset;

    if (driver->Read<uint64_t>(uLevel + EncryptedActors_Offset) > 0)
        return uLevel + EncryptedActors_Offset;

    auto Encryption = driver->Read<ActorsEncryption>(uLevel + EncryptedActors_Offset + 0x10);

    if (Encryption.Enc_1 > 0) {
        auto Enc = driver->Read<Encryption_Chunk>(Encryption.Enc_1 + 0x80);
        return (((driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_1)
                  | (driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_2) << 8))
                 | (driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_3) << 0x10)) & 0xFFFFFF
                | ((uint64_t) driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_4) << 0x18)
                | ((uint64_t) driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_5) << 0x20)) &
               0xFFFF00FFFFFFFFFF
               | ((uint64_t) driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_6) << 0x28)
               | ((uint64_t) driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_7) << 0x30)
               | ((uint64_t) driver->Read<uint8_t>(Encryption.Enc_1 + Enc.val_8) << 0x38);
    } else if (Encryption.Enc_2 > 0) {
        auto Encrypted_Actors = driver->Read<uint64_t>(Encryption.Enc_2);
        if (Encrypted_Actors > 0) {
            return (uint16_t) (Encrypted_Actors - 0x400) & 0xFF00
                   | (uint8_t) (Encrypted_Actors - 0x04)
                   | (Encrypted_Actors + 0xFC0000) & 0xFF0000
                   | (Encrypted_Actors - 0x4000000) & 0xFF000000
                   | (Encrypted_Actors + 0xFC00000000) & 0xFF00000000
                   | (Encrypted_Actors + 0xFC0000000000) & 0xFF0000000000
                   | (Encrypted_Actors + 0xFC000000000000) & 0xFF000000000000
                   | (Encrypted_Actors - 0x400000000000000) & 0xFF00000000000000;
        }
    } else if (Encryption.Enc_3 > 0) {
        auto Encrypted_Actors = driver->Read<uint64_t>(Encryption.Enc_3);
        if (Encrypted_Actors > 0)
            return (Encrypted_Actors >> 0x38) | (Encrypted_Actors << (64 - 0x38));
    } else if (Encryption.Enc_4 > 0) {
        auto Encrypted_Actors = driver->Read<uint64_t>(Encryption.Enc_4);
        if (Encrypted_Actors > 0)
            return Encrypted_Actors ^ 0xCDCD00;
    }
    return 0;
}

template<typename T>
struct TArray {
    uintptr_t base;
    int32_t count;
    int32_t max;
    std::vector<T> ToVec() const {
        if (!IsValid()) {
            return {};
        }
        std::vector<T> vec{};
        vec.resize(static_cast<size_t>(count));
        iovec l_iov{&vec[0], static_cast<size_t>(count) * sizeof(T)};
        iovec r_iov{reinterpret_cast<void *>(base), static_cast<size_t>(count) * sizeof(T)};
        return vec;
    }
    T operator[](size_t u) const {
        return driver->Read<T>(base + u * sizeof(T));
    }
    bool IsValid() const {
        return base && count > 0 && count <= max && max > 0;
    }
};
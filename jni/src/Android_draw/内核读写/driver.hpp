#ifndef DRAW_HPP
#define DRAW_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#include <malloc.h>
#include <thread>
#include <sys/mman.h>
#include <sys/uio.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <locale>
#include <string>
#include <dlfcn.h>
#include <regex.h>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <inttypes.h>

#include "字体.h"
#include "字体1.h"

long int 自身结构体, 基址头;
pid_t 进程 = 0;

// ==========================================
// ⭐ KPM IOCTL STRUCTURES
// ==========================================
struct kpm_mem_req {
    int32_t  pid;
    uint64_t remote_addr;
    uint64_t local_addr;
    uint64_t size;
    uint64_t transferred;
};

#define KPM_MEM_MAGIC 'M'
#define KPM_MEM_READ  _IOWR(KPM_MEM_MAGIC, 1, struct kpm_mem_req)


// ==========================================
// 🎮 MAIN DRIVER CLASS
// ==========================================
class c_driver {
private:
    int fd = -1;       
    pid_t pid = 0;     

public:
    c_driver() {
        // 瞬间打开 /dev/null，无需菜单交互
        fd = open("/dev/null", O_RDWR);
        
        if (fd < 0) {
            printf("\033[31;1m[FATAL] Failed to open /dev/null! KPM driver is not running.\033[0m\n");
            exit(1);
        }
    }

    ~c_driver() {
        if (fd > 0) {
            close(fd);
        }
    }

    void initialize(pid_t pid) {
        this->pid = pid;
    }

    // ==========================================
    // ⭐ KPM MEMORY READ (WITH SHORT LOGGING)
    // ==========================================
    bool Read(uintptr_t addr, void *buffer, size_t size, int read_write = 0) {
        if (fd < 0 || this->pid <= 0 || buffer == nullptr) {
            return false;
        }

        struct kpm_mem_req req;
        memset(&req, 0, sizeof(req));
        
        req.pid = (int32_t)this->pid;
        req.remote_addr = (uint64_t)addr;
        req.local_addr = (uint64_t)(uintptr_t)buffer;
        req.size = (uint64_t)size;
        req.transferred = 0;

        int ret = ioctl(fd, KPM_MEM_READ, &req);
        bool success = (ret >= 0);

        //printf("[RD] 0x%lx | sz:%zu | val:0x%08x\n", (unsigned long)addr, size, *(uint32_t*)buffer);

        // --- SHORT LOGGING ---
        // Prints Address | Request Size | Returned Hex Value (or status)
        //if (success && size >= 4) {
        //    printf("[Read] 0x%lx | sz:%zu | val:0x%08x\n", (unsigned long)addr, size, *(uint32_t*)buffer);
        //} else {
        //    printf("[Read] 0x%lx | sz:%zu | ok:%d\n", (unsigned long)addr, size, success);
        //}
        // ---------------------

        return success;
    }

    // ==========================================
    // ⭐ KPM MEMORY WRITE (DISABLED)
    // ==========================================
    template <typename T>
    T Write(uintptr_t addr, T res) {
        // KPM目前不支持写入，返回空以防止崩溃
        return {};
    }

    template <typename T>
    T Read(uintptr_t addr) {
        T res{};
        if (this->Read(addr, &res, sizeof(T)))
            return res;
        return res;
    }

    template<typename... s>
    uintptr_t GetPointer(uintptr_t address, s... args) {
        int count = 0;
        uintptr_t last_address = 0;
        int array[] = {(readcount(&count, args))...};
        this->Read(address + array[0], &last_address);
        for (int i = 1; i < count; i++) {
            if (i == count - 1) {
                last_address += array[i];
                return last_address;
            }
            this->Read(last_address + array[i], &last_address);
        }
        return last_address;
    }

    uintptr_t get_module_base(const char *name) {
        FILE *fp;
        long addr = 0;
        char *pch;
        char filename[64];
        char line[1024];
        snprintf(filename, sizeof(filename), "/proc/%d/maps", this->pid);
        fp = fopen(filename, "r");
        if (fp != NULL) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, name)) {
                    pch = strtok(line, "-");
                    addr = strtoul(pch, NULL, 16);
                    if (addr == 0x8000) addr = 0;
                    break;
                }
            }
            fclose(fp);
        }
        
        if (addr == 0 || addr == 0x8000) {
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "su -c 'cat /proc/%d/maps | grep %s'", this->pid, name);
            fp = popen(cmd, "r");
            if (fp != NULL) {
                while (fgets(line, sizeof(line), fp)) {
                    if (strstr(line, name)) {
                        pch = strtok(line, "-");
                        addr = strtoul(pch, NULL, 16);
                        break;
                    }
                }
                pclose(fp);
            }
        }
        return (addr == 0x8000) ? 0 : addr;
    }

    pid_t GetPID(const char* name) {
        FILE* fp;
        pid_t p = 0;
        char cmd[0x100];
        snprintf(cmd, sizeof(cmd), "pidof %s", name);
        fp = popen(cmd,"r");
        if(fp) {
            if (fscanf(fp,"%d", &p) != 1) p = 0;
            pclose(fp);
        }
        return p;
    }

    std::string readString(uintptr_t addr, size_t max_len = 256) {
        std::string result;
        char buffer[128];
        size_t read_count = 0;
        while (read_count < max_len) {
            size_t to_read = std::min(sizeof(buffer), max_len - read_count);
            if (!Read(addr + read_count, buffer, to_read)) break;
            for (size_t i = 0; i < to_read; ++i) {
                if (buffer[i] == '\0') {
                    result.append(buffer, i);
                    return result;
                }
            }
            result.append(buffer, to_read);
            read_count += to_read;
        }
        return result;
    }
};

static c_driver *driver = new c_driver();

/*--------------------------------------------------------------------------------------------------------*/

pid_t pid;

int GetPID(char* PackageName)
{
    FILE* fp;
    char cmd[0x100] = "pidof ";
    strcat(cmd, PackageName);
    fp = popen(cmd,"r");
    fscanf(fp,"%d", &pid);
    pclose(fp);
    if (pid > 0)
    {
        driver->initialize(pid);
    }
    return pid;
}

bool PidExamIne()
{
    char path[128];
    sprintf(path, "/proc/%d",pid);
    if (access(path,F_OK) != 0)
    {
        printf("\033[31;1m");
        puts("获取进程PID失败!");
        exit(1);
    }
    return true;
}

long GetModuleBase(char* module_name)
{
    uintptr_t base=0;
    base = driver->get_module_base(module_name);
    return base;
}

long ReadValue(long addr)
{
    long he = 0;
    if (addr < 0xFFFFFFFF) {
        driver->Read(addr, &he, 4);
    } else {
        driver->Read(addr, &he, 8);
    }
    return he;
}

long ReadDword(long addr)
{
    long he = 0;
    driver->Read(addr, &he, 4);
    return he;
}

float ReadFloat(long addr)
{
    float he = 0;
    driver->Read(addr, &he, 4);
    return he;
}

int WriteDword(long int addr, int value)
{
    driver->Write<int>(addr, value);
    return 0;
}

int WriteFloat(long int addr, float value)
{
    driver->Write<float>(addr, value);
    return 0;
}

#endif // DRAW_HPP
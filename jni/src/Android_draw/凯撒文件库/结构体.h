


void ImGuiLayout1();
void ImGuiLayout2();
void ImGuiLayout3();
void ImGuiLayout4();
ImColor 物资颜色;
int MenuTab = 1;
char *BoxName;
bool MemuSwitch = true;
bool BallSwitch = true;
ImGuiWindow *Window = nullptr;
void DrawLogo(ImVec2 center, float size);
bool IsLoGin = true, IsDown = false, IsWin = true;
ImVec2 ImagePos = {0, 0};

ImVec2 aimsuspensionPos;// NOLINT

bool isSetAim = false;
bool isAimDown = false;

ImTextureID FloatBall;

static bool 链接驱动;

int MyWeapon;//自身武器


//射线
enum LINE {
line1 = 0,
line2 = 1,
line3 = 2
};
static LINE LINE;
//方框
enum BOX {
box1 = 0,
box2 = 1,
box3 = 2
};
static BOX BOX;
//自瞄部位
enum 自瞄触发方式 {
开火 = 0,
开镜 = 1,
开火开镜 = 2
};

struct sConfig {
//人物绘制
struct 人物绘制 {
bool 绘制方框;
bool 绘制射线;
bool 绘制骨骼;
bool 绘制名称;
bool 绘制距离;
bool 绘制血量;
bool 绘制雷达;
bool 手持武器;
bool 盒子物资;
bool 盒子绘制;
bool 载具绘制;
bool 手雷绘制;
bool 背敌预警;
bool 隐藏人机;
bool 隐藏Draw;
bool 无后台;
};
人物绘制 人物绘制{0};
//绘制载具
//绘制预警






struct 预警绘制 {
bool 绘制雷达;
bool 手雷预警;
bool 背敌预警;
float 雷达X轴; 
float 雷达Y轴; 
float 雷达大小;
float 缩放;
float 雷达样式;
float 菜单X轴;//Config.预警绘制. 菜单X轴
float 菜单Y轴;//Config.预警绘制. 菜单Y轴
};
预警绘制 预警绘制{0};
//自瞄
struct 自动瞄准 {
bool 自瞄开关;
bool 触摸位置;
bool 动态范围;
bool 音量快捷;
bool 隐藏范围;
float 自瞄范围; 
bool 持续锁定;
bool 忽略人机;
bool 忽略倒地;
float 优先方式;
float 瞄准部位;
float 腰射距离;
float 锁定强度;
float 瞄准速度;
float 触摸范围;
float 触摸位置X;
float 触摸位置Y;
float 子弹速度;
float 压枪参数;
float 预判参数;
float 优先级;
float 屏幕位置;
float 子弹下坠预判;
float 自瞄距离;
float 语言;
bool 防录屏;
bool 性能模式;
bool 显示帧率;
bool 触摸驱动;
};
自动瞄准 自动瞄准{0};
};
sConfig Config{0};

static float AimbotFov = Config.自动瞄准.自瞄范围;

struct 颜色数据 {
ImVec4 设计颜色;
};
颜色数据 颜色[100];
ImVec4 设计颜色[100];
int 拼接(int a,int b) {
int t=9;
while(t<b)t=t*10+9;
t=(t+1)*a+b;
return t;
}
ImVec4 随机(){
int 红;
int 绿;
int 蓝;
红=(rand()%255+0);
绿=(rand()%255+0);
蓝=(rand()%255+0);
if(sizeof(红)==1||sizeof(红)==0)
红=拼接(红,红);
if(sizeof(绿)==1||sizeof(绿)==0)
绿=拼接(绿,绿);
if(sizeof(蓝)==1||sizeof(蓝)==0)
蓝=拼接(蓝,蓝);
return {(float)红/255.0f,(float)绿/255.0f,(float)蓝/255.0f,174/255.0f};
}
void 颜色初始化(){
srand((unsigned)time(NULL)); /*随机数*/
for(int 数量=0;数量<100;数量++){
设计颜色[数量]=随机();
}
}
ImVec4 获取颜色(int 序号){
return 设计颜色[序号-1];
}
ImVec4 队伍颜色(int 队伍){
    if(队伍<=100 && 队伍>0)
        return 获取颜色(队伍);
    else
        return {0/255.f,255/255.f,0/255.f,85/255.f};
}



bool isContain(string str, string check)
{
	return (str.find(check) != string::npos);
}


#include <array>
void DrawBone(ImVec2 start, ImVec2 end, bool Cansee)
{
     if(!isnan(start.x)&&!isnan(start.y)&&!isnan(end.x)&&!isnan(end.x)){
    double dx = start.x - end.x;
    double dy = start.y - end.y;
    if(std::sqrt(dx * dx + dy * dy)>100)
    return;
	if (Cansee) {
		ImGui::GetForegroundDrawList()->AddLine(start, end, 颜色1, {2});
	} else if (!Cansee){
		ImGui::GetForegroundDrawList()->AddLine(start, end, 颜色1, {2});
	}
	}
	
}
//预警图标。
void OffScreen(Vector2A Obj, float camear, ImColor color, float Radius)
{
	ImRect screen_rect = {0.0f, 0.0f, static_cast<float>(px * 2), static_cast<float>(py * 2)};
	auto screen_center = screen_rect.GetCenter();
	auto angle = atan2(screen_center.y - Obj.Y, screen_center.x - Obj.X);
	angle += camear > 0 ? M_PI : 0.0f;
	Vector2A arrow_center {
			screen_center.x + Radius * cosf(angle),
			screen_center.y + Radius * sinf(angle)
	};
	std::array<ImVec2, 4>points {
			ImVec2(-22.0f, -8.6f),
			ImVec2(0.0f, 0.0f),
			ImVec2(-22.0f, 8.6f),
			ImVec2(-18.0f, 0.0f)
	};
	for (auto & point : points)
	{
		auto x = point.x * 1.155f;
		auto y = point.y * 1.155f;
		point.x = arrow_center.X + x * cosf(angle) - y * sinf(angle);
		point.y = arrow_center.Y + x * sinf(angle) + y * cosf(angle);
	}
	float alpha = 1.0f;
	if (camear > 0)
	{
		constexpr float nearThreshold = 200 * 200;
		ImVec2 screen_outer_diff = {
				Obj.X < 0 ? abs(Obj.X) : (Obj.X > screen_rect.Max.x ? Obj.X - screen_rect.Max.x : 0.0f),
				Obj.Y < 0 ? abs(Obj.Y) : (Obj.Y > screen_rect.Max.y ? Obj.Y - screen_rect.Max.y : 0.0f),
		};
		float distance = static_cast<float>(pow(screen_outer_diff.x, 2) + pow(screen_outer_diff.y, 2));
		alpha = camear < 0 ? 1.0f : (distance / nearThreshold);
	}
	ImColor arrowColor = color;
	arrowColor.Value.w = std::min(alpha, 1.0f);
	ImGui::GetBackgroundDrawList()->AddTriangleFilled(points[0], points[1], points[3], arrowColor);
	ImGui::GetBackgroundDrawList()->AddTriangleFilled(points[2], points[1], points[3], arrowColor);
	ImGui::GetBackgroundDrawList()->AddQuad(points[0], points[1], points[2], points[3], ImColor(0.0f, 0.0f, 0.0f, alpha), 1.335f);
}



void SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = NULL) {
    *v = ImClamp(*v, v_min, v_max);
    
    ImGui::SliderFloat(label, v, v_min, v_max, format); // 创建滑条
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 13.0f)); // 缩小上下间距

}
//=========<=======手雷

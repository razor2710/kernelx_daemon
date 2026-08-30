if (Config.人物绘制.载具绘制) {
   if (距离 > 200.0f){
    continue;
    }

    std::string name;
    if (strstr(s_name, "VH_Scooter_C") != nullptr) {
        name = "小绵羊";
    } else if (strstr(s_name, "VH_Motorcycle_1_C") != nullptr) {
        name = "摩托";
    } else if (strstr(s_name, "VH_Snowbike_C") != nullptr) {
        name = "雪地摩托";
    } else if (strstr(s_name, "VH_Snowmobile_C") != nullptr) {
        name = "雪地滑板";
    } else if (strstr(s_name, "VH_MotorcycleCart_1_C") != nullptr) {
        name = "三轮摩托";
    } else if (strstr(s_name, "BP_VH_Tuk_1_C") != nullptr) {
        name = "三轮车";
    } else if (strstr(s_name, "VH_ATV1_C") != nullptr) {
        name = "雪地摩托";
    } else if (strstr(s_name, "BP_VH_Buggy_2_C") != nullptr) {
        name = "蹦蹦";
    } else if (strstr(s_name, "Mirado_open_4_C") != nullptr) {
        name = "敞篷跑车";
    } else if (strstr(s_name, "BP_CoupeRB_Base_C") != nullptr) {
        name = "跑车";
    } else if (strstr(s_name, "VH_Dacia_C") != nullptr) {
        name = "轿车";
    } else if (strstr(s_name, "Rony_01_C") != nullptr) {
        name = "皮卡车";
    } else if (strstr(s_name, "PickUp_07_C") != nullptr) {
        name = "皮卡车";
    } else if (strstr(s_name, "VH_UAZ01_C") != nullptr) {
        name = "吉普车";
    } else if (strstr(s_name, "BP_VH_Bigfoot_C") != nullptr) {
        name = "大脚车";
    } else if (strstr(s_name, "VH_UTV_C") != nullptr) {
        name = "全地形车";
    } else if (strstr(s_name, "VH_MiniBus_01_C") != nullptr) {
        name = "大巴车";
    } else if (strstr(s_name, "VH_BRDM_C") != nullptr) {
        name = "蟑螂车";
    } else if (strstr(s_name, "VH_Motorglider_C") != nullptr) {
        name = "滑翔机";
    } else if (strstr(s_name, "TrackVehicle_BP_C") != nullptr) {
        name = "矿车";
    }

    // 如果 name 不为空，则进行绘制
    if (!name.empty()) {
        name += " [";
        name += std::to_string((int)距离); // 将距离转换为整数并添加到字符串中
        name += "米]";

        ImGui::GetForegroundDrawList()->AddText(NULL, 25.0f, ImVec2(MIDDLE, Y), ImColor(255, 255, 255, 255), name.c_str());
    }
}

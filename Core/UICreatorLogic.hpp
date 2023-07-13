#pragma once

#include <imgui.h>
#include <imfilebrowser.h>
#include <functional>
#include <map>
#include <TextEditor.h>

class UICreatorFormProperties
{
    ImGui::FileBrowser fb;
    void FormProps(void* ptr);
    void ButtonProps(void* ptr);
    void ImageButtonProps(void* ptr);
    void SliderIntProps(void* ptr);
    void SliderInt2Props(void* ptr);
    void SliderInt3Props(void* ptr);
    void SliderInt4Props(void* ptr);
    void SliderFloatProps(void* ptr);
    void SliderFloat2Props(void* ptr);
    void SliderFloat3Props(void* ptr);
    void SliderFloat4Props(void* ptr);
    void InputTextProps(void* ptr);
    std::map<std::string, std::function<void(void*)>>props_func_table;
    void* last_active_widget = nullptr;
    TextEditor editor;
public:
    UICreatorFormProperties();
    ~UICreatorFormProperties() = default;
    void draw();
};

class UICreatorMain {
    struct FormCreatorData {
        std::string form_name;
        int form_pos[2] = {0, 0};
        int form_size[2] = {0, 0};
        bool open_form_creator = false;
        bool widget_toolbox = false;
    }form_creator_data;
    void NewForm(void);
    void FormAddWidget(void);
    int last_widget_id = -1;
    std::map<std::string, std::function<void* ()>>creator_func_table;
public:
    UICreatorMain();
    void draw();
};

void UIKitCreateForm(std::string);
void UIKitGetForm(std::string, void*&);
void UIKitDrawForm(std::string name);
void UIKitDrawForm(void);
std::vector<void*> UIKitGetForm(void);
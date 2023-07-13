#pragma once

#include <imgui.h>
#include <iostream>
#include <functional>
#include <filesystem>

typedef std::function<void(void*)> widget_callback;
static void* last_active = nullptr;
static bool last_active_form = false;
void UIKitSetEditMode(bool v = true);

bool UIKitGetEditMode(void);

struct WidgetProps {
	std::string title;
	int x = 0, y = 0, w = 0, h = 0;
	bool fixed_position = false;
	bool fixed_size = false;
	bool opened = true;
	bool active = true;
	bool pressed = false;
};

class Widget : public WidgetProps {
protected:
	widget_callback callback;
public:
	Widget(std::string title, widget_callback callback);
	virtual void ondraw(void) = 0;
	void setCallback(widget_callback callback);
	void inEditMode(void);
	void hoverRect(const ImVec4& color);
	std::string widget_type = "none";
	std::string callback_code;
	void* form_meta = nullptr;
};

class UIKitButton : public Widget {
public:
	UIKitButton(std::string title, widget_callback callback);
	UIKitButton& setPosition(int x, int y);
	UIKitButton& setSize(int w, int h);
	UIKitButton& setFixedPosition(bool f_pos = true);
	UIKitButton& setFixedSize(bool f_size = true);
	// ReSharper disable once IdentifierTypo
	void ondraw(void);
};

class UIKitImageButton : public Widget {
	std::filesystem::path src;
	ImTextureID image_id = 0;
public:
	UIKitImageButton(std::string title, widget_callback callback);
	~UIKitImageButton();
	UIKitImageButton& setImage(std::filesystem::path path);
	UIKitImageButton& setPosition(int x, int y);
	UIKitImageButton& setSize(int w, int h);
	UIKitImageButton& setFixedPosition(bool f_pos = true);
	UIKitImageButton& setFixedSize(bool f_size = true);
	void ondraw(void);
	const std::string getIMGSrc();
};

template <class T>
class UIKitSlider : public  Widget {
protected:
	T _min_val = 0;
	T _max_val = 100;
	T* current_val = nullptr;
public:
	UIKitSlider(std::string title, widget_callback callback);
	UIKitSlider(std::string title, widget_callback callback, T& ref_val);
	UIKitSlider& setMax(T _m);
	UIKitSlider& setMin(T _m);
	UIKitSlider<T>& setPosition(int x, int y);
	UIKitSlider<T>& setFixedPosition(bool f_pos = true);
	T getMin();
	T getMax();
	T& getValue(void);
	void ondraw(void);
};

template <class T>
class UIKitSlider2 : public Widget {
protected:
	T _min_val = 0;
	T _max_val = 100;
	T* current_val = nullptr;
public:
	UIKitSlider2(std::string title, widget_callback callback);
	UIKitSlider2(std::string title, widget_callback callback, T* ref_val);
	UIKitSlider2& setMax(T _m);
	UIKitSlider2& setMin(T _m);
	UIKitSlider2<T>& setPosition(int x, int y);
	UIKitSlider2<T>& setFixedPosition(bool f_pos = true);
	T* getValue(void);
	T getMin();
	T getMax();
	void ondraw(void);
};

template <class T>
class UIKitSlider3 : public Widget {
protected:
	T _min_val = 0;
	T _max_val = 100;
	T* current_val = nullptr;
public:
	UIKitSlider3(std::string title, widget_callback callback);
	UIKitSlider3(std::string title, widget_callback callback, T* ref_val);
	UIKitSlider3& setMax(T _m);
	UIKitSlider3& setMin(T _m);
	UIKitSlider3<T>& setPosition(int x, int y);
	UIKitSlider3<T>& setFixedPosition(bool f_pos = true);
	T* getValue(void);
	T getMin();
	T getMax();
	void ondraw(void);
};

template <class T>
class UIKitSlider4 : public Widget {
protected:
	T _min_val = 0;
	T _max_val = 100;
	T* current_val = nullptr;
public:
	UIKitSlider4(std::string title, widget_callback callback);
	UIKitSlider4(std::string title, widget_callback callback, T* ref_val);
	UIKitSlider4& setMax(T _m);
	UIKitSlider4& setMin(T _m);
	T* getValue(void);
	UIKitSlider4<T>& setPosition(int x, int y);
	UIKitSlider4<T>& setFixedPosition(bool f_pos = true);
	T getMin();
	T getMax();
	void ondraw(void);
};

class UIKitInputText : public Widget {
	std::string* current_val = nullptr;
public:
	UIKitInputText(std::string title, widget_callback callback);
	UIKitInputText(std::string title, widget_callback callback, std::string* ref_val);
	std::string& getText();
	void ondraw(void);
	bool is_password = false;
};

class UIKitForm : public WidgetProps {
	ImGuiWindowFlags form_flags = 0;
	std::list<void*>inner;
	std::unordered_map<ImGuiWindowFlags, bool>form_flags_map = {
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoBringToFrontOnFocus, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoFocusOnAppearing, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoMove, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoResize, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoInputs, false),
			std::make_pair(ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings, false),
	};
public:
	UIKitForm(const std::string& title, int x = 0, int y = 0, int w = 0, int h = 0);
	~UIKitForm(void);
	UIKitForm& setPosition(int x, int y);
	UIKitForm& setSize(int w, int h);
	UIKitForm& addFlag(ImGuiWindowFlags flag);
	UIKitForm& setFixedPosition(bool f_pos = true);
	UIKitForm& setFixedSize(bool f_size = true);
	UIKitForm& addWidget(void* w);
	UIKitForm& draw(void);
	bool checkFlag(ImGuiWindowFlags flag);
	UIKitForm& popFlag(ImGuiWindowFlags flag);
	std::optional<void*> operator[](std::string name);
	std::list<void*>get_widgets(void);
	const std::unordered_map<ImGuiWindowFlags, bool>GetFlagStatus(void) const;
};
#include "UICore.hpp"
#include "VulkanImageLoader.hpp"
#include <ranges>
#include <imgui_stdlib.h>
#define CF(v) static_cast<float>(v)

static bool edit_mode = false;

void UIKitSetEditMode(bool v) {
	edit_mode = v;
}

bool UIKitGetEditMode(void) {
	return edit_mode;
}

Widget::Widget(std::string title, widget_callback callback) :
	callback(callback) {
}

void Widget::setCallback(widget_callback callback) {
	this->callback = callback;
}

void Widget::inEditMode(void) {
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseDragging(0)) {
		auto m_pos = ImGui::GetMousePos();
		auto w_pos = ImGui::GetWindowPos();
		auto size = ImGui::GetItemRectSize();
		m_pos.x -= w_pos.x;
		m_pos.y -= w_pos.y;
		x = m_pos.x - size.x / 2;
		y = m_pos.y - size.y / 2;
	}
}

void Widget::hoverRect(const ImVec4& color) {
	auto window = ImGui::GetWindowDrawList();
	window->AddRect(ImVec2(x, y), ImVec2(w, h), IM_COL32(color.x * 255,
		color.y * 255,
		color.z * 255,
		color.w * 255));
}

UIKitButton::UIKitButton(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	this->widget_type = "button";
}

UIKitButton& UIKitButton::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

UIKitButton& UIKitButton::setSize(int w, int h) {
	this->w = w;
	this->h = h;
	return *this;
}

UIKitButton& UIKitButton::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

UIKitButton& UIKitButton::setFixedSize(bool f_size) {
	fixed_size = f_size;
	return *this;
}

void UIKitButton::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
		x = fixed_position || edit_mode ? x : 0;
		y = fixed_position || edit_mode ? y : 0;
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
		fixed_position = true;
	}
	if (!edit_mode) {
		if (ImGui::Button(title.c_str(), ImVec2(CF(w), CF(h)))) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		ImGui::SetCursorPos(ImVec2(x, y));
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::Button(title.c_str(), ImVec2(CF(w), CF(h)));
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(160, 50, 190, 255), 0, 0, 2.0f);
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

UIKitImageButton::UIKitImageButton(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	this->widget_type = "image_button";
}

UIKitImageButton::~UIKitImageButton() {
	if (!src.empty()) {
		UnloadImage(image_id);
	}
}

UIKitImageButton& UIKitImageButton::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

UIKitImageButton& UIKitImageButton::setSize(int w, int h) {
	this->w = w;
	this->h = h;
	return *this;
}

UIKitImageButton& UIKitImageButton::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

UIKitImageButton& UIKitImageButton::setFixedSize(bool f_size) {
	fixed_size = f_size;
	return *this;
}

UIKitImageButton& UIKitImageButton::setImage(std::filesystem::path path) {
	if (image_id) UnloadImage(image_id);
	image_id = LoadImage(path);
	src = path;
	return *this;
}

void UIKitImageButton::ondraw(void) {
	if (image_id == 0) return;
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
		fixed_position = true;
	}
	if (!edit_mode) {
		if (ImGui::ImageButton(image_id, ImVec2(CF(w), CF(h)))) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::ImageButton(image_id, ImVec2(CF(w), CF(h)));
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

const std::string UIKitImageButton::getIMGSrc()
{
	return std::string(src.string());
}

template <>
UIKitSlider<int>::UIKitSlider(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new int;
	this->widget_type = "slider_int";
}

template <>
UIKitSlider<int>::UIKitSlider(std::string title, widget_callback callback, int& ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = &ref_val;
	this->widget_type = "slider_int";
}

template <>
UIKitSlider<int>& UIKitSlider<int>::setMax(int _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider<int>& UIKitSlider<int>::setMin(int _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider<int>& UIKitSlider<int>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider<int>& UIKitSlider<int>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template<>
int UIKitSlider<int>::getMin()
{
	return _min_val;
}

template<>
int UIKitSlider<int>::getMax()
{
	return _max_val;
}

template <>
int& UIKitSlider<int>::getValue(void) {
	return *current_val;
}

template <>
void UIKitSlider<int>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderInt(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderInt(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}


template <>
UIKitSlider2<int>::UIKitSlider2(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new int;
	this->widget_type = "slider_int2";
}

template <>
UIKitSlider2<int>::UIKitSlider2(std::string title, widget_callback callback, int* ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = ref_val;
	this->widget_type = "slider_int2";
}

template<>
int UIKitSlider2<int>::getMin()
{
	return _min_val;
}

template<>
int UIKitSlider2<int>::getMax()
{
	return _max_val;
}

template <>
UIKitSlider2<int>& UIKitSlider2<int>::setMax(int _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider2<int>& UIKitSlider2<int>::setMin(int _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider2<int>& UIKitSlider2<int>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider2<int>& UIKitSlider2<int>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template <>
int* UIKitSlider2<int>::getValue(void) {
	return current_val;
}

template <>
void UIKitSlider2<int>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderInt2(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderInt2(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

template <>
UIKitSlider3<int>::UIKitSlider3(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new int;
	this->widget_type = "slider_int3";
}

template <>
UIKitSlider3<int>::UIKitSlider3(std::string title, widget_callback callback, int* ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = ref_val;
	this->widget_type = "slider_int3";
}

template<>
int UIKitSlider3<int>::getMin()
{
	return _min_val;
}

template<>
int UIKitSlider3<int>::getMax()
{
	return _max_val;
}

template <>
UIKitSlider3<int>& UIKitSlider3<int>::setMax(int _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider3<int>& UIKitSlider3<int>::setMin(int _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider3<int>& UIKitSlider3<int>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider3<int>& UIKitSlider3<int>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template <>
int* UIKitSlider3<int>::getValue(void) {
	return current_val;
}

template <>
void UIKitSlider3<int>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderInt3(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderInt3(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

template <>
UIKitSlider4<int>::UIKitSlider4(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new int;
	this->widget_type = "slider_int4";
}

template <>
UIKitSlider4<int>::UIKitSlider4(std::string title, widget_callback callback, int* ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = ref_val;
	this->widget_type = "slider_int4";
}

template<>
int UIKitSlider4<int>::getMin()
{
	return _min_val;
}

template<>
int UIKitSlider4<int>::getMax()
{
	return _max_val;
}

template <>
UIKitSlider4<int>& UIKitSlider4<int>::setMax(int _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider4<int>& UIKitSlider4<int>::setMin(int _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider4<int>& UIKitSlider4<int>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider4<int>& UIKitSlider4<int>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template <>
int* UIKitSlider4<int>::getValue(void) {
	return current_val;
}

template <>
void UIKitSlider4<int>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderInt4(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderInt4(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}


template <>
UIKitSlider<float>::UIKitSlider(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new float;
	this->widget_type = "slider_float";
}

template <>
UIKitSlider<float>::UIKitSlider(std::string title, widget_callback callback, float& ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = &ref_val;
	this->widget_type = "slider_float";
}

template<>
float UIKitSlider<float>::getMin()
{
	return _min_val;
}

template<>
float UIKitSlider<float>::getMax()
{
	return _max_val;
}

template <>
UIKitSlider<float>& UIKitSlider<float>::setMax(float _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider<float>& UIKitSlider<float>::setMin(float _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider<float>& UIKitSlider<float>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider<float>& UIKitSlider<float>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template <>
float& UIKitSlider<float>::getValue(void) {
	return *current_val;
}

template <>
void UIKitSlider<float>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderFloat(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderFloat(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}


template <>
UIKitSlider2<float>::UIKitSlider2(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new float;
	this->widget_type = "slider_float2";
}

template <>
UIKitSlider2<float>::UIKitSlider2(std::string title, widget_callback callback, float* ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = ref_val;
	this->widget_type = "slider_float2";
}

template<>
float UIKitSlider2<float>::getMin()
{
	return _min_val;
}

template<>
float UIKitSlider2<float>::getMax()
{
	return _max_val;
}

template <>
UIKitSlider2<float>& UIKitSlider2<float>::setMax(float _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider2<float>& UIKitSlider2<float>::setMin(float _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider2<float>& UIKitSlider2<float>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider2<float>& UIKitSlider2<float>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template <>
float* UIKitSlider2<float>::getValue(void) {
	return current_val;
}

template <>
void UIKitSlider2<float>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderFloat2(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderFloat2(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

template <>
UIKitSlider3<float>::UIKitSlider3(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new float;
	this->widget_type = "slider_float3";
}

template <>
UIKitSlider3<float>::UIKitSlider3(std::string title, widget_callback callback, float* ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = ref_val;
	this->widget_type = "slider_float3";
}

template<>
float UIKitSlider3<float>::getMin()
{
	return _min_val;
}

template<>
float UIKitSlider3<float>::getMax()
{
	return _max_val;
}

template <>
UIKitSlider3<float>& UIKitSlider3<float>::setMax(float _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider3<float>& UIKitSlider3<float>::setMin(float _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider3<float>& UIKitSlider3<float>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider3<float>& UIKitSlider3<float>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template <>
float* UIKitSlider3<float>::getValue(void) {
	return current_val;
}

template <>
void UIKitSlider3<float>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderFloat3(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderFloat3(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

template <>
UIKitSlider4<float>::UIKitSlider4(std::string title, widget_callback callback) :
	Widget(title, callback) {
	this->title = title;
	current_val = new float;
	this->widget_type = "slider_float4";
}

template <>
UIKitSlider4<float>::UIKitSlider4(std::string title, widget_callback callback, float* ref_val) :
	Widget(title, callback) {
	this->title = title;
	current_val = ref_val;
	this->widget_type = "slider_float4";
}

template<>
float UIKitSlider4<float>::getMin()
{
	return _min_val;
}

template<>
float UIKitSlider4<float>::getMax()
{
	return _max_val;
}

template <>
UIKitSlider4<float>& UIKitSlider4<float>::setMax(float _m) {
	_max_val = _m;
	return *this;
}

template <>
UIKitSlider4<float>& UIKitSlider4<float>::setMin(float _m) {
	_min_val = _m;
	return *this;
}

template <>
UIKitSlider4<float>& UIKitSlider4<float>::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

template <>
UIKitSlider4<float>& UIKitSlider4<float>::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

template <>
float* UIKitSlider4<float>::getValue(void) {
	return current_val;
}

template <>
void UIKitSlider4<float>::ondraw(void) {
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		if (ImGui::SliderFloat4(title.c_str(), current_val, _min_val, _max_val)) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::SliderFloat4(title.c_str(), current_val, _min_val, _max_val);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

UIKitForm::UIKitForm(const std::string& title, int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->title = title;
}

UIKitForm::~UIKitForm(void) {
}

UIKitForm& UIKitForm::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	return *this;
}

UIKitForm& UIKitForm::setSize(int w, int h) {
	this->w = w;
	this->h = h;
	return *this;
}

UIKitForm& UIKitForm::addFlag(ImGuiWindowFlags flag) {
	form_flags_map[flag] = true;
	return *this;
}

UIKitForm& UIKitForm::setFixedPosition(bool f_pos) {
	fixed_position = f_pos;
	return *this;
}

UIKitForm& UIKitForm::setFixedSize(bool f_size)
{
	fixed_size = f_size;
	return *this;
}

UIKitForm& UIKitForm::addWidget(void* w) {
	inner.push_back(w);
	static_cast<Widget*>(inner.back())->form_meta = this;
	return *this;
}

UIKitForm& UIKitForm::draw(void) {
	if (x != 0 && y != 0) { ImGui::SetNextWindowPos(ImVec2(x, y)); }
	else
	{
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (w != 0 && h != 0) { ImGui::SetNextWindowSize(ImVec2(w, h)); }
	else
	{
		auto size = ImGui::GetWindowSize();
		w = size.x;
		h = size.y;
	}
	form_flags = 0;
	for (auto f : form_flags_map) {
		if (f.second) {
			form_flags |= f.first;
		}
	}
	if (ImGui::Begin(title.c_str(), &opened, form_flags | (edit_mode ? ImGuiWindowFlags_NoBringToFrontOnFocus : 0))) {
		for (auto& it : inner) {
			((Widget*)it)->ondraw();
		}
		last_active = this;
		last_active_form = true;
		ImGui::End();
	}
	return *this;
}

bool UIKitForm::checkFlag(ImGuiWindowFlags flag)
{
	return form_flags_map[flag];
}

UIKitForm& UIKitForm::popFlag(ImGuiWindowFlags flag)
{
	form_flags_map[flag] = false;
	return *this;
}

std::optional<void*>UIKitForm::operator[](std::string name)
{
	auto it = std::ranges::find_if(inner.begin(), inner.end(), [name](void* w) {return ((Widget*)w)->title == name; });
	if (it != inner.end()) {
		return *it;
	}
	else {
		return std::nullopt;
	}
}

std::list<void*> UIKitForm::get_widgets(void) {
	return inner;
}

const std::unordered_map<ImGuiWindowFlags, bool> UIKitForm::GetFlagStatus(void) const
{
	return std::unordered_map<ImGuiWindowFlags, bool>(form_flags_map);
}

UIKitInputText::UIKitInputText(std::string title, widget_callback callback) :
	Widget(title, callback)
{
	this->title = title;
	current_val = new std::string;
	this->widget_type = "input_text";
}

UIKitInputText::UIKitInputText(std::string title, widget_callback callback, std::string* ref_val) :
	Widget(title, callback)
{
	this->title = title;
	current_val = ref_val;
	this->widget_type = "input_text";
}

std::string& UIKitInputText::getText()
{
	return *current_val;
}

void UIKitInputText::ondraw(void)
{
	if (x != 0 && y != 0) {
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y)));
	}
	else {
		auto pos = ImGui::GetCursorPos();
		x = pos.x;
		y = pos.y;
	}
	if (!edit_mode) {
		ImGui::Text(title.c_str());
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y + 20)));
		if (ImGui::InputText(("##"+title).c_str(), current_val, (is_password ? ImGuiInputTextFlags_::ImGuiInputTextFlags_Password : 0))) {
			callback(this);
			last_active = this;
			last_active_form = false;
		}
	}
	else {
		auto draw_list = ImGui::GetWindowDrawList();
		ImGui::Text(title.c_str());
		ImGui::SetCursorPos(ImVec2(CF(x), CF(y + 20)));
		ImGui::InputText(("##"+title).c_str(), current_val, (is_password ? ImGuiInputTextFlags_::ImGuiInputTextFlags_Password : 0));
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(120, 20, 150, 255));
		inEditMode();
		if (!fixed_position) fixed_position = true;
	}
}

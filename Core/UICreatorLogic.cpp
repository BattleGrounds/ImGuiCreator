#include "UICreatorLogic.hpp"
#include "UICore.hpp"
#include <ranges>
#include <string>
#include <format>
#include <imgui_stdlib.h>
#include <iostream>
template<typename T>
typename T::size_type LevenshteinDistance(const T& source,
	const T& target) {
	if (source.size() > target.size()) {
		return LevenshteinDistance(target, source);
	}

	using TSizeType = typename T::size_type;
	const TSizeType min_size = source.size(), max_size = target.size();
	std::vector<TSizeType> lev_dist(min_size + 1);

	for (TSizeType i = 0; i <= min_size; ++i) {
		lev_dist[i] = i;
	}

	for (TSizeType j = 1; j <= max_size; ++j) {
		TSizeType previous_diagonal = lev_dist[0], previous_diagonal_save;
		++lev_dist[0];

		for (TSizeType i = 1; i <= min_size; ++i) {
			previous_diagonal_save = lev_dist[i];
			if (source[i - 1] == target[j - 1]) {
				lev_dist[i] = previous_diagonal;
			}
			else {
				lev_dist[i] = min(min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
			}
			previous_diagonal = previous_diagonal_save;
		}
	}

	return lev_dist[min_size];
}

struct UICreatorData
{
	std::vector<UIKitForm>forms;
	std::string current_prop;
}creator_data;

UICreatorFormProperties::UICreatorFormProperties() {
	props_func_table = {
		std::make_pair("button", [&](void* ptr) { ButtonProps(ptr); }),
		std::make_pair("image_button", [&](void* ptr) {ImageButtonProps(ptr); }),
		std::make_pair("slider_int", [&](void* ptr) {SliderIntProps(ptr); }),
		std::make_pair("slider_int2", [&](void* ptr) {SliderInt2Props(ptr); }),
		std::make_pair("slider_int3", [&](void* ptr) {SliderInt3Props(ptr); }),
		std::make_pair("slider_int4", [&](void* ptr) {SliderInt4Props(ptr); }),
		std::make_pair("slider_float", [&](void* ptr) {SliderFloatProps(ptr); }),
		std::make_pair("slider_float2", [&](void* ptr) {SliderFloat2Props(ptr); }),
		std::make_pair("slider_float3", [&](void* ptr) {SliderFloat3Props(ptr); }),
		std::make_pair("slider_float4", [&](void* ptr) {SliderFloat4Props(ptr); }),
		std::make_pair("input_text", [&](void* ptr) { InputTextProps(ptr); }),
	};
}

void UICreatorFormProperties::FormProps(void* ptr) {
	auto form = static_cast<UIKitForm*>(ptr);
	std::string title = form->title.data();
	if (ImGui::InputText("title", &title)) {
		form->title = title;
	}
	int pos[2] = { form->x, form->y };
	int size[2] = { form->w, form->h };
	if (ImGui::DragInt2("pos", pos))
	{
		form->x = pos[0];
		form->y = pos[1];
	}
	if (ImGui::DragInt2("size", size))
	{
		form->w = size[0];
		form->h = size[1];
	}
	const std::unordered_map<std::string, ImGuiWindowFlags_>window_flags = {
			std::make_pair("No decoration", ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration),
			std::make_pair("No collapse", ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse),
			std::make_pair("No background", ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground),
			std::make_pair("No bring to front on focus", ImGuiWindowFlags_::ImGuiWindowFlags_NoBringToFrontOnFocus),
			std::make_pair("No title bar", ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar),
			std::make_pair("Has menu bar", ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar),
			std::make_pair("No focus on appearing", ImGuiWindowFlags_::ImGuiWindowFlags_NoFocusOnAppearing),
			std::make_pair("No move", ImGuiWindowFlags_::ImGuiWindowFlags_NoMove),
			std::make_pair("No resize", ImGuiWindowFlags_::ImGuiWindowFlags_NoResize),
			std::make_pair("Has inputs", ImGuiWindowFlags_::ImGuiWindowFlags_NoInputs),
			std::make_pair("No saved settings", ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings),
	};
	if (ImGui::BeginListBox("Flags"))
	{
		for (const auto& [fst, snd] : window_flags)
		{
			bool selected = form->checkFlag(snd);
			if (ImGui::Checkbox(fst.data(), &selected))
			{
				if (!selected)
					form->popFlag(snd);
				else
					form->addFlag(snd);
			}
		}
		ImGui::EndListBox();
	}
	std::list<void*>widgets = form->get_widgets();
	if (ImGui::BeginListBox("Widgets"))
	{
		for (auto& it : widgets) {
			auto w = static_cast<Widget*>(it);
			if (ImGui::Selectable(w->title.data()))
			{
				last_active = w;
				last_active_form = false;
			}
			if (ImGui::IsItemHovered())
			{
				w->hoverRect(ImVec4(1.0f, 0.0f, 0.80f, 1.0f));
			}
		}
		ImGui::EndListBox();
	}
}

void UICreatorFormProperties::ButtonProps(void* ptr) {
	UIKitButton* btn = static_cast<UIKitButton*>(ptr);
	std::string title(btn->title.data());
	if (ImGui::InputText("title", &title))
	{
		btn->title = title;
	}
	int pos[2] = { btn->x, btn->y };
	int size[2] = { btn->w, btn->h };
	if (ImGui::DragInt2("pos", pos))
	{
		btn->x = pos[0];
		btn->y = pos[1];
	}
	if (ImGui::DragInt2("size", size))
	{
		btn->w = size[0];
		btn->h = size[1];
	}
}

static bool fb_IsOpenned = false;

void UICreatorFormProperties::ImageButtonProps(void* ptr) {
	{
		UIKitImageButton* btn = static_cast<UIKitImageButton*>(ptr);
		std::string title = btn->title.data();
		if (ImGui::InputText("title", &title))
		{
			btn->title = title;
		}
		int pos[2] = { btn->x, btn->y };
		int size[2] = { btn->w, btn->h };
		if (ImGui::DragInt2("pos", pos))
		{
			btn->x = pos[0];
			btn->y = pos[1];
		}
		if (ImGui::DragInt2("size", size))
		{
			btn->w = size[0];
			btn->h = size[1];
		}
		if (ImGui::Button("Set image"))
		{
			fb.Open();
			fb_IsOpenned = true;
		}
		if (fb.IsOpened() || fb_IsOpenned)
		{
			fb.Display();
			if (fb.HasSelected())
			{
				btn->setImage(fb.GetSelected());
				fb.Close();
				fb_IsOpenned = false;
			}
		}
	}
}

void UICreatorFormProperties::SliderIntProps(void* ptr)
{
	UIKitSlider<int>* slider = static_cast<UIKitSlider<int>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	int _Max = slider->getMax();
	int _Min = slider->getMin();
	if (ImGui::DragInt("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragInt("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::SliderInt2Props(void* ptr)
{
	UIKitSlider2<int>* slider = static_cast<UIKitSlider2<int>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	int _Max = slider->getMax();
	int _Min = slider->getMin();
	if (ImGui::DragInt("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragInt("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::SliderInt3Props(void* ptr)
{
	UIKitSlider3<int>* slider = static_cast<UIKitSlider3<int>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	int _Max = slider->getMax();
	int _Min = slider->getMin();
	if (ImGui::DragInt("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragInt("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::SliderInt4Props(void* ptr)
{
	UIKitSlider4<int>* slider = static_cast<UIKitSlider4<int>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	int _Max = slider->getMax();
	int _Min = slider->getMin();
	if (ImGui::DragInt("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragInt("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::SliderFloatProps(void* ptr)
{
	UIKitSlider<float>* slider = static_cast<UIKitSlider<float>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	float _Max = slider->getMax();
	float _Min = slider->getMin();
	if (ImGui::DragFloat("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragFloat("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::SliderFloat2Props(void* ptr)
{
	UIKitSlider2<float>* slider = static_cast<UIKitSlider2<float>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	float _Max = slider->getMax();
	float _Min = slider->getMin();
	if (ImGui::DragFloat("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragFloat("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::SliderFloat3Props(void* ptr)
{
	UIKitSlider3<float>* slider = static_cast<UIKitSlider3<float>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	float _Max = slider->getMax();
	float _Min = slider->getMin();
	if (ImGui::DragFloat("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragFloat("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::SliderFloat4Props(void* ptr)
{
	UIKitSlider4<float>* slider = static_cast<UIKitSlider4<float>*>(ptr);
	std::string title(slider->title.data());
	if (ImGui::InputText("title", &title))
	{
		slider->title = title;
	}
	int pos[2] = { slider->x, slider->y };
	if (ImGui::DragInt2("pos", pos))
	{
		slider->x = pos[0];
		slider->y = pos[1];
	}
	float _Max = slider->getMax();
	float _Min = slider->getMin();
	if (ImGui::DragFloat("Max", &_Max)) {
		slider->setMax(_Max);
	}
	if (ImGui::DragFloat("Min", &_Min)) {
		slider->setMin(_Min);
	}
}

void UICreatorFormProperties::InputTextProps(void* ptr)
{
	UIKitInputText* input = static_cast<UIKitInputText*>(ptr);
	std::string title(input->title.data());
	if (ImGui::InputText("title", &title))
	{
		input->title = title;
	}
	int pos[2] = { input->x, input->y };
	if (ImGui::DragInt2("pos", pos))
	{
		input->x = pos[0];
		input->y = pos[1];
	}
	int size[2] = { input->w, input->h };
	if (ImGui::DragInt2("size", size)) {
		input->w = size[0];
		input->h = size[1];
	}
	ImGui::Checkbox("Is password", &input->is_password);
}
void UICreatorFormProperties::draw(void) {
	if (ImGui::Begin("UICreator.Properties"))
	{
		if (ImGui::BeginCombo("Widget", "select...")) {
			for (auto& f : creator_data.forms) {
				if (ImGui::Selectable(f.title.c_str())) {
					last_active = &f;
					last_active_form = true;
				}
				std::list<void*>widgets = f.get_widgets();
				for (auto& widget : widgets) {
					auto w = static_cast<Widget*>(widget);
					if (ImGui::Selectable(w->title.c_str())) {
						last_active = w;
						last_active_form = false;
					}
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::BeginTabBar("#UICreator.Props")) {

			if (ImGui::BeginTabItem("Properties")) {

				if (last_active && last_active_form) {
					FormProps(last_active);
				}
				else if (last_active) {
					auto w = static_cast<Widget*>(last_active);
					//UNSAFITY
					props_func_table[w->widget_type](w);
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Callback")) {
				if (last_active && !last_active_form) {
					auto w = static_cast<Widget*>(last_active);
					if (last_active_widget != last_active) {
						editor.SetText(w->callback_code);
						last_active_widget = last_active;
					}
					editor.Render("##cbck");
					w->callback_code = editor.GetText();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

std::vector<void*>UIKitGetForm(void)
{
	std::vector<void*>vec;
	for (auto& it : creator_data.forms) {
		vec.push_back(&it);
	}
	return vec;
}

void UIKitCreateForm(std::string name) {
	creator_data.forms.emplace_back(name);
}

void UIKitGetForm(std::string f_name, void*& ptr) {
	auto it = std::ranges::find_if(creator_data.forms, [f_name](UIKitForm f) {return f_name == f.title; });
	if (it != creator_data.forms.end()) {
		(UIKitForm*&)ptr = &*it;
	}
	else {
		std::cout << "Form " << f_name << " not found!\n";
		return;
	}
}

void UIKitDrawForm(std::string name)
{
	auto it = std::ranges::find_if(creator_data.forms, [name](UIKitForm f) {return name == f.title; });
	if (it != creator_data.forms.end()) {
		(*it).draw();
	}
	else {
		return;
	}
}

void UIKitDrawForm(void)
{
	for (auto& it : creator_data.forms) {
		it.draw();
	}
}

void UICreatorMain::NewForm() {
	if (form_creator_data.open_form_creator) {
		ImVec2 pos;
		pos.x = ImGui::GetIO().DisplaySize.x / 4;
		pos.y = ImGui::GetIO().DisplaySize.y / 4;
		ImGui::SetNextWindowPos(pos);
		if (ImGui::BeginPopupModal("Form Creator", &form_creator_data.open_form_creator)) {
			ImGui::InputText("Title", &form_creator_data.form_name);
			ImGui::DragInt2("pos", form_creator_data.form_pos);
			ImGui::DragInt2("size", form_creator_data.form_size);
			if (ImGui::Button("Apply")) {
				form_creator_data.open_form_creator = false;
				UIKitCreateForm(form_creator_data.form_name);
				UIKitForm* form = nullptr;
				UIKitGetForm(form_creator_data.form_name, (void*&)form);
				form->setPosition(form_creator_data.form_pos[0], form_creator_data.form_pos[1]);
				form->setSize(form_creator_data.form_size[0], form_creator_data.form_size[1]);
			}
			ImGui::EndPopup();
		}
	}
}

UICreatorMain::UICreatorMain()
{
	creator_func_table.insert(std::make_pair("Button", [&]() {return new UIKitButton("Button_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("ImageButton", [&]() {return new UIKitImageButton("ImageButton_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderInt", [&]() {return new UIKitSlider<int>("SliderInt_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderInt2", [&]() {return new UIKitSlider2<int>("SliderInt2_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderInt3", [&]() {return new UIKitSlider3<int>("SliderInt3_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderInt4", [&]() {return new UIKitSlider4<int>("SliderInt4_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderFloat", [&]() {return new UIKitSlider<float>("SliderFloat_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderFloat2", [&]() {return new UIKitSlider2<float>("SliderFloat2_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderFloat3", [&]() {return new UIKitSlider3<float>("SliderFloat3_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("SliderFloat4", [&]() {return new UIKitSlider4<float>("SliderFloat4_" + std::to_string(++last_widget_id), [](void*) {}); }));
	creator_func_table.insert(std::make_pair("InputText", [&]() {return new UIKitInputText("InputText_" + std::to_string(++last_widget_id), [](void*) {}); }));
}

void UICreatorMain::draw() {
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_A, false)) {
		form_creator_data.open_form_creator = !form_creator_data.open_form_creator;
		ImGui::OpenPopup("Form Creator");
	}
	if (ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift && ImGui::IsKeyPressed(ImGuiKey_W, false)) {
		form_creator_data.widget_toolbox = !form_creator_data.widget_toolbox;
	}
	NewForm();
	FormAddWidget();
}

void UICreatorMain::FormAddWidget(void) {
	if (form_creator_data.widget_toolbox) {
		if (ImGui::Begin("Widget Toolbox", &form_creator_data.widget_toolbox)) {
			if (ImGui::BeginCombo("Widget", "select...")) {
				for (auto& f : creator_data.forms) {
					if (ImGui::Selectable(f.title.data())) {
						last_active = &f;
						last_active_form = true;
					}
				}
				ImGui::EndCombo();
			}
			if (last_active_form) {
				UIKitForm* form = static_cast<UIKitForm*>(last_active);
				if (ImGui::BeginListBox("Widgets")) {
					for (const auto& [it, snd] : creator_func_table) {
						if (ImGui::Selectable(it.c_str())) {
							form->addWidget(snd());
						}
					}
					ImGui::EndListBox();
				}
			}
			ImGui::End();
		}
	}
}
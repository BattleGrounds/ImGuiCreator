#include "UICreatorFile.hpp"
#include <pugixml/pugixml.hpp>
#include "UICreatorLogic.hpp"
#include "UICore.hpp"
#include <fstream>
#include <iostream>
#include <imgui_stdlib.h>
#include <imfilebrowser.h>
static pugi::xml_document doc;
static std::string doc_fname;
void UIFileCreate(std::string filename)
{
	std::filesystem::create_directory("export_project");
	std::ofstream out("export_project/" + filename);
	out.close();
	doc.load_file(("export_project/" + filename).c_str());
	doc_fname = filename;
}

void ParseWidget(pugi::xml_node& attr, void* ptr) {
	auto w_base = static_cast<Widget*>(ptr);
	if (w_base->widget_type == "button") {
		auto btn = attr.append_child("Button");
		auto widget = static_cast<UIKitButton*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("w").set_value(widget->w);
		btn.append_attribute("h").set_value(widget->h);
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "image_button") {
		auto btn = attr.append_child("ImageButton");
		auto widget = static_cast<UIKitImageButton*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("w").set_value(widget->w);
		btn.append_attribute("h").set_value(widget->h);
		btn.append_child("img").append_attribute("src").set_value(widget->getIMGSrc().c_str());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_int") {
		auto btn = attr.append_child("SliderInt");
		auto widget = static_cast<UIKitSlider<int>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_int2") {
		auto btn = attr.append_child("SliderInt2");
		auto widget = static_cast<UIKitSlider2<int>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_int3") {
		auto btn = attr.append_child("SliderInt3");
		auto widget = static_cast<UIKitSlider3<int>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_int4") {
		auto btn = attr.append_child("SliderInt4");
		auto widget = static_cast<UIKitSlider4<int>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_float") {
		auto btn = attr.append_child("Sliderfloat");
		auto widget = static_cast<UIKitSlider<float>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_float2") {
		auto btn = attr.append_child("Sliderfloat2");
		auto widget = static_cast<UIKitSlider2<float>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_float3") {
		auto btn = attr.append_child("Sliderfloat3");
		auto widget = static_cast<UIKitSlider3<float>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "slider_float4") {
		auto btn = attr.append_child("Sliderfloat4");
		auto widget = static_cast<UIKitSlider4<float>*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("max").set_value(widget->getMax());
		btn.append_attribute("min").set_value(widget->getMin());
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else if (w_base->widget_type == "input_text") {
		auto btn = attr.append_child("InputText");
		auto widget = static_cast<UIKitInputText*>(ptr);
		btn.append_attribute("name").set_value(widget->title.c_str());
		btn.append_attribute("x").set_value(widget->x);
		btn.append_attribute("y").set_value(widget->y);
		btn.append_attribute("is_password").set_value(widget->is_password);
		btn.append_child("callback").text().set(widget->callback_code.c_str());
	}
	else return;
}

void UIFileWriteUp(void)
{
	if (!UIFileOpened()) return;
	std::filesystem::remove(doc_fname);
	UIFileCreate(doc_fname);
	auto forms = UIKitGetForm();
	for (auto f : forms) {
		auto form = static_cast<UIKitForm*>(f);
		auto doc_form = doc.append_child("form");
		doc_form.append_attribute("name").set_value(form->title.c_str());
		doc_form.append_attribute("x").set_value(form->x);
		doc_form.append_attribute("y").set_value(form->y);
		doc_form.append_attribute("w").set_value(form->w);
		doc_form.append_attribute("h").set_value(form->h);
		for (const auto& [flag, state] : form->GetFlagStatus()) {
			auto param = doc_form.append_child("param");
			param.append_attribute("id").set_value(flag);
			param.append_attribute("eq").set_value(state);
		}
		for (auto w : form->get_widgets()) {
			ParseWidget(doc_form, w);
		}
	}
	doc.save_file(doc_fname.c_str());
}

void UIFileLoad(std::filesystem::path path)
{
	doc.load_file(path.string().c_str());
	if (doc) {
		doc_fname = path.string().c_str();
		std::cout << "Loading document: " << path << std::endl;
		LoadUIState();
		return;
	}
	else {
		std::cout << std::format("Warning: document load failed [ {} ]\n", path.string());
	}
}

bool UIFileOpened(void)
{
	return doc;
}

static bool ui_file_manager = false;
static bool ui_file_creatorform = true;
static bool ui_file_loaderform = false;
static bool ui_file_saveform = false;

void UIFileCreatorForm(void) {
	if (ui_file_creatorform) {
		if (ImGui::BeginChild("RightPane")) {
			ImGui::Text("Filename");
			ImGui::InputText("##name_field", &doc_fname);
			ImGui::SameLine();
			if (ImGui::Button("Apply")) {
				UIFileCreate(doc_fname);
			}
			ImGui::EndChild();
		}
	}
}

ImGui::FileBrowser fb;

void UIFileLoaderForm(void) {
	if (ui_file_loaderform) {
		if (ImGui::BeginChild("RightPane")) {
			if (ImGui::Button("Load")) {
				fb.Open();
			}
			fb.Display();
			if (fb.HasSelected()) {
				UIFileLoad(fb.GetSelected());
				fb.Close();
			}
			ImGui::EndChild();
		}
	}
}

void UIFileManager(void)
{
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
		UIFileWriteUp();
	}
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O, false)) {
		ui_file_manager = true;
		ui_file_loaderform = true;
		ui_file_creatorform = false;
		ui_file_saveform = false;
	}
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N, false)) {
		ui_file_manager = true;
		ui_file_loaderform = false;
		ui_file_creatorform = true;
		ui_file_saveform = false;
	}
	if (ui_file_manager) {
		auto pos = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(pos.x / 4, pos.y / 4));
		if (ImGui::BeginPopupModal("UI File Manager", &ui_file_manager)) {
			if (ImGui::BeginChild("LeftPane", ImVec2(200, 300))) {
				auto c_pos = ImGui::GetCursorPos();
				ImGui::SetCursorPosX(c_pos.x + 50);
				ImGui::SetCursorPosY(c_pos.y + ImGui::GetWindowHeight() / 4);
				if (ImGui::Button("Create", ImVec2(100, 0))) {
					ui_file_creatorform = true;
					ui_file_loaderform = false;
					ui_file_saveform = false;
				}
				ImGui::SetCursorPosX(c_pos.x + 50);
				ImGui::SetCursorPosY(c_pos.y + ImGui::GetWindowHeight() / 4 + ImGui::GetItemRectSize().y+10);
				if (ImGui::Button("Load", ImVec2(100, 0))) {
					ui_file_creatorform = false;
					ui_file_loaderform = true;
					ui_file_saveform = false;
				}
				ImGui::EndChild();
			}
			ImGui::SameLine();
			UIFileCreatorForm();
			UIFileLoaderForm();
			ImGui::EndPopup();
		}
	}
}

void OpenUIFileManager(void)
{
	ui_file_manager = true;
	ImGui::OpenPopup("UI File Manager");
}

void UIFileCompile()
{

}

void LoadUIState(void)
{
	if (!doc) {
		std::cout << "Failed load ui state!\n";
		return;
	}
	else {
		std::cout << "Load UI State!\n";
	}
	for (auto form : doc) {
		std::cout << "Node Name = " << form.name() << std::endl;
		for (auto attr : form.attributes()) {
			std::cout << "\tAttribute '" << attr.name() << "': " << attr.as_string() << std::endl;
		}
		UIKitCreateForm(form.attribute("name").as_string("Example"));
		UIKitForm* f = nullptr;
		UIKitGetForm(form.attribute("name").as_string(), (void*&)f);
		f->x = form.attribute("x").as_int();
		f->y = form.attribute("y").as_int();
		f->w = form.attribute("w").as_int();
		f->h = form.attribute("h").as_int();
		for (auto inner : form) {
			std::cout << "\tNode Name = " << inner.name() << std::endl;
			for (auto attr : inner.attributes()) {
				std::cout << "\t\tAttribute '" << attr.name() << "': " << attr.as_string() << std::endl;
			}
			if ((std::string)inner.name() == "param" && inner.attribute("eq").as_bool()) {
				f->addFlag(inner.attribute("id").as_int());
			}
			else if ((std::string)inner.name() == "Button") {
				UIKitButton* button = new UIKitButton(inner.attribute("name").as_string(), [](void*) {});
				button->x = inner.attribute("x").as_int();
				button->y = inner.attribute("y").as_int();
				button->w = inner.attribute("w").as_int();
				button->h = inner.attribute("h").as_int();
				button->callback_code = inner.child("callback").text().as_string();
				f->addWidget(button);
			}
			else if ((std::string)inner.name() == "ImageButton") {
				std::cout << "Add an ImageButton element!\n";
				UIKitImageButton* button = new UIKitImageButton(inner.attribute("name").as_string(), [](void*) {});
				button->x = inner.attribute("x").as_int();
				button->y = inner.attribute("y").as_int();
				button->w = inner.attribute("w").as_int();
				button->h = inner.attribute("h").as_int();
				button->setImage(inner.child("img").attribute("src").as_string());
				button->callback_code = inner.child("callback").text().as_string();
				f->addWidget(button);
			}
			else if ((std::string)inner.name() == "InputText") {
				std::cout << "Add an InputText element!\n";
				UIKitInputText* button = new UIKitInputText(inner.attribute("name").as_string(), [](void*) {});
				button->x = inner.attribute("x").as_int();
				button->y = inner.attribute("y").as_int();
				button->is_password = inner.attribute("is_password").as_bool();
				button->callback_code = inner.child("callback").text().as_string();
				f->addWidget(button);
			}
			else if ((std::string)inner.name() == "SliderInt") {
				UIKitSlider<int>* slider = new UIKitSlider<int>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_int();
				slider->y = inner.attribute("y").as_int();
				slider->setMax(inner.attribute("max").as_int());
				slider->setMin(inner.attribute("min").as_int());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else if ((std::string)inner.name() == "SliderInt2") {
				UIKitSlider2<int>* slider = new UIKitSlider2<int>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_int();
				slider->y = inner.attribute("y").as_int();
				slider->setMax(inner.attribute("max").as_int());
				slider->setMin(inner.attribute("min").as_int());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else if ((std::string)inner.name() == "SliderInt3") {
				UIKitSlider3<int>* slider = new UIKitSlider3<int>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_int();
				slider->y = inner.attribute("y").as_int();
				slider->setMax(inner.attribute("max").as_int());
				slider->setMin(inner.attribute("min").as_int());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else if ((std::string)inner.name() == "SliderInt4") {
				UIKitSlider4<int>* slider = new UIKitSlider4<int>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_int();
				slider->y = inner.attribute("y").as_int();
				slider->setMax(inner.attribute("max").as_int());
				slider->setMin(inner.attribute("min").as_int());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else if ((std::string)inner.name() == "SliderFloat") {
				UIKitSlider<float>* slider = new UIKitSlider<float>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_float();
				slider->y = inner.attribute("y").as_float();
				slider->setMax(inner.attribute("max").as_float());
				slider->setMin(inner.attribute("min").as_float());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else if ((std::string)inner.name() == "SliderFloat2") {
				UIKitSlider2<float>* slider = new UIKitSlider2<float>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_float();
				slider->y = inner.attribute("y").as_float();
				slider->setMax(inner.attribute("max").as_float());
				slider->setMin(inner.attribute("min").as_float());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else if ((std::string)inner.name() == "SliderFloat3") {
				UIKitSlider3<float>* slider = new UIKitSlider3<float>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_float();
				slider->y = inner.attribute("y").as_float();
				slider->setMax(inner.attribute("max").as_float());
				slider->setMin(inner.attribute("min").as_float());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else if ((std::string)inner.name() == "SliderFloat4") {
				UIKitSlider4<float>* slider = new UIKitSlider4<float>(inner.attribute("name").as_string(), [](void*) {});
				slider->x = inner.attribute("x").as_float();
				slider->y = inner.attribute("y").as_float();
				slider->setMax(inner.attribute("max").as_float());
				slider->setMin(inner.attribute("min").as_float());
				slider->callback_code = inner.child("callback").text().as_string();
				f->addWidget(slider);
			}
			else {
				std::cout << "Unknow node: '" << inner.name() << "'\n";
				continue;
			}
		}

	}
}
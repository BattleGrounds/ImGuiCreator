#pragma once

#include <chaiscript/chaiscript.hpp>
#include <vector>
#include "UICore.hpp"
#include <ranges>

class UIKitConsole {
	chaiscript::ChaiScript c;
	void register_variable(const chaiscript::Boxed_Value& t, std::string name) {
		if (c.get_state().engine_state.m_functions.count(name) == 0) {
			c.add(t, name);
		}
	}
	std::vector<UIKitForm>forms;
	void create_form(std::string_view name, const ImVec2& pos = ImVec2(-1, -1), const ImVec2& size = ImVec2(-1, -1)) {
		forms.emplace_back(name, pos.x, pos.y, size.x, size.y);
	}

public:
	UIKitConsole(void) {
		c.add(chaiscript::fun(register_variable), "register_variable");
	}
	~UIKitConsole(void) = default;


};
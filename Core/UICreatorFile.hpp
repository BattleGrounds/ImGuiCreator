#pragma once


#include <string>
#include <filesystem>

void LoadUIState(void);
void UIFileCreate(std::string filename);
void UIFileWriteUp(void);
void UIFileLoad(std::filesystem::path path);
bool UIFileOpened(void);
void UIFileManager(void);
void OpenUIFileManager(void);
void UIFileCompile();

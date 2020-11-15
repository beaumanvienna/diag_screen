// Copyright (c) 2013- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "Common/UI/UIScreen.h"
#include "Common/File/DirListing.h"

struct ShaderInfo {
	std::string iniFile;  // which ini file was this definition in? So we can write settings back later
	std::string section;  // ini file section. This is saved.
	std::string name;     // Fancy display name.
	std::string parent;   // Parent shader ini section name.

	std::string fragmentShaderFile;
	std::string vertexShaderFile;

	// Show this shader in lists (i.e. not just for chaining.)
	bool visible;
	// Run at output instead of input resolution
	bool outputResolution;
	// Use x1 rendering res + nearest screen scaling filter
	bool isUpscalingFilter;
	// Use 2x display resolution for supersampling with blurry shaders.
	int SSAAFilterLevel;
	// Force constant/max refresh for animated filters
	bool requires60fps;

	struct Setting {
		std::string name;
		float value;
		float maxValue;
		float minValue;
		float step;
	};
	Setting settings[4];

	// TODO: Add support for all kinds of fun options like mapping the depth buffer,
	// SRGB texture reads, etc.  prev shader?

	bool operator == (const std::string &other) {
		return name == other;
	}
	bool operator == (const ShaderInfo &other) {
		return name == other.name;
	}
};
struct TextureShaderInfo {
	std::string iniFile;
	std::string section;
	std::string name;

	std::string computeShaderFile;

	bool operator == (const std::string &other) {
		return name == other;
	}
	bool operator == (const TextureShaderInfo &other) {
		return name == other.name;
	}
};


extern std::string boot_filename;
void UIBackgroundInit(SCREEN_UIContext &dc);
void UIBackgroundShutdown();

inline void NoOpVoidBool(bool) {}

class UIScreenWithBackground : public UIScreen {
public:
	UIScreenWithBackground() : UIScreen() {}
protected:
	void DrawBackground(SCREEN_UIContext &dc) override;
	void sendMessage(const char *message, const char *value) override;
};

class UIScreenWithGameBackground : public UIScreenWithBackground {
public:
	UIScreenWithGameBackground(const std::string &gamePath)
		: UIScreenWithBackground(), gamePath_(gamePath) {}
	void DrawBackground(SCREEN_UIContext &dc) override;
	void sendMessage(const char *message, const char *value) override;
protected:
	std::string gamePath_;
};

class UIDialogScreenWithBackground : public UIDialogScreen {
public:
	UIDialogScreenWithBackground() : UIDialogScreen() {printf("jc: UIDialogScreenWithBackground() : UIDialogScreen()\n");}
protected:
	void DrawBackground(SCREEN_UIContext &dc) override;
	void sendMessage(const char *message, const char *value) override;

	void AddStandardBack(SCREEN_UI::ViewGroup *parent);
};

class UIDialogScreenWithGameBackground : public UIDialogScreenWithBackground {
public:
	UIDialogScreenWithGameBackground(const std::string &gamePath)
		: UIDialogScreenWithBackground(), gamePath_(gamePath) {}
	void DrawBackground(SCREEN_UIContext &dc) override;
	void sendMessage(const char *message, const char *value) override;
protected:
	std::string gamePath_;
};

class PromptScreen : public UIDialogScreenWithBackground {
public:
	PromptScreen(std::string message, std::string yesButtonText, std::string noButtonText,
		std::function<void(bool)> callback = &NoOpVoidBool);

	void CreateViews() override;

	void TriggerFinish(DialogResult result) override;

private:
	SCREEN_UI::EventReturn OnYes(SCREEN_UI::EventParams &e);
	SCREEN_UI::EventReturn OnNo(SCREEN_UI::EventParams &e);

	std::string message_;
	std::string yesButtonText_;
	std::string noButtonText_;
	std::function<void(bool)> callback_;
};

class NewLanguageScreen : public ListPopupScreen {
public:
	NewLanguageScreen(const std::string &title);

private:
	void OnCompleted(DialogResult result) override;
	bool ShowButtons() const override { return true; }
	std::map<std::string, std::pair<std::string, int>> langValuesMapping;
	std::map<std::string, std::string> titleCodeMapping;
	std::vector<FileInfo> langs_;
};

class PostProcScreen : public ListPopupScreen {
public:
	PostProcScreen(const std::string &title, int id);

private:
	void OnCompleted(DialogResult result) override;
	bool ShowButtons() const override { return true; }
	std::vector<ShaderInfo> shaders_;
	int id_;
};

class TextureShaderScreen : public ListPopupScreen {
public:
	TextureShaderScreen(const std::string &title);

private:
	void OnCompleted(DialogResult result) override;
	bool ShowButtons() const override { return true; }
	std::vector<TextureShaderInfo> shaders_;
};

class LogoScreen : public UIScreen {
public:
	LogoScreen(bool gotoGameSettings = false)
		: gotoGameSettings_(gotoGameSettings) {}
	bool key(const KeyInput &key) override;
	bool touch(const TouchInput &touch) override;
	void update() override;
	void render() override;
	void sendMessage(const char *message, const char *value) override;
	void CreateViews() override {}

private:
	void Next();
	int frames_ = 0;
	bool switched_ = false;
	bool gotoGameSettings_ = false;
};

class CreditsScreen : public UIDialogScreenWithBackground {
public:
	CreditsScreen() : frames_(0) {}
	void update() override;
	void render() override;

	void CreateViews() override;

private:
	SCREEN_UI::EventReturn OnOK(SCREEN_UI::EventParams &e);

	SCREEN_UI::EventReturn OnSupport(SCREEN_UI::EventParams &e);
	SCREEN_UI::EventReturn OnPPSSPPOrg(SCREEN_UI::EventParams &e);
	SCREEN_UI::EventReturn OnPrivacy(SCREEN_UI::EventParams &e);
	SCREEN_UI::EventReturn OnForums(SCREEN_UI::EventParams &e);
	SCREEN_UI::EventReturn OnDiscord(SCREEN_UI::EventParams &e);
	SCREEN_UI::EventReturn OnShare(SCREEN_UI::EventParams &e);
	SCREEN_UI::EventReturn OnTwitter(SCREEN_UI::EventParams &e);

	int frames_;
};

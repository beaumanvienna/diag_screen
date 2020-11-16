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

#include "ppsspp_config.h"

#include <algorithm>
#include <functional>

#include "Common/Render/DrawBuffer.h"
#include "Common/UI/Context.h"
#include "Common/UI/View.h"
#include "Common/UI/ViewGroup.h"
#include "Common/UI/UI.h"

#include "Common/System/Display.h"
#include "Common/System/NativeApp.h"
#include "Common/System/System.h"
#include "Common/Math/curves.h"
#include "Common/File/VFS/VFS.h"

#include "Common/Data/Color/RGBAUtil.h"
#include "Common/Data/Text/I18n.h"
#include "Common/Data/Random/Rng.h"
#include "Common/TimeUtil.h"
#include "Common/File/FileUtil.h"

#include "UI/MiscScreens.h"
#include "UI/TextureUtil.h"

static const ImageID symbols[4] = {
	ImageID("I_CROSS"),
	ImageID("I_CIRCLE"),
	ImageID("I_SQUARE"),
	ImageID("I_TRIANGLE"),
};

static const uint32_t colors[4] = {
	0xC0FFFFFF,
	0xC0FFFFFF,
	0xC0FFFFFF,
	0xC0FFFFFF,
};

static std::unique_ptr<SCREEN_ManagedTexture> bgTexture;

static bool backgroundInited;
extern GlobalUIState globalUIState;
void UpdateUIState(GlobalUIState newState) {

	if (globalUIState != newState && globalUIState != UISTATE_EXIT) {
		globalUIState = newState;

		const char *state = nullptr;
		switch (globalUIState) {
		case UISTATE_EXIT: state = "exit";  break;
		case UISTATE_INGAME: state = "ingame"; break;
		case UISTATE_MENU: state = "menu"; break;
		case UISTATE_PAUSEMENU: state = "pausemenu"; break;
		}
		if (state) {
			System_SendMessage("uistate", state);
		}
	}
}


void UIBackgroundInit(SCREEN_UIContext &dc) {
	const std::string bgPng = "background.png";
	if (SCREEN_PFile::Exists(bgPng)) {
		const std::string &bgFile = bgPng;
		bgTexture = CreateTextureFromFile(dc.GetSCREEN_DrawContext(), bgFile.c_str(), DETECT, true);
	}
}

void UIBackgroundShutdown() {
	bgTexture.reset(nullptr);
	backgroundInited = false;
}

void DrawBackground(SCREEN_UIContext &dc, float alpha) {
	if (!backgroundInited) {
		UIBackgroundInit(dc);
		backgroundInited = true;
	}

	static float xbase[100] = {0};
	static float ybase[100] = {0};
	float xres = dc.GetBounds().w;
	float yres = dc.GetBounds().h;
	static int last_xres = 0;
	static int last_yres = 0;

	if (xbase[0] == 0.0f || last_xres != xres || last_yres != yres) {
		GMRng rng;
		for (int i = 0; i < 100; i++) {
			xbase[i] = rng.F() * xres;
			ybase[i] = rng.F() * yres;
		}
		last_xres = xres;
		last_yres = yres;
	}
	
	uint32_t bgColor = whiteAlpha(alpha);
    bgTexture = CreateTextureFromFile(dc.GetSCREEN_DrawContext(), "/home/yo/dev/marley/pictures/splash_pcsx2.png", DETECT, true);

	if (bgTexture != nullptr) {
		dc.Flush();
		dc.GetSCREEN_DrawContext()->BindTexture(0, bgTexture->GetTexture());
		dc.Draw()->DrawTexRect(dc.GetBounds(), 0, 0, 1, 1, bgColor);

		dc.Flush();
		dc.RebindTexture();
	}

	double t = time_now_d();
	for (int i = 0; i < 100; i++) {
		float x = xbase[i] + dc.GetBounds().x;
		float y = ybase[i] + dc.GetBounds().y + 40 * cosf(i * 7.2f + t * 1.3f);
		float angle = (float)sin(i + t);
		int n = i & 3;
		ui_draw2d.DrawImageRotated(symbols[n], x, y, 1.0f, angle, colorAlpha(colors[n], alpha * 0.1f));
	}

}

void DrawGameBackground(SCREEN_UIContext &dc, const std::string &gamePath) {
    /*
	std::shared_ptr<GameInfo> ginfo;
	if (gamePath.size())
		ginfo = g_gameInfoCache->GetInfo(dc.GetSCREEN_DrawContext(), gamePath, GAMEINFO_WANTBG);
	dc.Flush();

	bool hasPic = false;
	double loadTime;
	if (ginfo && ginfo->pic1.texture) {
		dc.GetSCREEN_DrawContext()->BindTexture(0, ginfo->pic1.texture->GetTexture());
		loadTime = ginfo->pic1.timeLoaded;
		hasPic = true;
	} else if (ginfo && ginfo->pic0.texture) {
		dc.GetSCREEN_DrawContext()->BindTexture(0, ginfo->pic0.texture->GetTexture());
		loadTime = ginfo->pic0.timeLoaded;
		hasPic = true;
	}
	if (hasPic) {
		uint32_t color = whiteAlpha(ease((time_now_d() - loadTime) * 3)) & 0xFFc0c0c0;
		dc.Draw()->DrawTexRect(dc.GetBounds(), 0,0,1,1, color);
		dc.Flush();
		dc.RebindTexture();
	} else {
		::DrawBackground(dc, 1.0f);
		dc.RebindTexture();
		dc.Flush();
	}
    */
}

void HandleCommonMessages(const char *message, const char *value, SCREEN_ScreenManager *manager, SCREEN_Screen *activeScreen) {
	bool isActiveScreen = manager->topScreen() == activeScreen;

	/*if (!strcmp(message, "clear jit")) {
		if (MIPSComp::jit && PSP_IsInited()) {
			MIPSComp::jit->ClearCache();
		}
		if (PSP_IsInited()) {
			currentMIPS->UpdateCore((CPUCore)g_PConfig.iCpuCore);
		}
	} else if (!strcmp(message, "control mapping") && isActiveScreen && activeScreen->tag() != "control mapping") {
		UpdateUIState(UISTATE_MENU);
		manager->push(new ControlMappingScreen());
	} else if (!strcmp(message, "display layout editor") && isActiveScreen && activeScreen->tag() != "display layout screen") {
		UpdateUIState(UISTATE_MENU);
		manager->push(new DisplayLayoutScreen());
	} else if (!strcmp(message, "settings") && isActiveScreen && activeScreen->tag() != "settings") {
		UpdateUIState(UISTATE_MENU);
		manager->push(new GameSettingsScreen(""));
	} else if (!strcmp(message, "language screen") && isActiveScreen) {
		auto dev = GetI18NCategory("Developer");
		auto langScreen = new SCREEN_NewLanguageScreen(dev->T("Language"));
		langScreen->OnChoice.Add([](SCREEN_UI::EventParams &) {
			NativeMessageReceived("recreateviews", "");
			if (host) {
				host->UpdateUI();
			}
			return SCREEN_UI::EVENT_DONE;
		});
		manager->push(langScreen);
	} else if (!strcmp(message, "window minimized")) {
		if (!strcmp(value, "true")) {
			gstate_c.skipDrawReason |= SKIPDRAW_WINDOW_MINIMIZED;
		} else {
			gstate_c.skipDrawReason &= ~SKIPDRAW_WINDOW_MINIMIZED;
		}
	}*/
}

void SCREEN_UIScreenWithBackground::DrawBackground(SCREEN_UIContext &dc) {
	::DrawBackground(dc, 1.0f);
	dc.Flush();
}

void SCREEN_UIScreenWithGameBackground::DrawBackground(SCREEN_UIContext &dc) {
	if (!gamePath_.empty()) {
		DrawGameBackground(dc, gamePath_);
	} else {
		::DrawBackground(dc, 1.0f);
		dc.Flush();
	}
}

void SCREEN_UIScreenWithGameBackground::sendMessage(const char *message, const char *value) {
/*
	if (!strcmp(message, "settings") && screenManager()->topScreen() == this) {
		screenManager()->push(new GameSettingsScreen(gamePath_));
	} else {
		SCREEN_UIScreenWithBackground::sendMessage(message, value);
	}
*/
}

void SCREEN_UIDialogScreenWithGameBackground::DrawBackground(SCREEN_UIContext &dc) {
/*
	DrawGameBackground(dc, gamePath_);
*/
}

void SCREEN_UIDialogScreenWithGameBackground::sendMessage(const char *message, const char *value) {
/*
	if (!strcmp(message, "settings") && screenManager()->topScreen() == this) {
		screenManager()->push(new GameSettingsScreen(gamePath_));
	} else {
		SCREEN_UIDialogScreenWithBackground::sendMessage(message, value);
	}
*/
}

void SCREEN_UIScreenWithBackground::sendMessage(const char *message, const char *value) {
	HandleCommonMessages(message, value, screenManager(), this);
}

void SCREEN_UIDialogScreenWithBackground::DrawBackground(SCREEN_UIContext &dc) {
	::DrawBackground(dc, 1.0f);
	dc.Flush();
}

void SCREEN_UIDialogScreenWithBackground::AddStandardBack(SCREEN_UI::ViewGroup *parent) {
	using namespace SCREEN_UI;
	auto di = GetI18NCategory("Dialog");
	parent->Add(new Choice(di->T("Back"), "", false, new AnchorLayoutParams(150, 64, 10, NONE, NONE, 10)))->OnClick.Handle<SCREEN_UIScreen>(this, &SCREEN_UIScreen::OnBack);
}

void SCREEN_UIDialogScreenWithBackground::sendMessage(const char *message, const char *value) {
	HandleCommonMessages(message, value, screenManager(), this);
}

PromptScreen::PromptScreen(std::string message, std::string yesButtonText, std::string noButtonText, std::function<void(bool)> callback)
		: message_(message), callback_(callback) {
	auto di = GetI18NCategory("Dialog");
	yesButtonText_ = di->T(yesButtonText.c_str());
	noButtonText_ = di->T(noButtonText.c_str());
}

void PromptScreen::CreateViews() {
	// Information in the top left.
	// Back button to the bottom left.
	// Scrolling action menu to the right.
	using namespace SCREEN_UI;

	Margins actionMenuMargins(0, 100, 15, 0);

	root_ = new LinearLayout(ORIENT_HORIZONTAL);

	ViewGroup *leftColumn = new AnchorLayout(new LinearLayoutParams(1.0f));
	root_->Add(leftColumn);

	float leftColumnWidth = dp_xres - actionMenuMargins.left - actionMenuMargins.right - 300.0f;
	leftColumn->Add(new TextView(message_, ALIGN_LEFT | FLAG_WRAP_TEXT, false, new AnchorLayoutParams(leftColumnWidth, WRAP_CONTENT, 10, 10, NONE, NONE)))->SetClip(false);

	ViewGroup *rightColumnItems = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(300, FILL_PARENT, actionMenuMargins));
	root_->Add(rightColumnItems);
	Choice *yesButton = rightColumnItems->Add(new Choice(yesButtonText_));
	yesButton->OnClick.Handle(this, &PromptScreen::OnYes);
	root_->SetDefaultFocusView(yesButton);
	if (!noButtonText_.empty())
		rightColumnItems->Add(new Choice(noButtonText_))->OnClick.Handle(this, &PromptScreen::OnNo);
}

SCREEN_UI::EventReturn PromptScreen::OnYes(SCREEN_UI::EventParams &e) {
	TriggerFinish(DR_OK);
	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn PromptScreen::OnNo(SCREEN_UI::EventParams &e) {
	TriggerFinish(DR_CANCEL);
	return SCREEN_UI::EVENT_DONE;
}

void PromptScreen::TriggerFinish(DialogResult result) {
	callback_(result == DR_OK || result == DR_YES);
	SCREEN_UIDialogScreenWithBackground::TriggerFinish(result);
}

SCREEN_PostProcScreen::SCREEN_PostProcScreen(const std::string &title, int id) : ListSCREEN_PopupScreen(title), id_(id) {
/*	auto ps = GetI18NCategory("PostShaders");
	ReloadAllPostShaderInfo();
	shaders_ = GetAllPostShaderInfo();
	std::vector<std::string> items;
	int selected = -1;
	for (int i = 0; i < (int)shaders_.size(); i++) {
		if (!shaders_[i].visible)
			continue;
		if (shaders_[i].section == g_PConfig.vPostShaderNames[id_])
			selected = i;
		items.push_back(ps->T(shaders_[i].section.c_str(), shaders_[i].name.c_str()));
	}
	adaptor_ = SCREEN_UI::StringVectorListAdaptor(items, selected);
*/
}

void SCREEN_PostProcScreen::OnCompleted(DialogResult result) {
/*
	if (result != DR_OK)
		return;
	g_PConfig.vPostShaderNames[id_] = shaders_[listView_->GetSelected()].section;
*/
}

SCREEN_TextureShaderScreen::SCREEN_TextureShaderScreen(const std::string &title) : ListSCREEN_PopupScreen(title) {
/*
	auto ps = GetI18NCategory("TextureShaders");
	ReloadAllPostShaderInfo();
	shaders_ = GetAllTextureShaderInfo();
	std::vector<std::string> items;
	int selected = -1;
	for (int i = 0; i < (int)shaders_.size(); i++) {
		if (shaders_[i].section == g_PConfig.sTextureShaderName)
			selected = i;
		items.push_back(ps->T(shaders_[i].section.c_str(), shaders_[i].name.c_str()));
	}
	adaptor_ = SCREEN_UI::StringVectorListAdaptor(items, selected);
*/
}

void SCREEN_TextureShaderScreen::OnCompleted(DialogResult result) {
/*
	if (result != DR_OK)
		return;
	g_PConfig.sTextureShaderName = shaders_[listView_->GetSelected()].section;
*/
}

SCREEN_NewLanguageScreen::SCREEN_NewLanguageScreen(const std::string &title) : ListSCREEN_PopupScreen(title) {
/*

	langValuesMapping = GetLangValuesMapping();

	std::vector<FileInfo> tempLangs;
	VFSGetFileListing("lang", &tempLangs, "ini");
	std::vector<std::string> listing;
	int selected = -1;
	int counter = 0;
	for (size_t i = 0; i < tempLangs.size(); i++) {
		// Skip README
		if (tempLangs[i].name.find("README") != std::string::npos) {
			continue;
		}

		if (tempLangs[i].name.find("ar_AE") != std::string::npos) {
			continue;
		}

		if (tempLangs[i].name.find("fa_IR") != std::string::npos) {
			continue;
		}

		FileInfo lang = tempLangs[i];
		langs_.push_back(lang);

		std::string code;
		size_t dot = lang.name.find('.');
		if (dot != std::string::npos)
			code = lang.name.substr(0, dot);

		std::string buttonTitle = lang.name;

		if (!code.empty()) {
			if (langValuesMapping.find(code) == langValuesMapping.end()) {
				// No title found, show locale code
				buttonTitle = code;
			} else {
				buttonTitle = langValuesMapping[code].first;
			}
		}
		if (g_PConfig.sLanguageIni == code)
			selected = counter;
		listing.push_back(buttonTitle);
		counter++;
	}

	adaptor_ = SCREEN_UI::StringVectorListAdaptor(listing, selected);
*/
}

void SCREEN_NewLanguageScreen::OnCompleted(DialogResult result) {
/*
	if (result != DR_OK)
		return;
	std::string oldLang = g_PConfig.sLanguageIni;
	std::string iniFile = langs_[listView_->GetSelected()].name;

	size_t dot = iniFile.find('.');
	std::string code;
	if (dot != std::string::npos)
		code = iniFile.substr(0, dot);

	if (code.empty())
		return;

	g_PConfig.sLanguageIni = code;

	bool iniLoadedSuccessfully = false;
	// Allow the lang directory to be overridden for testing purposes (e.g. Android, where it's hard to 
	// test new languages without recompiling the entire app, which is a hassle).
	const std::string langOverridePath = GetSysDirectory(DIRECTORY_SYSTEM) + "lang/";

	// If we run into the unlikely case that "lang" is actually a file, just use the built-in translations.
	if (!SCREEN_PFile::Exists(langOverridePath) || !SCREEN_PFile::IsDirectory(langOverridePath))
		iniLoadedSuccessfully = i18nrepo.LoadIni(g_PConfig.sLanguageIni);
	else
		iniLoadedSuccessfully = i18nrepo.LoadIni(g_PConfig.sLanguageIni, langOverridePath);

	if (iniLoadedSuccessfully) {
		// Dunno what else to do here.
		if (langValuesMapping.find(code) == langValuesMapping.end()) {
			// Fallback to English
			g_PConfig.iLanguage = PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
		} else {
			g_PConfig.iLanguage = langValuesMapping[code].second;
		}
		RecreateViews();
	} else {
		g_PConfig.sLanguageIni = oldLang;
	}
*/
}

void LogoScreen::Next() {
/*	if (!switched_) {
		switched_ = true;
		if (gotoGameSettings_) {
			if (boot_filename.size()) {
				screenManager()->switchScreen(new EmuScreen(boot_filename));
			} else {
				screenManager()->switchScreen(new MainScreen());
			}
			screenManager()->push(new GameSettingsScreen(boot_filename));
		} else if (boot_filename.size()) {
			screenManager()->switchScreen(new EmuScreen(boot_filename));
		} else {
			screenManager()->switchScreen(new MainScreen());
		}
	}
*/
}

const float logoScreenSeconds = 2.5f;

void LogoScreen::update() {
	SCREEN_UIScreen::update();
	frames_++;
	if (frames_ > 60 * logoScreenSeconds) {
		Next();
	}
}

void LogoScreen::sendMessage(const char *message, const char *value) {
/*	if (!strcmp(message, "boot") && screenManager()->topScreen() == this) {
		screenManager()->switchScreen(new EmuScreen(value));
	}
*/
}

bool LogoScreen::key(const KeyInput &key) {
	if (key.deviceId != DEVICE_ID_MOUSE) {
		Next();
		return true;
	}
	return false;
}

bool LogoScreen::touch(const TouchInput &touch) {
	if (touch.flags & TOUCH_DOWN) {
		Next();
		return true;
	}
	return false;
}

void LogoScreen::render() {
	using namespace SCREEN_Draw;

	SCREEN_UIScreen::render();
	SCREEN_UIContext &dc = *screenManager()->getUIContext();

	const Bounds &bounds = dc.GetBounds();

	float xres = dc.GetBounds().w;
	float yres = dc.GetBounds().h;

	dc.Begin();
	float t = (float)frames_ / (60.0f * logoScreenSeconds / 3.0f);

	float alpha = t;
	if (t > 1.0f)
		alpha = 1.0f;
	float alphaText = alpha;
	if (t > 2.0f)
		alphaText = 3.0f - t;
	uint32_t textColor = colorAlpha(dc.theme->infoStyle.fgColor, alphaText);

	::DrawBackground(dc, alpha);

	auto cr = GetI18NCategory("PSPCredits");
	auto gr = GetI18NCategory("Graphics");
	char temp[256];
	// Manually formatting UTF-8 is fun.  \xXX doesn't work everywhere.
	snprintf(temp, sizeof(temp), "%s Henrik Rydg%c%crd", cr->T("created", "Created by"), 0xC3, 0xA5);
	if (System_GetPropertyBool(SYSPROP_APP_GOLD)) {
		dc.Draw()->DrawImage(ImageID("I_ICONGOLD"), bounds.centerX() - 120, bounds.centerY() - 30, 1.2f, textColor, ALIGN_CENTER);
	} else {
		dc.Draw()->DrawImage(ImageID("I_ICON"), bounds.centerX() - 120, bounds.centerY() - 30, 1.2f, textColor, ALIGN_CENTER);
	}
	dc.Draw()->DrawImage(ImageID("I_LOGO"), bounds.centerX() + 40, bounds.centerY() - 30, 1.5f, textColor, ALIGN_CENTER);
	//dc.Draw()->DrawTextShadow(UBUNTU48, "PPSSPP", xres / 2, yres / 2 - 30, textColor, ALIGN_CENTER);
	dc.SetFontScale(1.0f, 1.0f);
	dc.SetFontStyle(dc.theme->uiFont);
	dc.DrawText(temp, bounds.centerX(), bounds.centerY() + 40, textColor, ALIGN_CENTER);
	dc.DrawText(cr->T("license", "Free Software under GPL 2.0+"), bounds.centerX(), bounds.centerY() + 70, textColor, ALIGN_CENTER);

	int ppsspp_org_y = yres / 2 + 130;
	dc.DrawText("www.ppsspp.org", bounds.centerX(), ppsspp_org_y, textColor, ALIGN_CENTER);

#if (defined(_WIN32) && !PPSSPP_PLATFORM(UWP)) || PPSSPP_PLATFORM(ANDROID) || PPSSPP_PLATFORM(LINUX)
	// Draw the graphics API, except on UWP where it's always D3D11
	std::string apiName = screenManager()->getSCREEN_DrawContext()->GetInfoString(InfoField::APINAME);
#ifdef _DEBUG
	apiName += ", debug build";
#endif
	dc.DrawText(gr->T(apiName), bounds.centerX(), ppsspp_org_y + 50, textColor, ALIGN_CENTER);
#endif

	dc.Flush();
}

void CreditsScreen::CreateViews() {
	using namespace SCREEN_UI;
	auto di = GetI18NCategory("Dialog");
	auto cr = GetI18NCategory("PSPCredits");

	root_ = new AnchorLayout(new LayoutParams(FILL_PARENT, FILL_PARENT));
	Button *back = root_->Add(new Button(di->T("Back"), new AnchorLayoutParams(260, 64, NONE, NONE, 10, 10, false)));
	back->OnClick.Handle(this, &CreditsScreen::OnOK);
	root_->SetDefaultFocusView(back);

}

SCREEN_UI::EventReturn CreditsScreen::OnSupport(SCREEN_UI::EventParams &e) {

	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn CreditsScreen::OnTwitter(SCREEN_UI::EventParams &e) {

	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn CreditsScreen::OnPPSSPPOrg(SCREEN_UI::EventParams &e) {

	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn CreditsScreen::OnPrivacy(SCREEN_UI::EventParams &e) {

	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn CreditsScreen::OnForums(SCREEN_UI::EventParams &e) {

	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn CreditsScreen::OnDiscord(SCREEN_UI::EventParams &e) {
	
	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn CreditsScreen::OnShare(SCREEN_UI::EventParams &e) {
	auto cr = GetI18NCategory("PSPCredits");
	System_SendMessage("sharetext", cr->T("CheckOutPPSSPP", "Check out PPSSPP, the awesome PSP emulator: https://www.ppsspp.org/"));
	return SCREEN_UI::EVENT_DONE;
}

SCREEN_UI::EventReturn CreditsScreen::OnOK(SCREEN_UI::EventParams &e) {
	TriggerFinish(DR_OK);
	return SCREEN_UI::EVENT_DONE;
}

void CreditsScreen::update() {
	SCREEN_UIScreen::update();
	UpdateUIState(UISTATE_MENU);
	frames_++;
}

void CreditsScreen::render() {
	SCREEN_UIScreen::render();

	auto cr = GetI18NCategory("PSPCredits");

	std::string specialthanksMaxim = "Maxim ";
	specialthanksMaxim += cr->T("specialthanksMaxim", "for his amazing Atrac3+ decoder work");

	std::string specialthanksKeithGalocy = "Keith Galocy ";
	specialthanksKeithGalocy += cr->T("specialthanksKeithGalocy", "at NVIDIA (hardware, advice)");

	std::string specialthanksOrphis = "Orphis (";
	specialthanksOrphis += cr->T("build server");
	specialthanksOrphis += ')';

	std::string specialthanksangelxwind = "angelxwind (";
	specialthanksangelxwind += cr->T("iOS builds");
	specialthanksangelxwind += ')';

	std::string specialthanksW_MS = "W.MS (";
	specialthanksW_MS += cr->T("iOS builds");
	specialthanksW_MS += ')';

	std::string specialthankssolarmystic = "solarmystic (";
	specialthankssolarmystic += cr->T("testing");
	specialthankssolarmystic += ')';

	const char * credits[] = {
		"PPSSPP",
		"",
		cr->T("title", "A fast and portable PSP emulator"),
		"",
		"",
		cr->T("created", "Created by"),
		"Henrik Rydg\xc3\xa5rd",
		"",
		"",
		cr->T("contributors", "Contributors:"),
		"unknownbrackets",
		"oioitff",
		"xsacha",
		"raven02",
		"tpunix",
		"orphis",
		"sum2012",
		"mikusp",
		"aquanull",
		"The Dax",
		"bollu",
		"tmaul",
		"artart78",
		"ced2911",
		"soywiz",
		"kovensky",
		"xele",
		"chaserhjk",
		"evilcorn",
		"daniel dressler",
		"makotech222",
		"CPkmn",
		"mgaver",
		"jeid3",
		"cinaera/BeaR",
		"jtraynham",
		"Kingcom",
		"arnastia",
		"lioncash",
		"JulianoAmaralChaves",
		"vnctdj",
		"kaienfr",
		"shenweip",
		"Danyal Zia",
		"Igor Calabria",
		"Coldbird",
		"Kyhel",
		"xebra",
		"LunaMoo",
		"zminhquanz",
		"ANR2ME",
		"adenovan",
		"iota97",
		"",
		cr->T("specialthanks", "Special thanks to:"),
		specialthanksMaxim.c_str(),
		specialthanksKeithGalocy.c_str(),
		specialthanksOrphis.c_str(),
		specialthanksangelxwind.c_str(),
		specialthanksW_MS.c_str(),
		specialthankssolarmystic.c_str(),
		cr->T("all the forum mods"),
		"",
		cr->T("this translation by", ""),   // Empty string as this is the original :)
		cr->T("translators1", ""),
		cr->T("translators2", ""),
		cr->T("translators3", ""),
		cr->T("translators4", ""),
		cr->T("translators5", ""),
		cr->T("translators6", ""),
		"",
		cr->T("written", "Written in C++ for speed and portability"),
		"",
		"",
		cr->T("tools", "Free tools used:"),
#ifdef __ANDROID__
		"Android SDK + NDK",
#endif
#if defined(USING_QT_UI)
		"Qt",
#elif !defined(USING_WIN_UI)
		"SDL",
#endif
		"CMake",
		"freetype2",
		"zlib",
		"PSP SDK",
		"",
		"",
		cr->T("website", "Check out the website:"),
		"www.ppsspp.org",
		cr->T("list", "compatibility lists, forums, and development info"),
		"",
		"",
		cr->T("check", "Also check out Dolphin, the best Wii/GC emu around:"),
		"https://www.dolphin-emu.org",
		"",
		"",
		cr->T("info1", "PPSSPP is only intended to play games you own."),
		cr->T("info2", "Please make sure that you own the rights to any games"),
		cr->T("info3", "you play by owning the UMD or by buying the digital"),
		cr->T("info4", "download from the PSN store on your real PSP."),
		"",
		"",
		cr->T("info5", "PSP is a trademark by Sony, Inc."),
	};


	// TODO: This is kinda ugly, done on every frame...
	char temp[256];
	if (System_GetPropertyBool(SYSPROP_APP_GOLD)) {
		snprintf(temp, sizeof(temp), "PPSSPP Gold %s", "0.1.9");
	} else {
		snprintf(temp, sizeof(temp), "PPSSPP %s", "0.1.9");
	}
	credits[0] = (const char *)temp;

	SCREEN_UIContext &dc = *screenManager()->getUIContext();
	dc.Begin();
	const Bounds &bounds = dc.GetLayoutBounds();

	const int numItems = ARRAY_SIZE(credits);
	int itemHeight = 36;
	int totalHeight = numItems * itemHeight + bounds.h + 200;
	int y = bounds.y2() - (frames_ % totalHeight);
	for (int i = 0; i < numItems; i++) {
		float alpha = linearInOut(y+32, 64, bounds.y2() - 192, 64);
		uint32_t textColor = colorAlpha(dc.theme->infoStyle.fgColor, alpha);

		if (alpha > 0.0f) {
			dc.SetFontScale(ease(alpha), ease(alpha));
			dc.DrawText(credits[i], bounds.centerX(), y, textColor, ALIGN_HCENTER);
			dc.SetFontScale(1.0f, 1.0f);
		}
		y += itemHeight;
	}

	dc.Flush();
}

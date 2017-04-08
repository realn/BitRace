#include "stdafx.h"
#include "Config.h"
#include "FileSystem.h"

#include <CBXml/Serialize.h>

CConfig::CScreen::CScreen()
  : Width(800), Height(600), ColorBits(32), RefreshRate(0), DevId(0)
  , Fullscreen(false), VSync(false) {}

const glm::uvec2 CConfig::CScreen::GetSize() const {
  return glm::uvec2(Width, Height);
}

const float CConfig::CScreen::GetAspectRatio() const {
  if(Height == 0)
    return 0.0f;
  return float(Width) / float(Height);
}


CConfig::CRender::CRender()
  : SmoothLines(true)
  , SmoothShade(true)
  , Blur(false) 
  , BlurTexSize(64)
  , BlurTexAlpha(0.3f)
{}

CConfig::CDiag::CDiag()
  : FPSCounter(false) {}

CConfig::CConfig() {}

static const cb::string XML_CONFIG_ROOT = L"Config";

const bool CConfig::Read(IFileSystem& fs, const cb::string & filepath) {
  return fs.ReadXml(filepath, XML_CONFIG_ROOT, *this);
}

const bool CConfig::Write(IFileSystem& fs, const cb::string & filepath) {
  return fs.WriteXml(filepath, XML_CONFIG_ROOT, *this);
}

static const cb::string XML_CONFIG_SCREEN_WIDTH = L"Width";
static const cb::string XML_CONFIG_SCREEN_HEIGHT = L"Height";
static const cb::string XML_CONFIG_SCREEN_COLORBITS = L"ColorBits";
static const cb::string XML_CONFIG_SCREEN_REFRESHRATE = L"RefreshRate";
static const cb::string XML_CONFIG_SCREEN_DEVID = L"DevId";
static const cb::string XML_CONFIG_SCREEN_FULLSCREEN = L"Fullscreen";
static const cb::string XML_CONFIG_SCREEN_VSYNC = L"VSync";

CB_DEFINEXMLWRITE(CConfig::CScreen) {
  return
    SetAttribute(XML_CONFIG_SCREEN_WIDTH, mObject.Width) &&
    SetAttribute(XML_CONFIG_SCREEN_HEIGHT, mObject.Height) &&
    SetAttribute(XML_CONFIG_SCREEN_COLORBITS, mObject.ColorBits) &&
    SetAttribute(XML_CONFIG_SCREEN_REFRESHRATE, mObject.RefreshRate) &&
    SetAttribute(XML_CONFIG_SCREEN_DEVID, mObject.DevId) &&
    SetAttribute(XML_CONFIG_SCREEN_FULLSCREEN, mObject.Fullscreen) &&
    SetAttribute(XML_CONFIG_SCREEN_VSYNC, mObject.VSync);
}

CB_DEFINEXMLREAD(CConfig::CScreen) {
  GetAttribute(XML_CONFIG_SCREEN_WIDTH, mObject.Width);
  GetAttribute(XML_CONFIG_SCREEN_HEIGHT, mObject.Height);
  GetAttribute(XML_CONFIG_SCREEN_COLORBITS, mObject.ColorBits);
  GetAttribute(XML_CONFIG_SCREEN_REFRESHRATE, mObject.RefreshRate);
  GetAttribute(XML_CONFIG_SCREEN_DEVID, mObject.DevId);
  GetAttribute(XML_CONFIG_SCREEN_FULLSCREEN, mObject.Fullscreen);
  GetAttribute(XML_CONFIG_SCREEN_VSYNC, mObject.VSync);
  return true;
}


static const cb::string XML_CONFIG_RENDER_SMOOTHLINES = L"SmoothLines";
static const cb::string XML_CONFIG_RENDER_SMOOTHSHADE = L"SmoothShade";
static const cb::string XML_CONFIG_RENDER_BLUR = L"Blur";
static const cb::string XML_CONFIG_RENDER_BLURTEXSIZE = L"BlurTexSize";
static const cb::string XML_CONFIG_RENDER_BLURTEXALPHA = L"BlurTexAlpha";

CB_DEFINEXMLWRITE(CConfig::CRender) {
  return
    SetAttribute(XML_CONFIG_RENDER_SMOOTHLINES, mObject.SmoothLines) &&
    SetAttribute(XML_CONFIG_RENDER_SMOOTHSHADE, mObject.SmoothShade) &&
    SetAttribute(XML_CONFIG_RENDER_BLUR, mObject.Blur) &&
    SetAttribute(XML_CONFIG_RENDER_BLURTEXSIZE, mObject.BlurTexSize) &&
    SetAttribute(XML_CONFIG_RENDER_BLURTEXALPHA, mObject.BlurTexAlpha);
}

CB_DEFINEXMLREAD(CConfig::CRender) {
  GetAttribute(XML_CONFIG_RENDER_SMOOTHLINES, mObject.SmoothLines);
  GetAttribute(XML_CONFIG_RENDER_SMOOTHSHADE, mObject.SmoothShade);
  GetAttribute(XML_CONFIG_RENDER_BLUR, mObject.Blur);
  GetAttribute(XML_CONFIG_RENDER_BLURTEXSIZE, mObject.BlurTexSize);
  GetAttribute(XML_CONFIG_RENDER_BLURTEXALPHA, mObject.BlurTexAlpha);
  return true;
}


static const cb::string XML_CONFIG_DIAG_FPSCOUNTER = L"FPSCounter";

CB_DEFINEXMLWRITE(CConfig::CDiag) {
  return
    SetAttribute(XML_CONFIG_DIAG_FPSCOUNTER, mObject.FPSCounter);
}

CB_DEFINEXMLREAD(CConfig::CDiag) {
  GetAttribute(XML_CONFIG_DIAG_FPSCOUNTER, mObject.FPSCounter);
  return true;
}


static const cb::string XML_CONFIG_SCREEN = L"Screen";
static const cb::string XML_CONFIG_RENDER = L"Render";
static const cb::string XML_CONFIG_DIAG = L"Diag";

CB_DEFINEXMLWRITE(CConfig) {
  return
    SetNode(XML_CONFIG_SCREEN, mObject.Screen) &&
    SetNode(XML_CONFIG_RENDER, mObject.Render) &&
    SetNode(XML_CONFIG_DIAG, mObject.Diag);
}

CB_DEFINEXMLREAD(CConfig) {
  GetNode(XML_CONFIG_SCREEN, mObject.Screen);
  GetNode(XML_CONFIG_RENDER, mObject.Render);
  GetNode(XML_CONFIG_DIAG, mObject.Diag);
  return true;
}

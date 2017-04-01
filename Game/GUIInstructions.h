#pragma once

#include <SDL_types.h>
#include <set>
#include <vector>
#include <glm/glm.hpp>

#include "Instructions.h"

class CGUIControl;
class CGUITextControl;

class CGUITextAnimation : public CInstruction {
private:
  CGUITextControl* m_pControl;
  std::string m_Text;
  Uint32  m_TextTargetPos;
  Uint32  m_TextStartPos;
  Uint32  m_TextPos;
  float   m_AnimTime;
  float   m_CharTime;
  float   m_Time;
  bool    m_TextChanged;

public:
  CGUITextAnimation(CGUITextControl* pControl, 
                    const std::string text,
                    const Uint32 lenStart, 
                    const Uint32 lenTarget,
                    const float animTime);
  virtual ~CGUITextAnimation();

  virtual void  Update(const float timeDelta) override;

protected:
  virtual void OnReset() override;
  virtual void OnEnd() override;
};

class CGUITextCountAnimation : public CWaitInstruction {
private:
  CGUITextControl* m_pControl;
  std::string m_TextPrefix;
  std::string m_TextSufix;
  Uint32 m_ValueStart;
  Uint32 m_ValueTarget;
  Uint32 m_LastValue;

public:
  CGUITextCountAnimation(CGUITextControl* pControl,
                         const Uint32 valStart,
                         const Uint32 valTarget,
                         const float waitTime,
                         const std::string& prefix = "",
                         const std::string& sufix = "");
  virtual ~CGUITextCountAnimation();

  virtual void Update(const float timeDelta) override;

  void SetValueRange(const Uint32 start, const Uint32 target);
  void SetText(const std::string& prefix, const std::string& sufix);

  const Uint32 GetValue() const;
  const std::string GetText() const;

protected:
  virtual void OnReset() override;
  virtual void OnEnd() override;
};

class CGUIFadeAnimation : public CWaitInstruction {
protected:
  CGUIControl* m_pControl;
  glm::vec4 m_ColorStart;
  glm::vec4 m_ColorTarget;
  glm::vec4 m_ColorMask;

public:
  static const glm::vec4 maskAll;
  static const glm::vec4 maskColor;
  static const glm::vec4 maskAlpha;

  CGUIFadeAnimation(CGUIControl* pControl,
                    const glm::vec4 colorStart,
                    const glm::vec4 colorTarget,
                    const float waitTime,
                    const glm::vec4 colorMask = CGUIFadeAnimation::maskAll);
  virtual ~CGUIFadeAnimation();

  virtual void Update(const float timeDelta) override;

  void SetColor(const glm::vec4& start, const glm::vec4& target);
  void SetColorMask(const glm::vec4& mask);

  const glm::vec4 GetColor() const;
};

#pragma once

#include <SDL_types.h>
#include <set>
#include <vector>
#include <glm/glm.hpp>

class CGUIControl;
class CGUITextControl;
class CGUIControllerList;

class CGUIController {
public:
  //  STOPPED -> RUNNING -> DONE
  enum State {
    STOPPED = 0,
    RUNNING = 1,
    DONE = 2,
  };

private:
  bool m_Enabled;
  State m_State;

public:
  CGUIController(CGUIControllerList* pList, const bool enabled = true);
  virtual ~CGUIController() {}

  virtual void Update(const float timeDelta) = 0;

  void Run();
  void Stop();
  void Done();

  void Rerun();
  void ForceRun();

  void SetEnabled(const bool enabled);
  const bool IsEnabled() const;

  const bool IsRunning() const;
  const bool IsStopped() const;
  const bool IsDone() const;

private:
  void ChangeState(const State newState);

protected:
  virtual void StateChange(const State oldState, State& newState) = 0;
};




class CGUIControllerList {
private:
  std::set<CGUIController*> m_List;

public:
  CGUIControllerList();
  ~CGUIControllerList();

  void Update(const float timeDelta);

  CGUIController* Add(CGUIController* pController);
  void Clear();

  void StopAll();
};





class CGUIDoneTrigger :
  public CGUIController {
public:
  enum CondOp {
    CO_AND = 0,
    CO_OR = 1
  };
protected:
  std::vector<CGUIController*> m_Conditions;
  std::vector<CGUIController*> m_Targets;
  CondOp  m_CondOp;

public:
  CGUIDoneTrigger(CGUIControllerList* pList,
                  CGUIController* pCondition = nullptr,
                  CGUIController* pTarget = nullptr,
                  const bool running = true);
  virtual ~CGUIDoneTrigger();

  void AddCondition(CGUIController* pCondition);
  void AddTarget(CGUIController* pTarget);

  void SetCondOp(const CondOp condOp);

  virtual void Update(const float timeDelta) override;

protected:
  virtual void StateChange(const State oldState, State & newState) override;
};





class CGUITextAnimation :
  public CGUIController {
private:
  CGUITextControl* m_pControl;
  std::string m_Text;
  float   m_AnimTime;
  float   m_Time;
  float   m_CharTime;
  Uint32  m_CharLen;
  bool    m_Visible;

public:
  CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const std::string text, const float animTime, const bool show = true);
  CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const float animTime, const bool show = true);
  virtual ~CGUITextAnimation();

  virtual void  Update(const float timeDelta) override;

  void SetVisible(const bool visible);
  void Show();
  void Hide();

protected:
  virtual void StateChange(const State oldState, State & newState) override;
};





class CGUITextCountAnimationBase :
  public CGUIController {
protected:
  CGUITextControl* m_pControl;
  std::string m_FormatText;
  float m_AnimTime;
  float m_Time;
  bool m_Visible;

public:
  CGUITextCountAnimationBase(CGUIControllerList* pList, CGUITextControl* pControl, const std::string& formatText, const float animTime, const bool show);
  virtual ~CGUITextCountAnimationBase();

  // Inherited via CGUIController
  virtual void Update(const float timeDelta) override;

  void SetVisible(const bool value);

protected:
  virtual void StateChange(const State oldState, State & newState) override;

  virtual void UpdateValue() = 0;
  virtual const std::string GetValueText() const = 0;
};





template<typename _Type>
class CGUITextCountAnimation :
  public CGUITextCountAnimationBase {
private:
  _Type m_Value;
  _Type m_AnimValue;

public:
  CGUITextCountAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const _Type value, const std::string& formatText, const float animTime, const bool show = true) :
    CGUITextCountAnimationBase(pList, pControl, formatText, animTime, show), m_Value(value), m_AnimValue(_Type(0)) {}
  virtual ~CGUITextCountAnimation() {}

  void SetValue(const _Type value) {
    m_Value = value;
  }

protected:
  // Inherited via CGUITextCountAnimationBase
  virtual void UpdateValue() override {
    m_AnimValue = (_Type)(m_Value * (m_Time / m_AnimTime));
  }

  virtual const std::string GetValueText() const override {
    return helper::format(m_FormatText, m_AnimValue);
  }
};





class CGUIFadeAnimation :
  public CGUIController {
protected:
  CGUIControl* m_pControl;
  float m_AnimTime;
  float m_Time;
  bool  m_Visible;

public:
  CGUIFadeAnimation(CGUIControllerList* pList,
                    CGUIControl* pControl,
                    const float animTime,
                    const bool visible = true);
  virtual ~CGUIFadeAnimation();

  virtual void Update(const float timeDelta) override;

  void SetVisible(const bool visible);
  void Show();
  void Hide();

  const bool IsVisible() const;

protected:
  virtual void StateChange(const State oldState, State & newState) override;

  virtual const glm::vec4 CalcColor() const = 0;
};


class CGUIAlphaFadeAnimation :
  public CGUIFadeAnimation {
protected:
  float m_MinAlpha;
  float m_MaxAlpha;

public:
  CGUIAlphaFadeAnimation(CGUIControllerList* pList,
                         CGUIControl* pControl,
                         const float animTime,
                         const float minAlpha = 0.0f,
                         const float maxAlpha = 1.0f,
                         const bool visible = true);
  virtual ~CGUIAlphaFadeAnimation();

  void SetAlphaRange(const float min, const float max);

protected:

  // Inherited via CGUIFadeAnimation
  virtual const glm::vec4 CalcColor() const override;
};


class CGUIColorFadeAnimation :
  public CGUIFadeAnimation {
protected:
  glm::vec4 m_MinColor;
  glm::vec4 m_MaxColor;

public:
  CGUIColorFadeAnimation(CGUIControllerList* pList,
                         CGUIControl* pControl,
                         const float animTime,
                         const glm::vec4& minColor = glm::vec4(0.0f),
                         const glm::vec4& maxColor = glm::vec4(1.0f),
                         const bool visible = true);
  virtual ~CGUIColorFadeAnimation();

  void SetColorRange(const glm::vec4& min, const glm::vec4& max);

protected:
  // Inherited via CGUIFadeAnimation
  virtual const glm::vec4 CalcColor() const override;
};


class CGUITimer :
  public CGUIController {
private:
  CGUIController* m_pTarget;
  float m_WaitTime;
  float m_Time;

public:
  CGUITimer(CGUIControllerList* pList, const float waitTime, CGUIController* pTarget = nullptr, const bool started = false);
  virtual ~CGUITimer();

  virtual void Update(const float timeDelta) override;

protected:
  // Inherited via CGUIController
  virtual void StateChange(const State oldState, State & newState) override;

};




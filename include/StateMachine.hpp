#pragma once

#include <State.hpp>
#include <functional>
#include <unordered_map>
#include <utility>

class StateMachine {
public:
  void RegisterFactory(StateID id, std::function<State::Ptr()> factory) {
    m_Factories[id] = std::move(factory);
  }

  void RequestChange(StateID id) { m_Pending = id; }

  void ApplyPendingChange() {

    if (m_Pending == StateID::None)
      return;
    if (m_Pending == StateID::Quit) {
      m_QuitRequested = true;
      m_Pending = StateID::None;
      return;
    }

    auto it{m_Factories.find(m_Pending)};
    if (it != m_Factories.end())
      m_Current = it->second();
    m_Pending = StateID::None;
  }

  bool QuitRequested() const { return m_QuitRequested; }

  void HandleInput() {
    if (m_Current)
      m_Current->HandleInput();
  }
  void Update(float dt) {
    if (m_Current)
      m_Current->Update(dt);
  }
  void Draw() {
    if (m_Current)
      m_Current->Draw();
  }

private:
  State::Ptr m_Current{};
  StateID m_Pending{StateID::None};
  std::unordered_map<StateID, std::function<State::Ptr()>> m_Factories{};
  bool m_QuitRequested{false};
};

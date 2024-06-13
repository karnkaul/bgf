#pragma once
#include <bave/input/gamepad.hpp>
#include <bave/services/service.hpp>

namespace bave {
class IGamepadProvider : public IService {
  public:
	[[nodiscard]] virtual auto get_gamepad() const -> Gamepad const& = 0;
};
} // namespace bave

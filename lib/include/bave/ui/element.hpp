#pragma once
#include <bave/graphics/drawable.hpp>

namespace bave::ui {
class IElement : public IDrawable {
  public:
	[[nodiscard]] virtual auto get_size() const -> glm::vec2 = 0;
	[[nodiscard]] virtual auto get_position() const -> glm::vec2 = 0;
	virtual void set_position(glm::vec2 position) = 0;
};
} // namespace bave::ui

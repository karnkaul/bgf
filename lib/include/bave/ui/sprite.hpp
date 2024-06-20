#pragma once
#include <bave/graphics/sprite.hpp>
#include <bave/ui/element.hpp>

namespace bave::ui {
class Sprite : public IElement {
  public:
	[[nodiscard]] auto get_size() const -> glm::vec2 override { return sprite.get_bounds().size(); }
	[[nodiscard]] auto get_position() const -> glm::vec2 override { return sprite.transform.position; }
	void set_position(glm::vec2 position) override { sprite.transform.position = position; }
	void draw(Shader& shader) const override { sprite.draw(shader); }

	bave::Sprite sprite{};
};
} // namespace bave::ui

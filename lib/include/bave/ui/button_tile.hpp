#pragma once
#include <bave/graphics/sprite.hpp>
#include <bave/ui/button.hpp>

namespace bave::ui {
class ButtonTile : public Button {
  public:
	explicit ButtonTile(Services const& services);

	void set_size(glm::vec2 sprite_size, glm::vec2 background_size);
	void set_outline_tint(bave::Rgba rgba);

	void set_position(glm::vec2 position) override;
	void draw(bave::Shader& shader) const override;

	bave::Sprite sprite{};
};
} // namespace bave::ui

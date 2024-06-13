#pragma once
#include <bave/graphics/text.hpp>
#include <bave/services/services.hpp>
#include <bave/ui/element.hpp>

namespace bave::ui {
class Text : public IElement {
  public:
	explicit Text(Services const& services);

	[[nodiscard]] auto get_size() const -> glm::vec2 override { return text.get_bounds().size(); }
	[[nodiscard]] auto get_position() const -> glm::vec2 override { return text.transform.position; }
	void set_position(glm::vec2 position) override { text.transform.position = position; }
	void draw(Shader& shader) const override { text.draw(shader); }

	bave::Text text{};
};
} // namespace bave::ui

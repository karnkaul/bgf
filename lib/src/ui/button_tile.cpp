#include <bave/ui/button_tile.hpp>

namespace bave::ui {
ButtonTile::ButtonTile(Services const& services) : Button(services) {
	min_size = glm::vec2{50.0f};
	auto const& styles = services.get<Styles>();
	set_style(styles.buttons["tile"]);
	set_size(glm::vec2{180.0f}, glm::vec2{200.0f});
}

void ButtonTile::set_size(glm::vec2 const sprite_size, glm::vec2 const background_size) {
	m_background.set_size(background_size);
	sprite.set_auto_size(sprite_size);
}

void ButtonTile::set_outline_tint(bave::Rgba const rgba) { m_style.outline[ButtonState::eIdle] = rgba; }

void ButtonTile::set_position(glm::vec2 const position) {
	Button::set_position(position);
	sprite.transform.position = position;
}

void ButtonTile::draw(Shader& shader) const {
	Button::draw(shader);
	sprite.draw(shader);
}
} // namespace bave::ui

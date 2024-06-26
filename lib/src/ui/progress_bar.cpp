#include <bave/services/styles.hpp>
#include <bave/ui/progress_bar.hpp>

namespace bave::ui {
ProgressBar::ProgressBar(Services const& services) : m_style(services.get<Styles>().progress_bars["default"]) {}

void ProgressBar::set_progress(float progress) {
	m_progress = progress;

	auto const width = progress * m_size.x;

	auto bg_size = m_size;
	bg_size.x += m_style.corner_ratio * bg_size.y;
	auto quad = RoundedQuad{};
	quad.size = bg_size;
	quad.corner_radius = m_style.corner_ratio * std::min(bg_size.x, bg_size.y);
	m_background.set_shape(quad);

	quad.size = m_size - m_style.padding;
	quad.size.x = width;
	quad.corner_radius = m_style.corner_ratio * std::min(quad.size.x, quad.size.y);
	m_fill.set_shape(quad);

	m_background.tint = m_style.background;
	m_fill.tint = m_style.fill;

	m_fill.tint.channels.w = 0xff;

	set_position(m_background.transform.position);
}

void ProgressBar::set_position(glm::vec2 const position) {
	m_background.transform.position = m_fill.transform.position = position;
	m_fill.transform.position.x -= 0.5f * (m_size.x - m_fill.get_shape().size.x);
}

void ProgressBar::set_size(glm::vec2 const size) {
	m_size = size;
	set_progress(m_progress);
}

void ProgressBar::set_style(Style style) {
	m_style = style;
	set_progress(m_progress);
}

void ProgressBar::draw(Shader& shader) const {
	m_background.draw(shader);
	m_fill.draw(shader);
}
} // namespace bave::ui

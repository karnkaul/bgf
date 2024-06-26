#include <bave/services/resources.hpp>
#include <bave/services/styles.hpp>
#include <bave/ui/loading_screen.hpp>

namespace bave::ui {
LoadingScreen::LoadingScreen(Services const& services)
	: m_ui_space(&services.get<Display>().ui), m_style(services.get<Styles>().loading_screen), m_progress_bar(services) {
	auto const space = m_ui_space->get_size();

	m_background.set_size(1.1f * space);
	m_background.tint = m_style.background_tint;

	m_spinner.set_size(m_style.spinner.size);
	m_spinner.set_texture(services.get<Resources>().spinner);

	m_progress_bar.set_size({m_style.progress_bar.n_width * space.x, m_style.progress_bar.height});
	m_progress_bar.set_position({0.0f, -0.5f * space.y + m_style.progress_bar.bottom_offset});
	auto style = m_progress_bar.get_style();
	style.padding = m_style.progress_bar.padding;
	m_progress_bar.set_style(style);
}

void LoadingScreen::update(Seconds const dt, float const progress) {
	m_progress_bar.set_progress(progress);

	Degrees rotation = m_spinner.transform.rotation;
	rotation.value -= m_style.spinner.spin_rate.value * dt.count();
	m_spinner.transform.rotation = rotation;
}

void LoadingScreen::draw(Shader& shader) const {
	shader.set_render_view(m_ui_space->render_view);
	m_background.draw(shader);
	m_spinner.draw(shader);
	m_progress_bar.draw(shader);
}
} // namespace bave::ui

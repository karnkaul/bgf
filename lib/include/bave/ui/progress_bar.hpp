#pragma once
#include <bave/services/services.hpp>
#include <bave/ui/element.hpp>
#include <bave/ui/outline_quad.hpp>
#include <bave/ui/style.hpp>

namespace bave::ui {
class ProgressBar : public IElement {
  public:
	using Style = ProgressBarStyle;

	explicit ProgressBar(Services const& services);

	glm::vec2 size{200.0f, 20.0f};
	glm::vec2 position{};

	void draw(Shader& shader) const final;

	[[nodiscard]] auto get_progress() const -> float { return m_progress; }
	void set_progress(float progress);

	[[nodiscard]] auto get_style() const -> Style const& { return m_style; }
	void set_style(Style style);

	[[nodiscard]] auto get_size() const -> glm::vec2 final { return size; }
	[[nodiscard]] auto get_position() const -> glm::vec2 final { return position; }
	void set_position(glm::vec2 position) final { this->position = position; }

	[[nodiscard]] auto get_bounds() const -> Rect<> { return m_background.get_bounds(); }

  private:
	Style m_style{};

	RoundedQuadShape m_background{};
	QuadShape m_fill{};

	float m_progress{};
};
} // namespace bave::ui

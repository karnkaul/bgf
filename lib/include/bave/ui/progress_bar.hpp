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

	void draw(Shader& shader) const final;

	[[nodiscard]] auto get_size() const -> glm::vec2 final { return m_size; }
	[[nodiscard]] auto get_position() const -> glm::vec2 final { return m_background.transform.position; }
	void set_position(glm::vec2 position) final;

	void set_size(glm::vec2 size);

	[[nodiscard]] auto get_progress() const -> float { return m_progress; }
	void set_progress(float progress);

	[[nodiscard]] auto get_style() const -> Style const& { return m_style; }
	void set_style(Style style);

	[[nodiscard]] auto get_bounds() const -> Rect<> { return m_background.get_bounds(); }

  private:
	Style m_style{};
	glm::vec2 m_size{300.0f, 35.0f};

	RoundedQuadShape m_background{};
	RoundedQuadShape m_fill{};

	float m_progress{};
};
} // namespace bave::ui

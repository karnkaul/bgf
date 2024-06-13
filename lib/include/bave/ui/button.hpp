#pragma once
#include <bave/graphics/text.hpp>
#include <bave/services/display.hpp>
#include <bave/services/services.hpp>
#include <bave/services/styles.hpp>
#include <bave/ui/outline_quad.hpp>
#include <bave/ui/style.hpp>
#include <bave/ui/widget.hpp>
#include <functional>

namespace bave::ui {
class Button : public IWidget {
  public:
	using State = ButtonState;
	using Style = ButtonStyle;

	static constexpr auto min_size_v = glm::vec2{200.0f, 50.0f};

	explicit Button(Services const& services);

	[[nodiscard]] auto get_size() const -> glm::vec2 final { return m_background.get_size(); }
	void set_position(glm::vec2 position) final;
	[[nodiscard]] auto get_position() const -> glm::vec2 final { return m_background.get_position(); }

	void set_scale(glm::vec2 scale);
	void set_font(std::shared_ptr<Font> font);
	void set_text(std::string text, bool resize = true);
	void set_text_height(TextHeight height, bool resize = true);
	void set_size(glm::vec2 size);

	[[nodiscard]] auto get_style() const -> Style { return m_style; }
	void set_style(Style const& style);

	[[nodiscard]] auto get_scale() const -> glm::vec2 { return m_background.get_background().transform.scale; }
	[[nodiscard]] auto get_text() const -> std::string_view { return m_text.get_string(); }

	[[nodiscard]] auto get_state() const -> State { return m_state; }

	std::function<void()> callback{};

  private:
	void on_move(PointerMove const& pointer_move) final;
	void on_tap(PointerTap const& pointer_tap) final;
	void tick(Seconds dt) final;
	void draw(Shader& shader) const final;

	void resize_background();
	void reposition_text();

	NotNull<IDisplay const*> m_display;
	NotNull<Styles const*> m_styles;

	OutlineQuad m_background{};
	Text m_text{};

	State m_state{};
	Style m_style{};
	Rect<> m_hitbox{};
};
} // namespace bave::ui

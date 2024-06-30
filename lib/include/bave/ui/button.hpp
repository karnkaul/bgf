#pragma once
#include <bave/graphics/text.hpp>
#include <bave/services/audio.hpp>
#include <bave/services/display.hpp>
#include <bave/services/resources.hpp>
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

	explicit Button(Services const& services);

	void on_move(PointerMove const& pointer_move) override;
	void on_tap(PointerTap const& pointer_tap) override;
	void tick(Seconds dt) override;
	void draw(Shader& shader) const override;

	[[nodiscard]] auto get_size() const -> glm::vec2 override { return m_background.get_size(); }
	void set_position(glm::vec2 position) override;
	[[nodiscard]] auto get_position() const -> glm::vec2 override { return m_background.get_position(); }

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
	glm::vec2 min_size{200.0f, 50.0f};

  protected:
	void resize_background();
	void reposition_text();

	NotNull<Styles const*> m_styles;
	NotNull<Resources const*> m_resources;
	NotNull<IAudio*> m_audio;

	OutlineQuad m_background{};
	Text m_text{};

	State m_state{};
	Style m_style{};
	Rect<> m_hitbox{};
};
} // namespace bave::ui

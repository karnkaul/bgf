#pragma once
#include <bave/services/display.hpp>
#include <bave/services/services.hpp>
#include <bave/ui/widget.hpp>
#include <memory>
#include <optional>
#include <vector>

namespace bave::ui {
class View : public Polymorphic {
  public:
	explicit View(Services const& services);

	void push(std::unique_ptr<IElement> element);

	[[nodiscard]] auto is_destroyed() const -> bool { return m_destroyed; }
	void set_destroyed() { m_destroyed = true; }

	[[nodiscard]] auto unproject(glm::vec2 const pointer) const { return m_display->unproject(render_view.value_or(m_display->get_default_view()), pointer); }

	virtual void on_move(PointerMove const& pointer_move);
	virtual void on_tap(PointerTap const& pointer_tap);
	virtual void tick(Seconds dt);
	virtual void render(Shader& shader) const;

	std::optional<RenderView> render_view{};
	bool block_input_events{true};
	bool active{true};

  protected:
	std::vector<std::unique_ptr<ui::IElement>> m_elements{};
	std::vector<Ptr<ui::IWidget>> m_widgets{};
	NotNull<IDisplay const*> m_display;
	bool m_destroyed{};
};
} // namespace bave::ui

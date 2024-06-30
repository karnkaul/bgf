#pragma once
#include <bave/services/services.hpp>
#include <bave/ui/widget.hpp>
#include <memory>
#include <optional>
#include <vector>

namespace bave::ui {
class View : public Polymorphic {
  public:
	void push(std::unique_ptr<IElement> element);

	[[nodiscard]] auto is_destroyed() const -> bool { return m_destroyed; }
	void set_destroyed() { m_destroyed = true; }

	virtual void on_move(PointerMove const& pointer_move);
	virtual void on_tap(PointerTap const& pointer_tap);
	virtual void tick(Seconds dt);
	virtual void render(Shader& shader) const;

	// leave null to use IDisplay::ui_view().
	std::optional<RenderView> render_view{};
	bool block_input_events{true};
	bool active{true};

  protected:
	std::vector<std::unique_ptr<ui::IElement>> m_elements{};
	std::vector<Ptr<ui::IWidget>> m_widgets{};
	bool m_destroyed{};
};
} // namespace bave::ui

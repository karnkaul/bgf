#pragma once
#include <bave/driver.hpp>
#include <bave/scene.hpp>
#include <bave/services/scene_switcher.hpp>

namespace bave {
class IAudio;
class Resources;

class GameDriver : public Driver {
  public:
	struct CreateInfo {
		struct Assets {
			struct {
				std::string_view uri{"fonts/main.ttf"};
				std::span<TextHeight const> preload_heights{};
			} main_font{};
			std::string_view spinner{"images/spinner.png"};
			std::string_view styles{"styles.json"};
		};

		Assets assets{};
	};

	explicit GameDriver(App& app, CreateInfo const& create_info);

  protected:
	void on_focus(FocusChange const& focus_change) override;
	void on_resize(WindowResize const& window_resize) override;
	void on_resize(FramebufferResize const& framebuffer_resize) override;
	void on_key(KeyInput const& key_input) override;
	void on_char(CharInput const& char_input) override;
	void on_tap(PointerTap const& pointer_tap) override;
	void on_move(PointerMove const& pointer_move) override;
	void on_scroll(MouseScroll const& mouse_scroll) override;
	void on_drop(std::span<std::string const> paths) override;

	void tick() override;
	void render() const override;

	[[nodiscard]] auto get_switcher() const -> ISceneSwitcher& { return m_services.get<ISceneSwitcher>(); }
	[[nodiscard]] auto make_loader() const -> Loader { return Loader{&get_app().get_data_store(), &get_app().get_render_device()}; }

	Services m_services;
	InclusiveRange<Seconds> m_dt_limit{0s, 1s};

  private:
	struct SceneSwitcher;
	struct Display;

	void load_resources(CreateInfo::Assets const& assets);
	void bind_services();

	Ptr<SceneSwitcher> m_switcher{};
	Ptr<Resources> m_resources{};
	Ptr<Display> m_display{};
	Ptr<IAudio> m_audio{};

	std::unique_ptr<Scene> m_scene{};
};
} // namespace bave

#pragma once
#include <bave/app.hpp>
#include <bave/assets/asset_manifest.hpp>
#include <bave/async_exec.hpp>
#include <bave/core/polymorphic.hpp>
#include <bave/input/event_sink.hpp>
#include <bave/loader.hpp>
#include <bave/services/services.hpp>
#include <bave/ui/loading_screen.hpp>
#include <bave/ui/view.hpp>

namespace bave {
class ISceneSwitcher;

class Scene : public EventSink {
  public:
	[[nodiscard]] auto get_app() const -> App& { return m_app; }
	[[nodiscard]] auto get_services() const -> Services const& { return m_services; }
	[[nodiscard]] auto get_switcher() const -> ISceneSwitcher&;
	[[nodiscard]] auto make_loader() const -> Loader { return Loader{&m_app.get_data_store(), &m_app.get_render_device()}; }

	[[nodiscard]] auto is_loading() const -> bool { return m_loading_screen.has_value(); }
	[[nodiscard]] auto is_ui_blocking_input() const -> bool;

	void push_view(std::unique_ptr<ui::View> view);
	auto pop_view() -> std::unique_ptr<ui::View>;

	void switch_track(std::string_view uri, bool force_restart = false) const;

	Rgba clear_colour{black_v};

  protected:
	explicit Scene(App& app, Services const& services, std::string name);

	virtual void start_loading();
	virtual auto get_asset_manifest() -> AssetManifest { return {}; }
	virtual void on_loaded() {}

	virtual void tick(Seconds /*dt*/) {}
	virtual void render(Shader& /*shader*/) const {}

	Logger m_log{};
	struct {
		std::string_view vert{"shaders/default.vert"};
		std::string_view frag{"shaders/default.frag"};
	} m_shader{};

  private:
	template <typename F>
	auto on_ui_event(F per_view) -> bool;

	auto cache_views() -> std::span<Ptr<ui::View> const>;

	void update_loading(Seconds dt);
	auto render_loading(Shader& shader) const -> bool;

	void on_focus_event(FocusChange const& focus_change);
	void on_key_event(KeyInput const& key_input);
	void on_move_event(PointerMove const& pointer_move);
	void on_tap_event(PointerTap const& pointer_tap);
	void on_scroll_event(MouseScroll const& mouse_scroll);
	void tick_frame(Seconds dt);
	void render_frame() const;

	App& m_app;
	Services const& m_services;
	Display const& m_display;
	std::vector<std::unique_ptr<ui::View>> m_views{};
	std::vector<Ptr<ui::View>> m_cached_views{};

	std::optional<AsyncExec> m_load{};
	std::optional<ui::LoadingScreen> m_loading_screen{};

	friend class GameDriver;
};

class EmptyScene : public Scene {
  public:
	explicit EmptyScene(App& app, Services const& services) : Scene(app, services, "EmptyScene") {}
};
} // namespace bave

#include <bave/assets/asset_list.hpp>
#include <bave/scene.hpp>
#include <bave/services/audio.hpp>
#include <bave/services/display.hpp>
#include <bave/services/scene_switcher.hpp>
#include <algorithm>

namespace bave {
Scene::Scene(App& app, Services const& services, std::string name)
	: m_log{std::move(name)}, m_app(app), m_services(services), m_display(services.get<Display>()) {
	m_log.info("constructed");
}

auto Scene::get_switcher() const -> ISceneSwitcher& { return m_services.get<ISceneSwitcher>(); }

auto Scene::is_ui_blocking_input() const -> bool {
	return std::any_of(m_views.begin(), m_views.end(), [](auto const& view) { return view->block_input_events; });
}

void Scene::push_view(std::unique_ptr<ui::View> view) {
	if (!view || view->is_destroyed()) { return; }
	m_views.push_back(std::move(view));
}

auto Scene::pop_view() -> std::unique_ptr<ui::View> {
	if (m_views.empty()) { return {}; }
	auto ret = std::move(m_views.back());
	m_views.pop_back();
	return ret;
}

void Scene::switch_track(std::string_view const uri, bool const force_restart) const {
	auto& audio = get_services().get<IAudio>();
	if (uri.empty()) {
		audio.stop_music();
	} else {
		if (force_restart || audio.get_music_uri() != uri) { audio.play_music(uri); }
	}
}

void Scene::start_loading() {
	auto asset_manifest = get_asset_manifest();
	if (asset_manifest.is_empty()) {
		on_loaded();
		return;
	}

	auto asset_list = AssetList{make_loader(), get_services()};
	asset_list.add_manifest(std::move(asset_manifest));
	m_load.emplace(asset_list.build_load_stage());
	m_loading_screen.emplace(m_services);
}

template <typename F>
auto Scene::on_ui_event(F per_view) -> bool {
	auto const cached_views = cache_views();
	for (auto it = cached_views.rbegin(); it != cached_views.rend(); ++it) {
		auto const& view = *it;
		if (!view->active) { continue; }
		per_view(*view);
		if (view->block_input_events) { return true; }
	}
	return false;
}

auto Scene::cache_views() -> std::span<Ptr<ui::View> const> {
	m_cached_views.clear();
	m_cached_views.reserve(m_views.size());
	for (auto const& view : m_views) { m_cached_views.push_back(view.get()); }
	return m_cached_views;
}

void Scene::update_loading(Seconds const dt) {
	if (!m_loading_screen) { return; }

	if (!m_load) {
		m_loading_screen->update(dt, 1.0f);
		return;
	}

	auto const load_status = m_load->update();
	auto const progress = load_status.progress();
	m_loading_screen->update(dt, progress);

	if (load_status.is_complete()) {
		on_loaded();
		m_loading_screen.reset();
		m_load.reset();
	}
}

auto Scene::render_loading(Shader& shader) const -> bool {
	if (!m_loading_screen) { return false; }
	m_loading_screen->draw(shader);
	return true;
}

void Scene::on_key_event(KeyInput const& key_input) {
	if (is_loading()) { return; }
	on_key(key_input);
}

void Scene::on_focus_event(FocusChange const& focus_change) {
	if (is_loading()) { return; }
	on_focus(focus_change);
}

void Scene::on_move_event(PointerMove const& pointer_move) {
	if (is_loading()) { return; }

	auto const& render_device = get_app().get_render_device();
	auto const& ui_view = m_display.ui.render_view;
	auto const func = [pointer_move, &render_device, &ui_view](ui::View& view) {
		auto pm = pointer_move;
		pm.pointer.position = render_device.unproject_to(view.render_view.value_or(ui_view), pointer_move.pointer.position);
		view.on_move(pm);
	};
	if (on_ui_event(func)) { return; }

	auto pm = pointer_move;
	pm.pointer.position = m_display.world.unproject(pointer_move.pointer.position);
	on_move(pm);
}

void Scene::on_tap_event(PointerTap const& pointer_tap) {
	if (is_loading()) { return; }

	auto const& render_device = get_app().get_render_device();
	auto const& ui_view = m_display.ui.render_view;
	auto const func = [pointer_tap, &render_device, &ui_view](ui::View& view) {
		auto pt = pointer_tap;
		pt.pointer.position = render_device.unproject_to(view.render_view.value_or(ui_view), pointer_tap.pointer.position);
		view.on_tap(pt);
	};
	if (on_ui_event(func)) { return; }

	auto pt = pointer_tap;
	pt.pointer.position = m_display.world.unproject(pointer_tap.pointer.position);
	on_tap(pt);
}

void Scene::on_scroll_event(MouseScroll const& mouse_scroll) {
	if (is_loading()) { return; }
	on_scroll(mouse_scroll);
}

void Scene::tick_frame(Seconds const dt) {
	update_loading(dt);
	if (is_loading()) { return; }
	tick(dt);
	for (auto const& view : cache_views()) { view->tick(dt); }
	std::erase_if(m_views, [](auto const& view) { return view->is_destroyed(); });
}

void Scene::render_frame() const {
	auto shader = get_app().load_shader(m_shader.vert, m_shader.frag);
	if (!shader) { return; }
	if (render_loading(*shader)) { return; }

	shader->set_render_view(m_display.world.render_view);
	render(*shader);

	for (auto const& view : m_views) {
		shader->set_render_view(view->render_view.value_or(m_display.ui.render_view));
		view->render(*shader);
	}
}
} // namespace bave

#include <bave/game_driver.hpp>
#include <bave/services/audio.hpp>
#include <bave/services/resources.hpp>
#include <bave/services/scene_switcher.hpp>
#include <bave/services/styles.hpp>
#include <bave/ui/dialog.hpp>

namespace bave {
namespace {
struct Audio : IAudio {
	NotNull<AudioDevice*> audio_device;
	NotNull<AudioStreamer*> audio_streamer;
	NotNull<Resources const*> resources;
	std::string_view interact_sfx_uri{};
	std::string music_uri{};

	explicit Audio(NotNull<AudioDevice*> audio_device, NotNull<AudioStreamer*> audio_streamer, NotNull<Resources const*> resources)
		: audio_device(audio_device), audio_streamer(audio_streamer), resources(resources) {}

	[[nodiscard]] auto get_audio_device() const -> AudioDevice& final { return *audio_device; }

	[[nodiscard]] auto get_sfx_gain() const -> float final { return audio_device->sfx_gain; }
	void set_sfx_gain(float const gain) final { audio_device->sfx_gain = gain; }

	[[nodiscard]] auto get_music_gain() const -> float final { return audio_streamer->gain; }
	void set_music_gain(float const gain) final { audio_streamer->gain = gain; }

	void play_sfx(std::string_view const uri) final {
		auto const clip = resources->get<AudioClip>(uri);
		if (!clip) { return; }
		audio_device->play_once(*clip);
	}

	void play_music(std::string_view const uri, Seconds const cross_fade) final {
		auto const clip = resources->get<AudioClip>(uri);
		if (!clip) {
			stop_music(cross_fade);
			music_uri.clear();
			return;
		}
		music_uri = std::string{uri};
		audio_streamer->play(clip, cross_fade);
	}

	void stop_music(Seconds const fadeout) final { audio_streamer->stop(fadeout); }

	[[nodiscard]] auto get_music_uri() const -> std::string_view final { return music_uri; }
};
} // namespace

struct GameDriver::SceneSwitcher : ISceneSwitcher {
	App& app;				  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
	Services const& services; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
	std::unique_ptr<Scene> next_scene{};

	explicit SceneSwitcher(App& app, Services const& services) : app(app), services(services) {}

	void switch_to_scene(std::unique_ptr<Scene> new_scene) final { next_scene = std::move(new_scene); }

	[[nodiscard]] auto get_app() const -> App& final { return app; }
	[[nodiscard]] auto get_services() const -> Services const& final { return services; }
};

GameDriver::GameDriver(App& app, CreateInfo const& create_info) : Driver(app) {
	load_resources(create_info.assets);
	bind_services(create_info.assets.interact_sfx);
	m_scene = std::make_unique<EmptyScene>(app, m_services);
}

void GameDriver::on_focus(FocusChange const& focus_change) { m_scene->on_focus_event(focus_change); }

void GameDriver::on_resize(WindowResize const& window_resize) { m_scene->on_resize(window_resize); }

void GameDriver::on_resize(FramebufferResize const& framebuffer_resize) { m_scene->on_resize(framebuffer_resize); }

void GameDriver::on_key(KeyInput const& key_input) { m_scene->on_key_event(key_input); }

void GameDriver::on_char(CharInput const& char_input) { m_scene->on_char(char_input); }

void GameDriver::on_tap(PointerTap const& pointer_tap) { m_scene->on_tap_event(pointer_tap); }

void GameDriver::on_move(PointerMove const& pointer_move) { m_scene->on_move_event(pointer_move); }

void GameDriver::on_scroll(MouseScroll const& mouse_scroll) { m_scene->on_scroll_event(mouse_scroll); }

void GameDriver::on_drop(std::span<std::string const> paths) { m_scene->on_drop(paths); }

void GameDriver::tick() {
	m_display->ui.sync_to_default_view();

	if (m_switcher->next_scene) {
		m_scene = std::move(m_switcher->next_scene);
		m_resources->clear();
		m_scene->start_loading();
	}

	auto const dt = std::clamp(get_app().get_dt(), m_dt_limit.lo, m_dt_limit.hi);
	m_scene->tick_frame(dt);

	clear_colour = m_scene->clear_colour;
}

void GameDriver::render() const { m_scene->render_frame(); }

void GameDriver::load_resources(CreateInfo::Assets const& assets) {
	auto resources = std::make_unique<Resources>();
	m_resources = resources.get();
	auto const loader = make_loader();
	auto preload_heights = std::vector(assets.main_font.preload_heights.begin(), assets.main_font.preload_heights.end());
	preload_heights.push_back(ui::Dialog::text_height_v);
	resources->main_font = loader.load_font(assets.main_font.uri, preload_heights);
	resources->spinner = loader.load_texture(assets.spinner, true);
	resources->interact_sfx = loader.load_audio_clip(assets.interact_sfx);
	m_services.bind<Resources>(std::move(resources));

	auto styles = std::make_unique<Styles>();
	if (auto const json = loader.load_json(assets.styles)) { *styles = Styles::load(json); }
	m_services.bind<Styles>(std::move(styles));
}

void GameDriver::bind_services(std::string_view const interact_sfx_uri) {
	auto display = std::make_unique<Display>(&get_app().get_render_device());
	m_display = display.get();
	m_services.bind<Display>(std::move(display));

	auto switcher = std::make_unique<SceneSwitcher>(get_app(), m_services);
	m_switcher = switcher.get();
	m_services.bind<ISceneSwitcher>(std::move(switcher));

	auto audio = std::make_unique<Audio>(&get_app().get_audio_device(), &get_app().get_audio_streamer(), &m_services.get<Resources>());
	audio->interact_sfx_uri = interact_sfx_uri;
	m_audio = audio.get();
	m_services.bind<IAudio>(std::move(audio));
}
} // namespace bave

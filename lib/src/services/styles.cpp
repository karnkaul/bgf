#include <bave/io/json_io.hpp>
#include <bave/services/styles.hpp>

namespace bave {
namespace {
struct StyleLoader {
	StringMap<Rgba> const& rgbas; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

	template <typename Type>
	void load(StringMap<Type>& out, dj::Json const& json) const {
		for (auto const& [id, in_style] : json.object_view()) {
			if (id.empty()) { continue; }
			auto out_style = Type{};
			from(in_style, out_style);
			out.insert_or_assign(std::string{id}, out_style);
		}
	}

	template <typename Type>
	void from(dj::Json const& json, Type& out) const {
		from_json(json, out);
	}

	void from(dj::Json const& json, Rgba& out) const {
		auto const str = json.as_string();
		if (str.starts_with('#')) {
			from_json(json, out);
		} else {
			out = rgbas[str];
		}
	}

	void from(dj::Json const& json, ui::LoadingScreenStyle& out) const {
		from(json["background_tint"], out.background_tint);
		auto const& spinner = json["spinner"];
		from(spinner["size"], out.spinner.size);
		from(spinner["spin_rate"], out.spinner.spin_rate.value);
		auto const& progress_bar = json["progress_bar"];
		from(progress_bar["n_width"], out.progress_bar.n_width);
		from(progress_bar["height"], out.progress_bar.height);
		from(progress_bar["bottom_offset"], out.progress_bar.bottom_offset);
		from(progress_bar["padding"], out.progress_bar.padding);
	}

	void from(dj::Json const& json, ui::ButtonStyle::Tint& out) const {
		from(json["idle"], out[ui::ButtonState::eIdle]);
		from(json["hover"], out[ui::ButtonState::eHover]);
		from(json["press"], out[ui::ButtonState::ePress]);
	}

	void from(dj::Json const& json, ui::ButtonStyle& out) const {
		from(json["background"], out.background);
		from(json["outline"], out.outline);
		from(json["text"], out.text);
		from(json["outline_width"], out.outline_width);
		from(json["corner_ratio"], out.corner_ratio);
	}

	void from(dj::Json const& json, ui::ProgressBarStyle& out) const {
		from(json["background"], out.background);
		from(json["fill"], out.fill);
		from(json["corner_ratio"], out.corner_ratio);
		from(json["padding"], out.padding);
	}

	void from(dj::Json const& json, ui::DialogStyle& out) const {
		from(json["footer_padding"], out.footer_padding);
		from(json["corner_ratio"], out.corner_ratio);
		from(json["background_tint"], out.background_tint);
		from(json["outline_tint"], out.outline_tint);
		from(json["content_text_tint"], out.content_text_tint);
	}

	void from(dj::Json const& json, ui::SliderStyle& out) const {
		from(json["progress_bar"], out.progress_bar);
		from(json["knob_diameter"], out.knob_diameter);
		from(json["knob_tint"], out.knob_tint);
	}
};

struct StyleSaver {
	template <typename Type>
	static void save(StringMap<Type> const& styles, dj::Json& out, std::string_view const key) {
		auto out_styles = dj::Json{};
		for (auto const& [id, button_style] : styles) {
			if (id.empty()) { continue; }
			to(out_styles[id], button_style);
		}
		if (out_styles) { out[key] = std::move(out_styles); }
	}

	template <typename Type>
	static void to(dj::Json& json, Type const& out) {
		to_json(json, out);
	}

	static void to(dj::Json& out, ui::LoadingScreenStyle const& loading_screen_style) {
		to_json(out["background_tint"], loading_screen_style.background_tint);
		auto& spinner = out["spinner"];
		to_json(spinner["size"], loading_screen_style.spinner.size);
		to_json(spinner["spin_rate"], loading_screen_style.spinner.spin_rate.value);
		auto& progress_bar = out["progress_bar"];
		to_json(progress_bar["n_width"], loading_screen_style.progress_bar.n_width);
		to_json(progress_bar["height"], loading_screen_style.progress_bar.height);
		to_json(progress_bar["bottom_offset"], loading_screen_style.progress_bar.bottom_offset);
		to_json(progress_bar["padding"], loading_screen_style.progress_bar.padding);
	}

	static void to(dj::Json& out, ui::ButtonStyle::Tint const& button_tint) {
		to_json(out["idle"], button_tint[ui::ButtonState::eIdle]);
		to_json(out["hover"], button_tint[ui::ButtonState::eHover]);
		to_json(out["press"], button_tint[ui::ButtonState::ePress]);
	}

	static void to(dj::Json& out, ui::ButtonStyle const& button_style) {
		to(out["background"], button_style.background);
		to(out["outline"], button_style.outline);
		to(out["text"], button_style.text);
		out["outline_width"] = button_style.outline_width;
		out["corner_ratio"] = button_style.corner_ratio;
	}

	static void to(dj::Json& out, ui::ProgressBarStyle const& progress_bar_style) {
		to_json(out["background"], progress_bar_style.background);
		to_json(out["fill"], progress_bar_style.fill);
		out["corner_ratio"] = progress_bar_style.corner_ratio;
		out["padding"] = progress_bar_style.padding;
	}

	static void to(dj::Json& out, ui::DialogStyle const& dialog_style) {
		to_json(out["footer_padding"], dialog_style.footer_padding);
		to_json(out["corner_ratio"], dialog_style.corner_ratio);
		to_json(out["background_tint"], dialog_style.background_tint);
		to_json(out["outline_tint"], dialog_style.outline_tint);
		to_json(out["content_text_tint"], dialog_style.content_text_tint);
	}

	static void to(dj::Json& out, ui::SliderStyle const& slider_style) {
		to_json(out["progress_bar"], slider_style.progress_bar);
		to_json(out["knob_diameter"], slider_style.knob_diameter);
		to_json(out["knob_tint"], slider_style.knob_tint);
	}
};
} // namespace

auto Styles::load(dj::Json const& json) -> Styles {
	auto ret = Styles{};
	auto const loader = StyleLoader{ret.rgbas};
	loader.load(ret.rgbas, json["rgbas"]);
	loader.load(ret.buttons, json["buttons"]);
	loader.load(ret.progress_bars, json["progress_bars"]);
	loader.load(ret.dialogs, json["dialogs"]);
	loader.load(ret.sliders, json["sliders"]);
	if (auto const& loading_screen = json["loading_screen"]) { loader.from(loading_screen, ret.loading_screen); }
	return ret;
}

auto Styles::save() const -> dj::Json {
	auto ret = dj::Json{};
	StyleSaver::save(rgbas, ret, "rgbas");
	StyleSaver::save(rgbas, ret, "rgbas");
	StyleSaver::save(buttons, ret, "buttons");
	StyleSaver::save(progress_bars, ret, "progress_bars");
	StyleSaver::save(dialogs, ret, "dialogs");
	StyleSaver::save(sliders, ret, "sliders");
	StyleSaver::to(ret["loading_screen"], loading_screen);
	return ret;
}
} // namespace bave

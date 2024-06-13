#pragma once
#include <bave/graphics/rgba.hpp>
#include <bave/services/service.hpp>
#include <bave/string_map.hpp>
#include <bave/ui/style.hpp>

namespace dj {
class Json;
}

namespace bave {
struct Styles : IService {
	StringMap<Rgba> rgbas{};
	StringMap<ui::ButtonStyle> buttons{};
	StringMap<ui::ProgressBarStyle> progress_bars{};
	StringMap<ui::DialogStyle> dialogs{};
	StringMap<ui::SliderStyle> sliders{};
	ui::LoadingScreenStyle loading_screen{};

	static auto load(dj::Json const& json) -> Styles;

	[[nodiscard]] auto save() const -> dj::Json;
};
} // namespace bave

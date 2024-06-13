#pragma once
#include <bave/core/enum_array.hpp>
#include <bave/core/radians.hpp>
#include <bave/font/text_height.hpp>
#include <bave/graphics/rgba.hpp>
#include <glm/vec2.hpp>

namespace bave::ui {
enum class ButtonState { eIdle, eHover, ePress, eCOUNT_ };

struct LoadingScreenStyle {
	Rgba background_tint{black_v};
	struct {
		glm::vec2 size{100.0f};
		Degrees spin_rate{720.0f};
	} spinner{};
	struct {
		float n_width{0.9f};
		float height{50.0f};
		float bottom_offset{200.0f};
		float padding{30.0f};
	} progress_bar{};
};

struct ButtonStyle {
	using Tint = EnumArray<ButtonState, Rgba>;

	Tint background{white_v, yellow_v, cyan_v};
	Tint outline{black_v, black_v, black_v};
	Tint text{black_v, black_v, black_v};
	float outline_width{10.0f};
	float corner_ratio{0.33f};
};

struct ProgressBarStyle {
	Rgba background{white_v};
	Rgba fill{yellow_v};
	float corner_ratio{0.5f};
	float padding{10.0f};
};

struct DialogStyle {
	glm::vec2 footer_padding{20.0f, 10.0f};
	float corner_ratio{0.25f};
	Rgba background_tint{yellow_v};
	Rgba outline_tint{cyan_v};
	Rgba content_text_tint{black_v};
};

struct SliderStyle {
	std::string progress_bar{};
	float knob_diameter{30.0f};
	Rgba knob_tint{white_v};
};
} // namespace bave::ui

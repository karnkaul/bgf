#pragma once
#include <bave/audio/audio_clip.hpp>
#include <bave/font/font.hpp>
#include <bave/graphics/texture.hpp>
#include <bave/resource_map.hpp>
#include <bave/services/service.hpp>
#include <memory>

namespace bave {
struct Resources : ResourceMap, IService {
	std::shared_ptr<Font> main_font{};
	std::shared_ptr<Texture> spinner{};
	std::shared_ptr<AudioClip> interact_sfx{};
};
} // namespace bave

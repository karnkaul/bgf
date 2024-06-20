#pragma once
#include <string>
#include <vector>

namespace dj {
class Json;
}

namespace bave {
struct AssetManifest {
	std::vector<std::string> flat_textures{};
	std::vector<std::string> textures{};
	std::vector<std::string> nine_slices{};
	std::vector<std::string> flat_atlases{};
	std::vector<std::string> atlases{};
	std::vector<std::string> fonts{};
	std::vector<std::string> audio_clips{};
	std::vector<std::string> anim_timelines{};
	std::vector<std::string> particle_emitters{};

	static auto load_from(dj::Json const& json) -> AssetManifest;

	[[nodiscard]] auto is_empty() const -> bool;

	[[nodiscard]] auto make_union(AssetManifest const& other) const -> AssetManifest;
	[[nodiscard]] auto make_complement(AssetManifest const& other) const -> AssetManifest;
};

[[nodiscard]] inline auto operator+(AssetManifest const& a, AssetManifest const& b) -> AssetManifest { return a.make_union(b); }
[[nodiscard]] inline auto operator-(AssetManifest const& a, AssetManifest const& b) -> AssetManifest { return a.make_complement(b); }
} // namespace bave

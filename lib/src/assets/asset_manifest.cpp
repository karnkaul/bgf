#include <bave/assets/asset_manifest.hpp>
#include <djson/json.hpp>
#include <unordered_set>

namespace bave {
namespace {
template <typename T>
auto create_union(T const& a, T const& b) -> T {
	auto ret = a;
	ret.reserve(a.size() + b.size());
	std::copy(b.begin(), b.end(), std::back_inserter(ret));
	return ret;
}

template <typename T>
auto create_complement(T const& a, T const& b) -> T {
	auto ret = a;
	auto to_erase = std::unordered_set<typename T::value_type>{b.begin(), b.end()};
	auto const func = [&to_erase](auto const& t) { return to_erase.contains(t); };
	std::erase_if(ret, func);
	return ret;
}
} // namespace

auto AssetManifest::load_from(dj::Json const& json) -> AssetManifest {
	auto ret = AssetManifest{};
	for (auto const& texture : json["textures"].array_view()) { ret.textures.push_back(texture.as<std::string>()); }
	for (auto const& texture : json["flat_textures"].array_view()) { ret.flat_textures.push_back(texture.as<std::string>()); }
	for (auto const& atlas : json["atlases"].array_view()) { ret.atlases.push_back(atlas.as<std::string>()); }
	for (auto const& atlas : json["flat_atlases"].array_view()) { ret.flat_atlases.push_back(atlas.as<std::string>()); }
	for (auto const& slice : json["nine_slices"].array_view()) { ret.nine_slices.push_back(slice.as<std::string>()); }
	for (auto const& font : json["fonts"].array_view()) { ret.fonts.push_back(font.as<std::string>()); }
	for (auto const& timeline : json["anim_timelines"].array_view()) { ret.anim_timelines.push_back(timeline.as<std::string>()); }
	for (auto const& clip : json["audio_clips"].array_view()) { ret.audio_clips.push_back(clip.as<std::string>()); }
	for (auto const& emitter : json["particle_emitters"].array_view()) { ret.particle_emitters.push_back(emitter.as<std::string>()); }
	return ret;
}

auto AssetManifest::is_empty() const -> bool {
	if (!textures.empty()) { return false; }
	if (!flat_textures.empty()) { return false; }
	if (!atlases.empty()) { return false; }
	if (!flat_atlases.empty()) { return false; }
	if (!nine_slices.empty()) { return false; }
	if (!fonts.empty()) { return false; }
	if (!anim_timelines.empty()) { return false; }
	if (!audio_clips.empty()) { return false; }
	if (!particle_emitters.empty()) { return false; }
	return true;
}

auto AssetManifest::make_union(AssetManifest const& other) const -> AssetManifest {
	auto ret = AssetManifest{};
	ret.textures = create_union(textures, other.textures);
	ret.flat_textures = create_union(flat_textures, other.flat_textures);
	ret.atlases = create_union(atlases, other.atlases);
	ret.flat_atlases = create_union(flat_atlases, other.flat_atlases);
	ret.nine_slices = create_union(nine_slices, other.nine_slices);
	ret.fonts = create_union(fonts, other.fonts);
	ret.anim_timelines = create_union(anim_timelines, other.anim_timelines);
	ret.audio_clips = create_union(audio_clips, other.audio_clips);
	ret.particle_emitters = create_union(particle_emitters, other.particle_emitters);
	return ret;
}

auto AssetManifest::make_complement(AssetManifest const& other) const -> AssetManifest {
	auto ret = AssetManifest{};
	ret.textures = create_complement(textures, other.textures);
	ret.flat_textures = create_complement(flat_textures, other.flat_textures);
	ret.atlases = create_complement(atlases, other.atlases);
	ret.flat_atlases = create_complement(flat_atlases, other.flat_atlases);
	ret.nine_slices = create_complement(nine_slices, other.nine_slices);
	ret.fonts = create_complement(fonts, other.fonts);
	ret.anim_timelines = create_complement(anim_timelines, other.anim_timelines);
	ret.audio_clips = create_complement(audio_clips, other.audio_clips);
	ret.particle_emitters = create_complement(particle_emitters, other.particle_emitters);
	return ret;
}
} // namespace bave

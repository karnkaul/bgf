#pragma once
#include <bave/assets/asset_manifest.hpp>
#include <bave/async_exec.hpp>
#include <bave/loader.hpp>
#include <bave/services/resources.hpp>
#include <bave/services/services.hpp>
#include <unordered_map>
#include <unordered_set>

namespace bave {
struct Resources;
}

namespace bave {
class AssetLoader;

class AssetList {
  public:
	explicit AssetList(Loader loader, Services const& services);

	auto add_texture(std::string uri, bool mip_map = false) -> AssetList&;
	auto add_nine_slice(std::string uri) -> AssetList&;
	auto add_atlas(std::string uri, bool mip_map = false) -> AssetList&;
	auto add_font(std::string uri) -> AssetList&;
	auto add_audio_clip(std::string uri) -> AssetList&;
	auto add_anim_timeline(std::string uri) -> AssetList&;
	auto add_particle_emitter(std::string uri) -> AssetList&;

	void add_manifest(AssetManifest manifest);

	[[nodiscard]] auto build_load_stage() const -> AsyncExec::Stage;

  private:
	Loader m_loader;
	NotNull<Resources*> m_resources;

	std::unordered_map<std::string, bool> m_textures{};
	std::unordered_set<std::string> m_nine_slices{};
	std::unordered_map<std::string, bool> m_atlases{};
	std::unordered_set<std::string> m_fonts{};
	std::unordered_set<std::string> m_audio_clips{};
	std::unordered_set<std::string> m_anim_timelines{};
	std::unordered_set<std::string> m_emitters{};
};
} // namespace bave

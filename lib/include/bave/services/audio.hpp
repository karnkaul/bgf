#pragma once
#include <bave/core/time.hpp>
#include <bave/services/service.hpp>
#include <span>
#include <string_view>

namespace bave {
class IAudio : public IService {
  public:
	[[nodiscard]] virtual auto get_sfx_gain() const -> float = 0;
	virtual void set_sfx_gain(float gain) = 0;

	[[nodiscard]] virtual auto get_music_gain() const -> float = 0;
	virtual void set_music_gain(float gain) = 0;

	virtual void play_sfx(std::string_view uri) = 0;
	virtual void play_music(std::string_view uri, Seconds crossfade = 1s) = 0;
	virtual void stop_music() = 0;

	[[nodiscard]] virtual auto get_music_uri() const -> std::string_view = 0;

	void play_any_sfx(std::span<std::string const> uris);
};
} // namespace bave

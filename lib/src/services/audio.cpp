#include <bave/core/random.hpp>
#include <bave/services/audio.hpp>

namespace bave {
void IAudio::play_any_sfx(std::span<std::string const> uris) {
	if (uris.empty()) { return; }
	if (uris.size() == 1) { return play_sfx(uris.front()); }
	auto const index = random_in_range(std::size_t{}, uris.size() - 1);
	play_sfx(uris[index]);
}
} // namespace bave

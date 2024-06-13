#include <bave/services/resources.hpp>
#include <bave/ui/text.hpp>

namespace bave::ui {
Text::Text(Services const& services) {
	text.set_font(services.get<Resources>().main_font);
	text.tint = white_v;
}
} // namespace bave::ui

#pragma once
#include <bave/core/time.hpp>
#include <bave/graphics/sprite.hpp>
#include <bave/services/display.hpp>
#include <bave/services/services.hpp>
#include <bave/ui/progress_bar.hpp>

namespace bave::ui {
class LoadingScreen {
  public:
	explicit LoadingScreen(Services const& services);

	void update(Seconds dt, float progress);
	void draw(Shader& shader) const;

  private:
	NotNull<IDisplay const*> m_display;
	LoadingScreenStyle m_style{};
	Sprite m_background{};
	Sprite m_spinner{};
	ProgressBar m_progress_bar;
};
} // namespace bave::ui

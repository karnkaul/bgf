#pragma once
#include <bave/app.hpp>
#include <bave/scene.hpp>
#include <bave/services/services.hpp>

namespace bave {
class ISceneSwitcher : public IService {
  public:
	virtual void switch_to_scene(std::unique_ptr<Scene> new_scene) = 0;

	template <std::derived_from<Scene> T, typename... Args>
		requires(std::constructible_from<T, App&, Services const&, Args && ...>)
	void switch_to(Args&&... args) {
		switch_to_scene(std::make_unique<T>(get_app(), get_services(), std::forward<Args>(args)...));
	}

  private:
	[[nodiscard]] virtual auto get_app() const -> App& = 0;
	[[nodiscard]] virtual auto get_services() const -> Services const& = 0;
};
} // namespace bave

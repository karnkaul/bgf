#pragma once
#include <bave/services/service.hpp>
#include <bave/vector_space.hpp>

namespace bave {
class Display : public IService {
  public:
	explicit Display(NotNull<RenderDevice const*> render_device) : world(render_device), ui(render_device) {}

	VectorSpace world;
	VectorSpace ui;
};
} // namespace bave

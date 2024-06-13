#pragma once
#include <bave/core/time.hpp>
#include <bave/graphics/shader.hpp>
#include <bave/input/event.hpp>
#include <bave/ui/element.hpp>

namespace bave::ui {
class IWidget : public IElement {
  public:
	virtual void on_move(PointerMove const& /*pointer_move*/) {}
	virtual void on_tap(PointerTap const& /*pointer_tap*/) {}
	virtual void tick(Seconds /*dt*/) {}
};
} // namespace bave::ui

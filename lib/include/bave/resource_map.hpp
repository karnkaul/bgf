#pragma once
#include <bave/core/polymorphic.hpp>
#include <bave/string_map.hpp>
#include <memory>
#include <typeindex>

namespace bave {
class ResourceMap {
  public:
	template <typename Type>
	auto add(std::string uri, std::shared_ptr<Type> resource) {
		if (uri.empty() || !resource) { return; }
		m_resources.insert_or_assign(std::move(uri), std::make_unique<Model<Type>>(std::move(resource)));
	}

	[[nodiscard]] auto contains(std::string_view const uri) const -> bool { return m_resources.contains(uri); }

	template <typename Type>
	[[nodiscard]] auto contains(std::string_view const uri) const -> bool {
		if (uri.empty()) { return false; }
		auto const it = m_resources.find(uri);
		if (it == m_resources.end()) { return false; }
		return it->second->type_index == typeid(Type);
	}

	template <typename Type>
	[[nodiscard]] auto get(std::string_view const uri, std::shared_ptr<Type> const& fallback = {}) const -> std::shared_ptr<Type> {
		if (uri.empty()) { return fallback; }
		auto const it = m_resources.find(uri);
		if (it == m_resources.end()) { return fallback; }
		auto const& resource = it->second;
		if (resource->type_index != typeid(Type)) { return fallback; }
		return static_cast<Model<Type> const&>(*it->second).resource;
	}

	void clear() { m_resources.clear(); }

	[[nodiscard]] auto size() const -> std::size_t { return m_resources.size(); }

  private:
	struct Base : Polymorphic {
		std::type_index type_index;
		explicit Base(std::type_index type_index) : type_index(type_index) {}
	};

	template <typename T>
	struct Model : Base {
		std::shared_ptr<T> resource;
		explicit Model(std::shared_ptr<T> t) : Base(typeid(T)), resource(std::move(t)) {}
	};

	StringMap<std::unique_ptr<Base>> m_resources{};
};
} // namespace bave

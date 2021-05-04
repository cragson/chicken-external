#pragma once

#include <vector>
#include "feature.hpp"
#include <memory>

class cheat
{

public:

	[[nodiscard]] inline size_t get_features_size() const noexcept
	{
		return this->m_features.size();
	}
	
	bool setup_features();

	void run();

	void shutdown();

private:

	std::vector< std::unique_ptr < feature > > m_features;
};

extern std::unique_ptr< cheat > g_pCheat;
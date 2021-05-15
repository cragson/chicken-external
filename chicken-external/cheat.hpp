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

	static bool setup_offsets();

	void run();

	void shutdown();

	static void print_offsets();
	

private:

	std::vector< std::unique_ptr < feature > > m_features;
};

extern std::unique_ptr< cheat > g_pCheat;
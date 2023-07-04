#include "Game.h"

uint16_t target_fps = 144;
std::chrono::microseconds us_per_update((int64_t)(1.0/target_fps * 1000000));

void Game::run()
{



	auto previous_time = std::chrono::steady_clock::now();
	std::chrono::microseconds lag{0};
	while(true)
	{
		// calculate how long the previous frame took to render
		auto current_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed = current_time - previous_time;
		std::chrono::microseconds elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
		previous_time = current_time;
		lag += elapsed_us;

		// process_input()

		// Fixed update time step with variable rendering.
		while (lag >= us_per_update)
		{
			// update(elapsed_seconds);
			lag -= us_per_update;
		}

		// render(lag / MS_PER_UPDATE)

	}
}
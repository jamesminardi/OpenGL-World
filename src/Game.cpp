#include "Game.h"
#include "Log.h"

[[noreturn]] void Game::run()
{
    Locator::init();

    auto consoleLogger = ConsoleLogger();
    Locator::provide(&consoleLogger);

    Logger& logger = Locator::getLogger();



    std::chrono::microseconds us_per_update((int64_t)(1.0/144 * 1000000)); // target 144 fps

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
        logger.logMsg("Process Input");

		// Fixed update time step with variable rendering.
		while (lag >= us_per_update)
		{
			// update(elapsed_seconds);
            logger.logMsg("Update");
			lag -= us_per_update;
		}

		// render(lag / MS_PER_UPDATE)
        logger.logMsg("Process Input");
	}
}
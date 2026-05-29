#include "audio_level_manager.h"

#include "audio_level_config.h"
#include "logger.h"

PCMFrame AudioLevelManager::normalize(
    const PCMFrame& input)
{
    Logger::log(INFO,
        "AudioLevelManager normalize stub:"
        " STREAMID=" +
        std::to_string(input.streamId) +
        " SOURCE_GAIN_IN_DB=" +
        std::to_string(
            AudioLevelConfig::gainInDb(
                input.sourceProtocol)) +
        " TARGET_GAIN_OUT_DB=" +
        std::to_string(
            AudioLevelConfig::gainOutDb(
                input.targetProtocol)));

    return input;
}

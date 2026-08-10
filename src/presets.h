#pragma once
#include <string>
inline const char* allowedPresets[] = {
    "veryslow",
    "slow",
    "medium",
    "fast"
};

inline std::pair<std::string, std::string> ffmpegPreset(std::string_view presetName) {
    // Returns {"crf", "ffmpeg_preset"}
    if (presetName == "veryslow") {
        return {"2", "veryslow"};
    }
    else if (presetName == "slow") {
        return {"15", "slow"};
    }
    else if (presetName == "medium") {
        return {"18", "medium"};
    }
    else if (presetName == "fast") {
        return {"25", "veryfast"};
    }
    return {"18", "medium"};
}

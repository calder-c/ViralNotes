#include "guimode.h"
#include <deque>
#include <boost/process.hpp>
#include <boost/asio.hpp>
using namespace boost;
class WriterPool {
public:
    asio::io_context ctx{};                          // always constructed, outlives everything
    std::optional<asio::writable_pipe> wp;
    std::optional<process::process> ffmpegProc;
    std::deque<std::vector<std::uint8_t>> ready, pool;
    std::mutex m;
    WriterPool(VisualizerSettings & settings) {
        auto ffmpegPath = process::environment::find_executable("ffmpeg");
        std::initializer_list<std::string> ffmpegInitList = {
            "-framerate", "60",
            "-f", "rawvideo",
            "-pix_fmt", "rgba",
            "-s", std::to_string(settings.SCREEN_X) + "x" + std::to_string(settings.SCREEN_Y),
            "-i", "pipe:0",
            //---------------------------------------
            "-itsoffset", std::to_string(settings.playOffset),
            "-i", settings.musicFilename,
            //---------------------------------------
            "-c:v", "libx264",
        "-preset", "veryfast",
        "-crf", "18",
        "-vf", "scale=trunc(iw/2)*2:trunc(ih/2)*2",
        "-pix_fmt", "yuv420p",
        "-c:a", "aac",
        "-shortest",
        "-y",
            settings.exportPath

        };
        wp = asio::writable_pipe{ctx};
        ffmpegProc = process::process(ctx, ffmpegPath, ffmpegInitList, process::process_stdio{wp.value(), nullptr, {}});
    }
    ~WriterPool() {
        if (wp && ffmpegProc) {
            wp->close();  // EOF -> ffmpeg flushes and finalizes
            ffmpegProc->wait();
        }

    }
    void push(uint8_t ptr, )
};

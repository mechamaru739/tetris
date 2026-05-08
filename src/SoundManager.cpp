#include "tetris/SoundManager.h"
#include <windows.h>

namespace tetris {

SoundManager::SoundManager() : muted_(false) {
}

SoundManager::~SoundManager() {
}

void SoundManager::play(SoundType type) {
    if (muted_) return;

    SoundParams params = getParams(type);

    // 异步播放，不阻塞主线程
    std::thread([this, params]() {
        playAsync(params);
    }).detach();
}

void SoundManager::toggleMute() {
    muted_ = !muted_;
}

bool SoundManager::isMuted() const {
    return muted_;
}

SoundParams SoundManager::getParams(SoundType type) {
    switch (type) {
        case SoundType::MOVE:
            return {200, 30};   // 低音调，短促
        case SoundType::ROTATE:
            return {400, 50};   // 中音调
        case SoundType::SOFT_DROP:
            return {300, 40};   // 中低音调
        case SoundType::HARD_DROP:
            return {150, 80};   // 低音调，较长
        case SoundType::CLEAR_LINE:
            return {600, 100};  // 高音调，较长
        case SoundType::GAME_OVER:
            return {100, 300};  // 很低音调，很长
        default:
            return {400, 50};
    }
}

void SoundManager::playAsync(SoundParams params) {
    std::lock_guard<std::mutex> lock(playMutex_);
    Beep(params.frequency, params.duration);
}

} // namespace tetris

#pragma once

#include <windows.h>
#include <thread>
#include <mutex>
#include <atomic>

namespace tetris {

// 音效类型枚举
enum class SoundType {
    MOVE,       // 左右移动
    ROTATE,     // 旋转
    SOFT_DROP,  // 软降
    HARD_DROP,  // 硬降
    CLEAR_LINE, // 消行
    GAME_OVER   // 游戏结束
};

// 音效参数结构
struct SoundParams {
    DWORD frequency;  // 频率 (Hz)
    DWORD duration;   // 持续时间 (ms)
};

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    // 播放音效（异步，不阻塞）
    void play(SoundType type);

    // 切换静音状态
    void toggleMute();

    // 获取静音状态
    bool isMuted() const;

private:
    std::atomic<bool> muted_;
    std::mutex playMutex_;

    // 获取音效参数
    static SoundParams getParams(SoundType type);

    // 异步播放线程函数
    void playAsync(SoundParams params);
};

} // namespace tetris

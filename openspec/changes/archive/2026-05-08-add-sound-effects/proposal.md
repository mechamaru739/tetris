## Why

当前俄罗斯方块游戏没有音效，玩家无法获得听觉反馈。添加简单的音效可以增强游戏体验，让玩家在移动、旋转、消行等操作时获得即时反馈。

## What Changes

- 添加音效管理器类，负责播放各种游戏音效
- 使用 Windows Beep API 生成简单音效（无需外部音频文件）
- 为以下游戏操作添加音效：
  - 方块左右移动
  - 方块旋转
  - 方块软降（加速下落）
  - 方块硬降（直接落底）
  - 行消除
  - 游戏结束
- 添加音效开关功能（静音/取消静音）

## Capabilities

### New Capabilities

- `sound-effects`: 游戏音效系统，包括音效生成、播放和控制

### Modified Capabilities

（无）

## Impact

- 新增 `SoundManager` 类
- 修改 `Game` 类以集成音效系统
- 修改 `InputHandler` 以支持静音切换
- 使用 Windows API `Beep()` 或 `MessageBeep()` 生成音效
- 不需要外部音频文件或第三方库

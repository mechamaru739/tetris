#pragma once

namespace tetris {

// Raw key states
struct InputState {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool space = false;
    bool enter = false;
    bool escape = false;
    bool mute = false;  // M key for mute toggle
};

// Processed key events (single press)
struct InputEvents {
    bool leftPressed = false;
    bool rightPressed = false;
    bool rotatePressed = false;
    bool dropPressed = false;
    bool downHeld = false;
    bool enterPressed = false;
    bool escapePressed = false;
    bool mutePressed = false;  // M key pressed
};

class InputHandler {
public:
    virtual ~InputHandler() = default;

    // Get current raw key states
    virtual InputState getState() = 0;

    // Get processed events (handles debouncing)
    InputEvents getEvents() {
        InputState current = getState();
        InputEvents events{};

        // Only trigger on key press (not hold)
        events.leftPressed = current.left && !previousState_.left;
        events.rightPressed = current.right && !previousState_.right;
        events.rotatePressed = current.up && !previousState_.up;
        events.dropPressed = current.space && !previousState_.space;
        events.enterPressed = current.enter && !previousState_.enter;
        events.escapePressed = current.escape && !previousState_.escape;
        events.mutePressed = current.mute && !previousState_.mute;

        // Down can be held for continuous soft drop
        events.downHeld = current.down;

        previousState_ = current;
        return events;
    }

private:
    InputState previousState_;
};

} // namespace tetris

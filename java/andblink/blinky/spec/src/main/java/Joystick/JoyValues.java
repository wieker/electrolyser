package Joystick;

import static java.lang.Math.floor;

public class JoyValues implements JoystickMovedListener {
    private int X, Y;
    @Override
    public void OnMoved(float pan, float tilt) {
        X = Double.valueOf(floor(pan * 100)).intValue();
        Y = Double.valueOf(floor(tilt * 100)).intValue();
    }

    @Override
    public void OnReleased() {

    }

    @Override
    public void OnReturnedToCenter() {

    }

    public Integer getX() {
        return X;
    }

    public Integer getY() {
        return Y;
    }
}

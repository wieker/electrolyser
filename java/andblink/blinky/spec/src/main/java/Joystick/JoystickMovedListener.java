package Joystick;

public interface JoystickMovedListener {
    public void OnMoved(float pan, float tilt);
    public void OnReleased();
    public void OnReturnedToCenter();
}

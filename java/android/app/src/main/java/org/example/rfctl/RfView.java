package org.example.rfctl;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.view.View;

public class RfView extends View {

    public RfView(Context context) {
        super(context);
    }
    Paint paint = new Paint(Paint.FAKE_BOLD_TEXT_FLAG);
    Path starPath;
    Path curvePath;


    Paint textPaint = new Paint(Paint.LINEAR_TEXT_FLAG);
    Paint shaderPaint = new Paint();
    {
        Typeface typeface = Typeface.create(
                Typeface.SERIF, Typeface.BOLD);
        textPaint.setTypeface(typeface);
        Shader shader = new LinearGradient(0, 400, 300, 500,
                Color.RED, Color.GREEN, Shader.TileMode.CLAMP);
        shaderPaint.setShader(shader);
        // create star path
        starPath = createStarPath(300, 500);
        curvePath = createCurvePath();
    }

    protected void onDraw(Canvas canvas) {
        // draw basic shapes
        canvas.drawLine(
                1, 1,
                10 * MainActivity.selection, 10 * MainActivity.selection,
                shaderPaint);
    }

    private Path createStarPath(int x, int y) {
        Path path = new Path();
        path.moveTo(0 + x, 150 + y);
        path.lineTo(120 + x, 140 + y);
        path.lineTo(150 + x, 0 + y);
        path.lineTo(180 + x, 140 + y);
        path.lineTo(300 + x, 150 + y);
        path.lineTo(200 + x, 190 + y);
        path.lineTo(250 + x, 300 + y);
        path.lineTo(150 + x, 220 + y);
        path.lineTo(50 + x, 300 + y);
        path.lineTo(100 + x, 190 + y);
        path.lineTo(0 + x, 150 + y);
        return path;
    }

    private Path createCurvePath() {
        Path path = new Path();
        path.addArc(new RectF(400, 40, 780, 300),
                -210, 230);
        return path;
    }
}

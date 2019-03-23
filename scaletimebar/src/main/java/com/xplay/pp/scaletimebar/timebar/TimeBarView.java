package com.xplay.pp.scaletimebar.timebar;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.text.SimpleDateFormat;

public class TimeBarView extends LinearLayout {

    private ScaleTimeBar mScaleTimeBar;
    private String text;
    private TextView textView;
    private float lastX;

    public TimeBarView(@NonNull Context context) {
        this(context, null);
    }

    public TimeBarView(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public TimeBarView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        //设置竖直方向排列
//        setOrientation(VERTICAL);
        setGravity(Gravity.CENTER);
//        addView();
    }

    public long getTime() {
        return mScaleTimeBar.calcCourseTimeMills();
    }

    private void addView() {
        //添加textview
        textView = new TextView(getContext());
        textView.setTextColor(Color.BLACK);
        textView.setTextSize(20);
        textView.setGravity(Gravity.CENTER);
        LayoutParams layoutParams = new LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, 0);
        layoutParams.weight = 1;
        textView.setLayoutParams(layoutParams);
        addView(textView, 0);

        //添加时间戳
        mScaleTimeBar = new ScaleTimeBar(getContext());
        mScaleTimeBar.setOnBarMoveListener(onBarMoveListener);
        LayoutParams timebarLayoutParams = new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 0);
        timebarLayoutParams.weight = 3;
        mScaleTimeBar.setLayoutParams(timebarLayoutParams);
        addView(mScaleTimeBar);
        updateTime(getTime());
    }

    private void init() {
    }

    ScaleTimeBar.OnBarMoveListener onBarMoveListener = new ScaleTimeBar.OnBarMoveListener() {
        @Override
        public void onBarMove(long time) {
            updateTime(time);
        }

        @Override
        public void onBarMoveFinish(long time) {
            updateTime(time);
        }
    };

    SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    private void updateTime(long time) {
        try {
            textView.setText(simpleDateFormat.format(time));
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
            Log.e("TimeBarView", "*******************  updateTime()  IllegalArgumentException!!");
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
//        Paint paint = new Paint();
//        paint.setStrokeWidth(3);
//        paint.setColor(Color.YELLOW);
//        paint.setAntiAlias(true);
//        //绘制中轴线
//        canvas.drawLine(getWidth() / 2, 0, getWidth() / 2, getHeight(), paint);

    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                lastX = event.getX();
                break;
            case MotionEvent.ACTION_MOVE:
                float x = event.getX();
                scrollBy((int) (lastX - x), 0);
                lastX = x;
                break;
        }
        return true;
    }
}

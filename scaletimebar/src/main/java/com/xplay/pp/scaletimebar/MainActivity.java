package com.xplay.pp.scaletimebar;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.xplay.pp.scaletimebar.timebar.ScaleTimeBar;
import com.xplay.pp.scaletimebar.timebar.SmallTime;
import com.xplay.pp.scaletimebar.timebar.TimeBarView;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;

public class MainActivity extends AppCompatActivity {


    private TextView main_tv_show;
    private ScaleTimeBar main_scaletimebar;
    SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    private TimeBarView main_timebar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        main_tv_show.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ViewGroup.LayoutParams layoutParams = main_timebar.getLayoutParams();

                layoutParams.width = layoutParams.width / 2;
                main_timebar.setLayoutParams(layoutParams);
            }
        });
    }

    private void initView() {
        main_timebar = findViewById(R.id.main_timebar);
        main_tv_show = findViewById(R.id.main_tv_show);
        main_scaletimebar = findViewById(R.id.main_scaletimebar);
        main_scaletimebar.setOnBarMoveListener(new ScaleTimeBar.OnBarMoveListener() {
            @SuppressLint("SimpleDateFormat")
            @Override
            public void onBarMove(long time) {
//                Log.e("TAG", "**************** onBarMove");
                main_tv_show.setText(simpleDateFormat.format(time));
            }

            @Override
            public void onBarMoveFinish(long time) {
//                Log.e("TAG", "**************** onBarMoveFinish");
                main_tv_show.setText(simpleDateFormat.format(time));
            }
        });

        Calendar instance = Calendar.getInstance();
        instance.set(Calendar.HOUR_OF_DAY, 0);
        instance.set(Calendar.MINUTE, 0);
        instance.set(Calendar.SECOND, 0);
        instance.set(Calendar.MILLISECOND, 0);
        long startTime = instance.getTimeInMillis();
        instance.set(Calendar.HOUR_OF_DAY, 23);
        instance.set(Calendar.MINUTE, 59);
        instance.set(Calendar.SECOND, 59);
        instance.set(Calendar.MILLISECOND, 999);
        long endTime = instance.getTimeInMillis();

        ArrayList<SmallTime> smallTimes = new ArrayList<>();
        SmallTime smallTime = null;
        long time_1_50 = (endTime - startTime)/ 100;
        for (int i = 0; i < 100; i++) {
            smallTime = new SmallTime();
            smallTime.setStartValue(startTime + time_1_50 * i);
            smallTime.setEndValue(startTime + time_1_50 * (i + 1));
            if (i % 5 == 0) {
                smallTime.setTimeColor(Color.RED);
            } else if (i % 4 == 0) {
                smallTime.setTimeColor(Color.BLUE);
            } else if (i % 3 == 0) {
                smallTime.setTimeColor(Color.GREEN);
            } else if (i % 2 == 0) {
                smallTime.setTimeColor(Color.YELLOW);
            } else {
                smallTime.setTimeColor(Color.LTGRAY);
            }
            smallTimes.add(smallTime);
        }
        main_scaletimebar.setSmallTimeList(smallTimes);
    }

}

package com.pp.client.xplay.videoviewer;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.pp.client.xplay.R;

/**
 * Created by qing on 19-1-8.
 */

public class PPPlay extends FrameLayout {

    private XPlay xPlay;

    public PPPlay(@NonNull Context context) {
        this(context, null);
    }

    public PPPlay(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public PPPlay(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setBackgroundResource(R.drawable.selector_play);
        LayoutInflater.from(context).inflate(R.layout.layout_ppplay, this);
        setUpXPlay();
    }

    private void setUpXPlay() {
        xPlay = (xPlay == null) ? XplayPool.obtain(getContext()) : xPlay;
        if (xPlay.getParent() != null) {
            ((ViewGroup) xPlay.getParent()).removeView(xPlay);
        }
        FrameLayout.LayoutParams params = new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        addView(xPlay, 0, params);
    }
}

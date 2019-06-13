package com.pp.client.xplay.videoviewer;

import android.content.Context;
import android.databinding.DataBindingUtil;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.pp.client.xplay.R;
import com.pp.client.xplay.databinding.LayoutPpplayBinding;
import com.pp.client.xplay.videoviewer.callback.VideoViewerHelper;

public class PPPlay extends FrameLayout implements VideoViewerHelper {

    private XPlay xPlay;
    private LayoutPpplayBinding mViewBing;
    private PPPlayViewModel mViewModel;

    public PPPlay(@NonNull Context context) {
        this(context, null);
    }

    public PPPlay(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public PPPlay(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setBackgroundResource(R.drawable.selector_play);
        mViewBing = DataBindingUtil.inflate(LayoutInflater.from(context), R.layout.layout_ppplay, this, true);
        mViewModel = new PPPlayViewModel(context, this);
        mViewBing.setViewModel(mViewModel);
        initView();
    }

    private void initView() {
        mViewBing.videoviewerProgressbar.setOnSeekBarChangeListener(mViewModel.onSeekBarChangeListener);
    }

    public PPPlayViewModel getViewModel() {
        return mViewModel;
    }

    private void releaseXPlay() {
        if (null != xPlay) {
            XplayPool.release(xPlay);
            removeView(xPlay);
            xPlay = null;
        }
    }

    private void setUpXPlay(XPlay.SurfaceHolderCallback callback) {
        xPlay = (xPlay == null) ? XplayPool.obtain(getContext()) : xPlay;
        Log.e("TAG", "********** XPlay: " + xPlay);
        xPlay.clear();
        if (xPlay.getParent() != null) {
            ((ViewGroup) xPlay.getParent()).removeView(xPlay);
        }
        xPlay.setSurfaceHolderCallback(callback);
        FrameLayout.LayoutParams params = new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        addView(xPlay, 0, params);
    }

    @Override
    public void startRender(XPlay.SurfaceHolderCallback callback) {
        setUpXPlay(callback);
    }

    @Override
    public void stopRender() {
        releaseXPlay();
    }
}

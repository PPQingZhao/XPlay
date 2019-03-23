package com.pp.client.xplay;

import android.Manifest;
import android.databinding.DataBindingUtil;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.pp.client.xplay.callback.UICallback;
import com.pp.client.xplay.databinding.ActivityMainBinding;
import com.pp.client.xplay.videoviewer.PPPlay;
import com.tbruyelle.rxpermissions2.Permission;
import com.tbruyelle.rxpermissions2.RxPermissions;

import io.reactivex.functions.Consumer;

public class MainActivity extends AppCompatActivity implements UICallback {

    private PPPlay ppPlay;
    private ActivityMainBinding mainBinding;
    private MainViewModel mViewModel;
    private PPPlay ppPlay2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mainBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        mViewModel = new MainViewModel(getBaseContext(), this);
        mainBinding.setViewModel(mViewModel);
        initView();
        initVideoViewer();
    }

    private void initVideoViewer() {
        ppPlay = new PPPlay(getBaseContext());
        if (ppPlay.getParent() != null) {
            ((ViewGroup) ppPlay.getParent()).removeView(ppPlay);
        }
        ppPlay.setSelected(true);
        mViewModel.addVideoModel(ppPlay.getViewModel());
        mainBinding.videoviewer.addView(ppPlay, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

        ppPlay2 = new PPPlay(getBaseContext());
        if (ppPlay2.getParent() != null) {
            ((ViewGroup) ppPlay2.getParent()).removeView(ppPlay);
        }
        ppPlay2.setSelected(true);
        mViewModel.addVideoModel(ppPlay2.getViewModel());
        mainBinding.videoviewer2.addView(ppPlay2, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
    }

    private void initView() {
    }


    private void startPlay() {
        if (mViewModel != null) {
            mViewModel.startPlay();
        }
    }

    private void requestPermissionList(String deniedPermission) {
        new RxPermissions(this).requestEachCombined(deniedPermission).subscribe(new Consumer<Permission>() {
            @Override
            public void accept(Permission permission) throws Exception {
                if (permission == null) {
                    Log.e("MainActivity", "Permission is NULL!");
                    return;
                }
                if (permission.granted) {  //全部权限授权成功
                    permissionGranted(permission);
                } else if (permission.shouldShowRequestPermissionRationale) { //至少有一个权限未授权 并且该权限未勾选不再询问
//                    showDialogWithoutNeverAgain();
                } else {    //至少有一个权限未授权 并且已勾选不再询问
//                    showDialogWithSetting();
                }
            }
        });
    }


    private void permissionGranted(Permission permission) {
        if (permission != null) {
            switch (permission.name) {
                case Manifest.permission.WRITE_EXTERNAL_STORAGE:
                    startPlay();
                    break;
            }
        }
    }

    @Override
    public void show() {

    }

    @Override
    public void dismiss() {

    }

    @Override
    public void requestPermission(String deniedPermission) {
        if (TextUtils.isEmpty(deniedPermission)) return;
        requestPermissionList(deniedPermission);
    }
}

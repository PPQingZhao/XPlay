package com.xplay.pp.ppcamera;

import android.databinding.DataBindingUtil;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.xplay.pp.ppcamera.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding mBinding;
    private MainViewModel mMainViewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        initView();
        initViewMode();
    }

    private void initViewMode() {
        mMainViewModel = new MainViewModel(getBaseContext());
        mBinding.setViewModel(mMainViewModel);
    }

    private void initView() {
    }
}

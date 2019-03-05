package com.pp.client.xplay.callback;

public interface UICallback {

    /*显示(dialog)*/
    void show();

    /*隐藏(dialog)*/
    void dismiss();

    /*申请权限*/
    void requestPermission(String deniedPermission);
}

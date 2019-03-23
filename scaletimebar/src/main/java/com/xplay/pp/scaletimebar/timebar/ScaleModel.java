package com.xplay.pp.scaletimebar.timebar;

import android.util.Log;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class ScaleModel {

    /*模式一：最小刻度1分钟,一个大刻度５分钟*/
    public static final int UNITVALUE_1_MIN = 0;
    /*模式二：最小刻度5分钟,一个大刻度10分钟*/
    public static final int UNITVALUE_5_MIN = 1;
    /*模式三：最小刻度10分钟,一个大刻度30分钟*/
    public static final int UNITVALUE_10_MIN = 2;
    /*模式四：最小刻度30分钟,一个大刻度1小时*/
    public static final int UNITVALUE_30_MIN = 3;
    /*模式五：最小刻度1小时,一个大刻度２小时*/
    public static final int UNITVALUE_1_HOUR = 4;
    private final HashMap<Integer, SizeParam> sizeParamMap = new HashMap<>();
    private final List<Scaler> scaleList = new ArrayList<>();
    private long sartValue;
    private long endValue;

    /*显示区域/可见区域*/
    private int disPlayWidth;
    private SizeParam currSizeParam;

    public ScaleModel() {
        SizeParam model = null;
        /*模式一：最小刻度1分钟,一个大刻度５分钟*/
        model = new SizeParam(5, 1, 60 * 1000, 10);
        sizeParamMap.put(UNITVALUE_1_MIN, model);
        /*模式二：最小刻度5分钟,一个大刻度10分钟*/
        model = new SizeParam(10, 5, 60 * 1000, 10);
        sizeParamMap.put(UNITVALUE_5_MIN, model);
        /*模式三：最小刻度10分钟,一个大刻度30分钟*/
        model = new SizeParam(30, 10, 60 * 1000, 10);
        sizeParamMap.put(UNITVALUE_10_MIN, model);
        /*模式四：最小刻度30分钟,一个大刻度1小时*/
        model = new SizeParam(60, 30, 60 * 1000, 10);
        sizeParamMap.put(UNITVALUE_30_MIN, model);
        /*模式五：最小刻度1小时,一个大刻度２小时*/
        model = new SizeParam(2 * 60, 60, 60 * 1000, 10);
        sizeParamMap.put(UNITVALUE_1_HOUR, model);

        //设置默认模式为　　模式二：最小刻度5分钟,一个大刻度10分钟
        currSizeParam = sizeParamMap.get(UNITVALUE_5_MIN);
    }

    public void setSizeParam(int unitModle) {
        SizeParam model = sizeParamMap.get(unitModle);
        if (null == model) {
            return;
        }
        currSizeParam = model;
        scaleList.clear();
        Scaler scaler = null;
        for (int i = 0; i < (getEndValue() - getSartValue()) / (model.getUnitValue() * model.getDecimal()) + 1/*补上一个刻度*/; i++) {
            scaler = new Scaler();
            //是否是关键刻度
            scaler.setKeyScaler(i % (model.getLargeValue() / model.getUnitValue()) == 0);
            //当前刻度位置(即第几个刻度)
            scaler.setPosition(i);
            scaleList.add(scaler);
           /* if (i % model.getUnitValue() == 0) {
                scaler = new Scaler();
                //是否是关键刻度
                scaler.setKeyScaler(i % model.getLargeValue() == 0);
                //当前刻度位置(即第几个刻度)
                scaler.setPosition(i / model.getUnitValue());
                scaleList.add(scaler);
            }*/
        }
        Log.e("TAG", "***************scaleList Size:" + scaleList.size());
    }

    public void changeSize(int scaleWidth) {
        //缩放后的宽度小于显示宽度,不做缩放处理
        if (getWidthBySizeParm(UNITVALUE_1_MIN) <= scaleWidth && scaleWidth < getWidthBySizeParm(UNITVALUE_5_MIN)) {
            setSizeParam(UNITVALUE_1_MIN);
        } else if (getWidthBySizeParm(UNITVALUE_5_MIN) <= scaleWidth && scaleWidth < getWidthBySizeParm(UNITVALUE_10_MIN)) {
            setSizeParam(UNITVALUE_5_MIN);
        } else if (getWidthBySizeParm(UNITVALUE_10_MIN) <= scaleWidth && scaleWidth < getWidthBySizeParm(UNITVALUE_30_MIN)) {
            setSizeParam(UNITVALUE_10_MIN);
        } else if (getWidthBySizeParm(UNITVALUE_30_MIN) <= scaleWidth && scaleWidth < getWidthBySizeParm(UNITVALUE_1_HOUR)) {
            setSizeParam(UNITVALUE_1_HOUR);
        }
    }

    public int getWidthBySizeParm(int mapKey) {
        return getWidthBySizeParm(sizeParamMap.get(mapKey));
    }

    public int getWidthBySizeParm(SizeParam sizeParam) {
        if (null == sizeParam) return 0;
        // 当前模式下有几个刻度(small)
        int scaleCount = (int) ((getEndValue() - getSartValue()) / (sizeParam.getDecimal() * sizeParam.getUnitValue()));
        return (int) (scaleCount * getPixelsPerScaler());
    }

    public int getScaleWith() {
        return getWidthBySizeParm(currSizeParam);
    }

    public float getPixelsPerScaler1() {
        //计算一个大刻度里面有多少个小刻度
        int smallInLargeCount = getLargeScale() / getUnitValue();
        //一个屏幕分布getDisPlayCount()个大刻度,则小刻度有 smallInLargeCount*getDisPlayCount() 个,计算每个小刻度对应的屏幕像素
        return disPlayWidth * 1.0f / (smallInLargeCount * getDisPlayCount());
    }

    public List<Scaler> getScaleList() {
        return scaleList;
    }

    public int getUnitValue() {
        return null == currSizeParam ? 0 : currSizeParam.getUnitValue();
    }

    public int getLargeScale() {
        return null == currSizeParam ? 0 : currSizeParam.getLargeValue();
    }

    public void setSartValue(long sartValue) {
        this.sartValue = sartValue;
    }

    public void setEndValue(long endValue) {
        if (null != currSizeParam) {
            this.endValue = endValue + currSizeParam.getUnitValue() * currSizeParam.getDecimal()/*补上１个刻度*/;
        }
    }

    public long getSartValue() {
        return sartValue;
    }

    public long getEndValue() {
        return endValue;
    }

    public int getDecimal() {
        return null == currSizeParam ? 0 : currSizeParam.getDecimal();
    }

    public long getScaleCount() {
        long scaleCount = 0;
        if (null != currSizeParam) {
            scaleCount = (getEndValue() - getSartValue()) / (currSizeParam.getDecimal() * currSizeParam.getUnitValue());
        }
        return scaleCount;
    }

    public void setDisPlayWidth(int disPlayWidth) {
        this.disPlayWidth = disPlayWidth;
    }

    public float getPixelsPerScaler() {
        SizeParam sizeParam = sizeParamMap.get(UNITVALUE_1_HOUR);
        return disPlayWidth * 1.0f / (sizeParam.getLargeValue() / sizeParam.getUnitValue() * getDisPlayCount());
    }

    public int getDisPlayCount() {
        return null == currSizeParam ? 0 : currSizeParam.getDisPlayLargeCount();
    }

    SimpleDateFormat simpleDateFormat = new SimpleDateFormat("HH:mm");

    //计算当前刻度代表的真实意义:   (当前刻度位置) * (一个刻度表示值) + (开始刻度值)
    public String getSubscrib(Scaler scaler) {
        if (null == scaler) return null;
        try {
            long scaleTimeMill = getSartValue() + scaler.getPosition() * getUnitValue() * getDecimal();
            return simpleDateFormat.format(scaleTimeMill);
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        }
        return null;
    }
}

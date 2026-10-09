package com.fyairo.hyspray;

import android.app.PendingIntent;
import android.widget.Toast;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.location.LocationManager;
import android.location.Criteria;
import android.provider.Settings;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationProvider;
import java.lang.ClassLoader;
import dalvik.system.DexClassLoader;
import java.lang.reflect.Field;
import android.os.Bundle;
import android.os.Environment;
import java.io.File;

import java.util.List;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import java.io.IOException;
import java.lang.Exception;
import java.lang.Throwable;
import android.net.DhcpInfo;
import android.content.Context;
import android.util.DisplayMetrics;   //屏幕像素密度

import android.view.inputmethod.InputMethodManager;

//import android.hardware.Camera;
//import android.view.SurfaceHolder;
//import android.view.SurfaceView;

//import android.bluetooth.BluetoothGattCharacteristic;
//import android.bluetooth.BluetoothGattService;

//import java.util.Iterator;
//import android.os.Looper;

public class ExtendsQtWithJava extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static ExtendsQtWithJava m_instance;
    public static WifiInfo currentWifiInfo;                //当前所连接的wifi
    public static List<ScanResult> wifiList;               // wifi列表
    public static List<WifiConfiguration> wifiConList;     // wifi 已成功连接过的配置列表
    public static int wifiIndex;            //从scanResult 得到的wifi列表进行记录位置
    public static  String[] str;
    public static  WifiManager conMan;
    public static DhcpInfo hostDhcpInfo;    //手机连接wifi 后得到的动态ip
    //private BluetoothLeService mBluetoothLeService;
    public static LocationManager locationManager;
    public static Location location;
    //public static Camera mCamera;
    //public static Toast toast ;

    public enum WifiCipherType {
        WIFICIPHER_WEP, WIFICIPHER_WPA, WIFICIPHER_NOPASS, WIFICIPHER_INVALID
    }

    public ExtendsQtWithJava(){
        m_instance = this;
    }

    public static void showToast(String text){
/*
       try {
            if(toast!=null){
                toast.setText("" + text + "aaa111");
            }else{
                toast= Toast.makeText(m_instance,"" + text + "asdfasdfasdfasd", Toast.LENGTH_SHORT);
            }
            toast.show();
        } catch (Exception e) {
                                            //解决在子线程中调用Toast的异常情况处理
            Looper.prepare();
            Toast.makeText(m_instance,  "" + text + "aaaaa5555" , Toast.LENGTH_SHORT).show();
            Looper.loop();
        }
*/
    }

    public static void setDomStorageEnabled(){
        //m_instance.webview.getSettings().setDomStorageEnabled(true);     //QtAndroidWebViewController.java 239: 加入 webSettings.setDomStorageEnabled(true); 编译qtwebview 复制QtAndroidWebView.jar到QT
        //WebSettings webSettings = webView.getSettings();
        //webSettings.setDomStorageEnabled(true);
    }


    public static void openCamera(){
        //mCamera=Camera.open(0);
        /*
        String mDeviceAddress;
        mBluetoothLeService.connect(mDeviceAddress);
        String msg = "1";
        byte[] WriteBytes = hex2byte(msg.getBytes());

        ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
                    new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
        int groupPosition = 0;
        int childPosition = 0;
        BluetoothGattCharacteristic characteristic =
                mGattCharacteristics.get(groupPosition).get(childPosition);
        characteristic.setValue(WriteBytes);
        mBluetoothLeService.writeCharacteristic(characteristic);
        */
    }


    public void enableGps() {
        final LocationManager manager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        if (!manager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
            //startActivity(new Intent(android.provider.Settings.ACTION_LOCATION_SOURCE_SETTINGS));
        }
    }

    public void showKeyboard(int isShow) {
         InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
         if (null == imm) return;

         if (isShow == 1) {
             if (getCurrentFocus() != null) {
                 imm.showSoftInput(getCurrentFocus(), 0);                 //有焦点打开
             } else {
                 imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);                 //无焦点打开
             }
         } else {
             if (getCurrentFocus() != null) {
                 imm.hideSoftInputFromWindow(getCurrentFocus().getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);             //有焦点关闭
             } else {
                 imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);             //无焦点关闭
             }
         }
     }

    public static void openWifi(){                                //实例化单例对象
        if(conMan == null)conMan = (WifiManager) m_instance.getSystemService(Context.WIFI_SERVICE);
        if(!conMan.isWifiEnabled()){
            conMan.setWifiEnabled(true);
        }
    }

    public static void closeWifi(){

        if(conMan.isWifiEnabled()){
            conMan.setWifiEnabled(false);
        }
    }

    public static void scanWifi(){
        if(conMan == null)conMan = (WifiManager) m_instance.getSystemService(Context.WIFI_SERVICE);
        Log.i("scanWifi","开始扫描...");
        conMan.startScan();
    }

    public static int getWifiCount(){              //get wifi info
        wifiList = conMan.getScanResults();
        Log.i("getWifiCount","" + wifiList.size());
        return wifiList.size();
    }
    public static String getWifiSSID(int index){
        if(index>=0&&index<wifiList.size()){
            return wifiList.get(index).SSID;
        }
        else{
            return "";
        }
    }

    public static int getWifiLevel(int index){
        if(index>=0 && index < wifiList.size()){
            return wifiList.get(index).level;
        }
        else{
            return 0;
        }
    }

    public static int getWifiFrequency(int index){
        if(index>=0 && index < wifiList.size()){
            return wifiList.get(index).frequency;
        }
        else{
            return 0;
        }
    }

    public static String getWifiBSSID(int index){
        if(index>=0 && index < wifiList.size()){
            return wifiList.get(index).BSSID;
        }
        else{
            return "";
        }
    }

    public static String getWifiKeyType(int index){            //加密方式判断
        if (wifiList.get(index).capabilities.contains("WEP")) {
            return "WEP";
        } else if (wifiList.get(index).capabilities.contains("PSK")) {
            return "PSK";
        } else if (wifiList.get(index).capabilities.contains("EAP")) {
            return "EAP";
        }
        return "无";
    }

    public static String getCurrentWifiSSID(){        //get current connected wifi info
        ConnectivityManager onnectivityManager = (ConnectivityManager) m_instance
                    .getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo wifi = onnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        if(!wifi.isConnected())return "";
        if(conMan == null)conMan = (WifiManager) m_instance.getSystemService(Context.WIFI_SERVICE);
        currentWifiInfo = conMan.getConnectionInfo();
        String s = currentWifiInfo.getSSID();
        //Log.i("getCurrentWifiSSID",s + " " + conMan);
        if (s == "<unknown ssid>") {
            NetworkInfo networkInfo = getNetworkInfo();
            if (networkInfo != null && networkInfo.isConnected()) {
                if (networkInfo.getExtraInfo() != null){
                    s = networkInfo.getExtraInfo().replace("\"","");
                }
            }
        }
        //Log.i("getCurrentWifiSSID",s + " " + conMan);
        return s;
    }

    public static NetworkInfo getNetworkInfo(){
        try{
            final ConnectivityManager connectivityManager = (ConnectivityManager)m_instance.getSystemService(Context.CONNECTIVITY_SERVICE);
            if (null != connectivityManager){
                return connectivityManager.getActiveNetworkInfo();
            }
        }catch(Exception e){
            e.printStackTrace();
        }
        return null;
    }

    public static String getCurrentWifiIPAddress(){    //get current ip address
        currentWifiInfo = conMan.getConnectionInfo();
        return "";        // WifiUtil.intToIp(currentWifiInfo.getIpAddress());
    }

    public static String getHostIPAddress(){
        hostDhcpInfo = conMan.getDhcpInfo();
        return "";//WifiUtil.intToIp(hostDhcpInfo.serverAddress);
    }

    public static int getCurrentNetworkId(){           //get current network id
        currentWifiInfo = conMan.getConnectionInfo();
        return currentWifiInfo.getNetworkId();
    }

    public static int networkState(){                 //get wifi connect state
        if(conMan == null)conMan = (WifiManager) m_instance.getSystemService(Context.WIFI_SERVICE);
        return conMan.isWifiEnabled()? 1 : 0;
    }

    public static void connectDevice(String ssid,String passwd){    //连接到新设备
//        WifiConfiguration wc = new WifiConfiguration();
//        wc.SSID = "\""+ssid+"\"";
//        wc.preSharedKey = "\""+passwd+"\"";

//        wc.hiddenSSID = true;
//        wc.status = WifiConfiguration.Status.ENABLED;
//        wc.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
//        wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
//        wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
//        wc.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
//        wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
//        wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
//        wc.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
    }

    public WifiConfiguration CreateWifiInfo(ScanResult scanresult,String Password)   //生成一个网络配置
    {
       WifiConfiguration wc = new WifiConfiguration();
       wc.SSID = "\""+scanresult.SSID+"\"";      //<span style="color: rgb(255, 0, 0); ">这个地方一定要注意了。旁边的“是不能够省略的。密码的地方也一样。</span>
       wc.preSharedKey = "\""+Password+"\"";      //该热点的密码
       wc.hiddenSSID = true;
       wc.status = WifiConfiguration.Status.ENABLED;
       wc.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
       wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
       wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
       wc.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
       wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
       wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
       wc.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
       return wc;
    }

    public WifiConfiguration CreateWifiInfoWithoutPasswd(ScanResult scanresult)    //无密码
    {
        WifiConfiguration wc = new WifiConfiguration();
        wc.SSID = "\""+scanresult.SSID+"\"";      //<span style="color: rgb(255, 0, 0); ">这个地方一定要注意了。旁边的“是不能够省略的。密码的地方也一样。</span>
                   //        wc.preSharedKey = "\""+Password+"\"";      //该热点的密码
        wc.hiddenSSID = true;
        wc.status = WifiConfiguration.Status.ENABLED;
        wc.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
        wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
        wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
        wc.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
        wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
        wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
        wc.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
        return wc;
     }

    public static int connectToWifi(int scanresultId,String Password){    //根据网络配置连接wifi
        int networkId = conMan.addNetwork(m_instance.CreateWifiInfo(wifiList.get(scanresultId),Password));
        if(networkId != -1){
            conMan.enableNetwork(networkId, false);
            conMan.saveConfiguration();
            return 1;//success
        }
        return 0;//falure
    }

    public static void connectToWifiWithoutPasswd(int scanresultId){    //无密码
        int networkId = conMan.addNetwork(m_instance.CreateWifiInfoWithoutPasswd(wifiList.get(scanresultId)));
        if(networkId != -1){
            conMan.enableNetwork(networkId, false);
            conMan.saveConfiguration();
            return ;//success
        }
        return ;//falure
    }

    public static double getDentisy(){    //获取屏幕像素密度
        DisplayMetrics metrics=new DisplayMetrics();
        m_instance.getWindowManager().getDefaultDisplay().getMetrics(metrics);
        return metrics.density;
    }

    private static WifiConfiguration CreateWifiInfo(String SSID, String Password, WifiCipherType Type){
         WifiConfiguration config = new WifiConfiguration();
         config.allowedAuthAlgorithms.clear();
         config.allowedGroupCiphers.clear();
         config.allowedKeyManagement.clear();
         config.allowedPairwiseCiphers.clear();
         config.allowedProtocols.clear();
         config.SSID = "\"" + SSID + "\"";

         if(Type == WifiCipherType.WIFICIPHER_NOPASS)
         {
             config.hiddenSSID = true;
             config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
             //config.wepKeys[0] = "";       //这两行去掉才能连接上
             //config.wepTxKeyIndex = 0;
         }

         if(Type == WifiCipherType.WIFICIPHER_WEP)
         {
             config.preSharedKey = "\""+Password+"\"";
             config.hiddenSSID = true;
             config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.SHARED);
             config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
             config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
             config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
             config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
             config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
             config.wepTxKeyIndex = 0;
         }

         if(Type == WifiCipherType.WIFICIPHER_WPA)
         {
             config.preSharedKey = "\""+Password+"\"";
             config.hiddenSSID = true;
             config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
             config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
             config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
             config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
             config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
             config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
             config.status = WifiConfiguration.Status.ENABLED;
         }

         return config;
    }

    public static String getwifi(String sid,String Password){
          if(conMan == null)conMan = (WifiManager) m_instance.getSystemService(Context.WIFI_SERVICE);
          int netId;
          WifiConfiguration config;
          if(Password == ""){
              config = CreateWifiInfo(sid, Password, WifiCipherType.WIFICIPHER_NOPASS);
              netId = conMan.addNetwork(config);
          }else{
              config = CreateWifiInfo(sid,Password,WifiCipherType.WIFICIPHER_WPA);
              netId = conMan.addNetwork(config);
          }
          conMan.disconnect();
          conMan.enableNetwork(netId, true);
          conMan.reconnect();
          return "" + netId + "," +conMan.getWifiState() ;
    }

    public static  String intToIp(int paramInt) {
        return (paramInt & 0xFF) + "." + (0xFF & paramInt >> 8) + "." + (0xFF & paramInt >> 16) + "."
                        + (0xFF & paramInt >> 24);
    }

    public static String getwifiip() {
        //conMan.getDhcpInfo().ipAddress;
        //conMan.getConnectionInfo().getIpAddress()
        return intToIp(conMan.getDhcpInfo().gateway);
    }


    public void removeWifi_1(String SSID){
        if(conMan == null)conMan = (WifiManager) m_instance.getSystemService(Context.WIFI_SERVICE);
        List<WifiConfiguration> conlist = conMan.getConfiguredNetworks();          //获取保存的配置信息
        for(int i =0; i< conlist.size(); i++){
               //Log.e(TAG,"i = " + String.valueOf(i) + "SSID = " + conlist.get(i).SSID + " netId = " + String.valueOf(conlist.get(i).networkId));
               //conMan.forget(conlist.get(i).networkId, null);                    //忘记所有wifi密码
           if(conlist.get(i).SSID == SSID){                                        //忘记当前wifi密码
               //conMan.forget(conlist.get(i).networkId, null);
           }
       }
    }

    public static String getgpslocation(){
        locationManager = (LocationManager) m_instance.getSystemService(Context.LOCATION_SERVICE);
        List<String> providers = locationManager.getProviders(true);
        for (String provider : providers) {
            Location l = locationManager.getLastKnownLocation(provider);
            if (l == null) {
                continue;
            }
            if (location == null || l.getAccuracy() < location.getAccuracy()) {
                // Found best last known location: %s", l);
                location = l;
                if (location != null){
                    double latt = location.getLatitude();
                    double lngg = location.getLongitude();
                    return "" + latt + "," + lngg;
                }
            }
        }

        return "0,0";
    }
}

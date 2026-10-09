QT += core gui quick network multimedia androidextras quickcontrols2 core-private positioning webview mqtt
CONFIG += c++11 mobility #qzxing_qml qzxing_multimedia
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
# include (QZXing/QZXing.pri)

#android: include(/home/song/android/android_openssl/openssl.pri)
QMAKE_CXXFLAGS_RELEASE += -O3

DEFINES += QT_DEPRECATED_WARNINGS

ANDROID_VERSION_CODE = "25"                             #安装高版本会覆盖低版本
ANDROID_VERSION_NAME = "1.3.1"               #版本号.小改.补丁
ANDROID_BUILD_DAY = "B-20230103"
#主版本号 . 子版本号 . 修正版本号 B- 编译日期，编译日期的格式为：YYYYMMDD
#1.2.0 B-202220622

DEFINES += ANDROID_VERSION_CODE=\\\"$$ANDROID_VERSION_CODE\\\"
DEFINES += ANDROID_VERSION_NAME=\\\"$$ANDROID_VERSION_NAME\\\"
DEFINES += ANDROID_BUILD_DAY=\\\"$$ANDROID_BUILD_DAY\\\"

#DEFINES += FyUdpDebug FyUdpDebugGBK                                 #使用udp调试信息

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    ../../android/libfy/camerafilter/camerafilter.cpp \              #camerafilter.so和qzxing有冲突
    ../../android/libfy/base/base.cpp \
    mainwork.cpp \
    wifimanager/wifimanager.cpp \
    ../../android/libfy/sqlite/easytablemodel.cpp \
    ../../android/libfy/network/tcpwork.cpp \
    ../../android/libfy/network/udpwork.cpp \
    ../../android/libfy/fjson/fjson.cpp \
    ../../android/libfy/gps/gps.cpp \
    ../../android/libfy/network/httpwork.cpp \
    ../../android/libfy/iotSDK/iotsdk.cpp \
    iotmessage.cpp \
    msgsocket.cpp \
    deviceman.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/fyairo/hyspray/ExtendsQtWithJava.java \
    android/src/com/fyairo/hyspray/WifiUtil.java

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android

    ANDROID_EXTRA_LIBS = \
        /home/song/code/HySpray/../../android/OpenCV-android-sdk-4.1.0/sdk/native/libs/armeabi-v7a/libopencv_java4.so \
        /home/song/code/HySpray/../../android/android-zbar-sdk/zbar/src/main/jniLibs/armeabi-v7a/libiconv.so \
        /home/song/code/HySpray/../../android/android-zbar-sdk/zbar/src/main/jniLibs/armeabi-v7a/libzbar.so \
        $$PWD/../../android/android_openssl/Qt-5.12.3/arm/libcrypto.so \
        $$PWD/../../android/android_openssl/Qt-5.12.3/arm/libssl.so
}

HEADERS += \
    ../../android/libfy/camerafilter/camerafilter.h \
    ../../android/libfy/base/base.h \
    mainwork.h \
    wifimanager/wifimanager.h \
    ../../android/libfy/sqlite/easytablemodel.h \
    ../../android/libfy/network/tcpwork.h \
    ../../android/libfy/network/udpwork.h \
    ../../android/libfy/fjson/fjson.h \
    ../../android/libfy/gps/gps.h \
    ../../android/libfy/network/httpwork.h \
    ../../android/libfy/iotSDK/iotsdk.h \
    iotmessage.h \
    msgsocket.h \
    fyspraydef.h \
    deviceman.h

ANDROID_OPENCV = /home/song/android/OpenCV-android-sdk-4.1.0/sdk/native
ANDROID_ROOT = /home/song/android

INCLUDEPATH += \
$$ANDROID_OPENCV/jni/include/opencv    \
$$ANDROID_OPENCV/jni/include/opencv2   \
$$ANDROID_OPENCV/jni/include           \
/home/song/android/android-zbar-sdk/zbar/src/main/jni/include    \
$$ANDROID_ROOT/libfy \
/home/song/android/openssl-1.1.1h/include

LIBS += \
$$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java4.so \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_contrib.a \             ##静态库有顺序
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_legacy.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_ml.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_objdetect.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_calib3d.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_video.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_videoio.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_features2d.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_highgui.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_androidcamera.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_flann.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_imgproc.a   \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_core.a      \
$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibjpeg-turbo.a \
#$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibpng.a \
#$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibtiff.a \
#$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibjasper.a \
#$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libtbb.a
/home/song/android/android-zbar-sdk/zbar/src/main/jniLibs/armeabi-v7a/libzbar.so \                   #和qzxing有冲突
/home/song/android/android-zbar-sdk/zbar/src/main/jniLibs/armeabi-v7a/libiconv.so

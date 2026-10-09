HySpray - 智能空间顶端雾化系统
基于 Qt/QML 开发的 Android 应用，用于控制智能喷雾主机及喷头，支持本地 WiFi 直连与云端远程管理。可通过扫描二维码快速添加设备，配置喷雾方案，实现定时/循环喷雾、喷头分组、演示控制及喷雾记录上传等功能。

功能特性
设备管理

扫描主机/喷头二维码，自动识别序列号

配置主机连接的工作 WiFi

读取主机下已注册的喷头信息

一键配喷头（批量设置喷头 ID）

喷雾方案

支持 3 套独立方案

喷雾模式：变风速 / 定风速

工作模式：定时 / 循环

工作日 / 每天选择

每套方案最多 3 个时间段，可设置启动时间、结束时间、喷洒时长、间隔时间

喷头分组：选择方案生效的喷头（1~16）

方案冲突检测，避免同一喷头在重叠时间段内执行多个方案

云端同步

用户登录 / 注册（权限分级：访客、普通用户、超级管理员）

从服务器下载设备列表及喷雾方案

提交主机信息、喷头信息及喷雾方案到云端

当前设备信息一键下载 / 上传

演示控制

重启主机

停机 / 复机

全部喷雾 / 清除喷雾

单喷头试喷

一键加水（控制水泵）

喷雾记录

从主机读取喷雾记录（通过 RS485 或 TCP）

本地保存为 JSON 文件

上传喷雾记录到服务器

系统设置

查看应用版本

用户登录状态保存

调试信息 UDP 输出（可选）

技术栈
框架：Qt 5.12.3 for Android

语言：C++11 / QML

UI：Qt Quick Controls 2、Qt Quick Layouts、Qt Quick Extras

网络：Qt Network (TCP/UDP/HTTP)、Qt MQTT、Qt WebView

多媒体：Qt Multimedia（二维码扫描音频）

传感器：Qt Sensors（方向传感器）

Android 集成：Qt Android Extras

第三方库：

OpenCV 4.1.0（图像处理）

ZBar（二维码解码）

OpenSSL 1.1.1（HTTPS 通信）

QtMqtt（MQTT 客户端）

QtWebView（内嵌网页）

构建说明
环境要求
Qt 5.12.3 for Android（含 Android armv7 工具链）

Android NDK（推荐 r19c 或与 Qt 5.12.3 匹配版本）

Android SDK

OpenCV Android SDK 4.1.0（路径：/home/song/android/OpenCV-android-sdk-4.1.0）

ZBar Android SDK（路径：/home/song/android/android-zbar-sdk）

OpenSSL for Android（路径：/home/song/android/android_openssl/Qt-5.12.3/arm）

配置与编译
克隆仓库到本地

使用 Qt Creator 打开 HySpray.pro

根据实际环境修改 .pro 文件中的第三方库路径：

ANDROID_OPENCV

ANDROID_ROOT

ANDROID_EXTRA_LIBS 中的 OpenSSL 库路径

选择 Android armv7 构建套件

执行 qmake 并构建

部署到 Android 设备（Android 5.0 及以上）

注意：项目依赖 assets:/jdhtj.ttf 字体文件，请确保该字体存在于资源中或 Android assets 目录。

目录结构
text
HySpray/
├── HySpray.pro                 # Qt 工程文件
├── main.cpp                    # 程序入口
├── main.qml                    # 主界面框架
├── mainwork.cpp/h              # 核心业务逻辑（设备、方案、网络）
├── deviceman.cpp/h             # 设备与方案数据模型
├── iotmessage.cpp/h            # 云端 API 通信
├── msgsocket.cpp/h             # UDP 调试消息
├── fyspraydef.h                # 常量定义
├── ImgSpray.qml                # 演示页喷头组件
├── ListSpray.qml               # 喷头列表（旧）
├── ListV.qml                   # 主机列表（旧）
├── MinsSelect.qml              # 分钟选择器
├── TimeSelect.qml              # 时间选择器
├── Page0Form.ui.qml            # 登录页
├── Page1Form.ui.qml            # 系统概况页
├── Page2Form.ui.qml            # 主机配置页
├── Page3Form.ui.qml            # 喷头设置页
├── Page4Form.ui.qml            # 喷雾方案页
├── Page5Form.ui.qml            # 演示管理页
├── Page6Form.ui.qml            # 用户信息页
├── ScanQRCodeView.qml          # 二维码扫描页
├── TableSpray.qml              # 喷头表格视图
├── TableV.qml                  # 主机表格视图
├── register.qml                # 注册页动画
├── qtquickcontrols2.conf       # Qt Quick Controls 2 样式配置
└── android/                    # Android 平台相关文件
    ├── AndroidManifest.xml
    ├── build.gradle
    ├── res/
    └── src/
主要模块说明
设备与方案模型 (deviceman.h/cpp)
PlanInfo：存储单套喷雾方案（ID、喷雾模式、工作模式、时间段、喷头组）

WorkingPeriod：单个时间段（启动、结束、喷洒、间隔）

parseSprayPlan()：解析云端下发的 JSON 方案

MainController：主机信息转换（云端 JSON → 本地存储 JSON）

核心业务 (mainwork.h/cpp)
管理设备列表（增删改查、本地持久化）

喷雾方案的构建、冲突检测、发送到主机

与主机 TCP 通信（端口 10086），指令队列发送与超时处理

与云端 IoT 平台通信（设备注册、状态查询、方案更新）

WiFi 连接管理、GPS 定位、地址解析

喷雾记录读取与上传

云端通信 (iotmessage.h/cpp)
基于 HTTPS 的 REST API 封装

用户登录、设备列表、设备信息、喷头信息、方案更新等接口

支持 Token 鉴权，自动重连与超时处理

QML 界面
采用 SwipeView + 底部导航栏，共 6 个主页面

使用 EasyTableModel 实现表格数据绑定

二维码扫描通过 CameraFilter 和 ZBar 实现

时间选择使用 Tumbler 组件

版本信息
当前版本：1.3.1

构建日期：B-20230103

Android 版本代码：25

许可证
本项目为内部项目，未指定开源许可证。如需使用请联系开发者。

联系方式
开发者：song

项目地址：内部 Git 仓库

捐赠
BTC: 13SongiriQuWoFhoimsVS21CyaTxozKBVA

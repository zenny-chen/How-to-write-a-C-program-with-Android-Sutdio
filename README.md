最近笔者用上了Android Studio 3.2，感觉这个版本对C语言的支持变得更友好了，在IDE层面对C语言错误的解析上也完全兼容了GNU11标准，可谓非常舒服。所以笔者也强烈推荐各位使用Windows系统的小伙伴们可以考虑Android Studio这款完全免费的工具来开发学习C语言！

大家可以在此处下载当前最新版的Android Studio：[https://developer.android.google.cn/studio/](https://developer.android.google.cn/studio/)。

各位放心，这个链接是Google专门针对中国开发者所设立的，因此我们无需开启任何VPN或其他服务器代理即可快速下载，而后续的插件也是能被快速下载，不需要做其他额外网络配置上的工作。

<br />

Android Studio默认采用的是Java编程语言，所以如果我们要做C语言开发的话需要额外下载一些插件，各位可以先打开Android Sutdio，然后在欢迎界面处点击“Configure”，再选择“SDK Manager”，如下图所示：

![1.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/1.png)

完成之后，Android Studio将会弹出偏好设置对话框，并且应该默认定格在“Android SDK”这个配置选项上。各位点击“SDK Tools”选项卡，然后这里需要勾选上“CMake”、“LLDB”以及“NDK”这三项。而x86模拟器加速器也建议选中。如下图所示：

![2.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/2.png)

点击OK之后，Android Studio就开始安装了。

安装完上述插件之后，我们就可以开始创建一个C语言项目工程了。由于一个Android项目默认使用Java，如果要用C语言的话需要通过JNI进行桥接。如果各位感觉麻烦的话，笔者这里提供了一种可以完全使用纯C语言的项目配置方法。
各位先在欢迎界面上点击“Start a new Android Studio project”。然后进入项目名配置界面，笔者这里就使用CTest。公司名随便填，但一般格式为xxx.com，或者xxx.yyy.com。当然，这里最最重要的是**必须要勾选上**“Include C++ Support”。如下图所示：

![3.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/3.png)

随后我们点击“Next”按钮，进入到SDK配置界面，这里我们使用默认配置即可。再点击“Next”，进入Activity配置界面，这里同样，我们使用默认的“Empty Activity”即可。再点击“Next”按钮，进入到Activity的创建界面。由于我们这里不需要Java端的代码，也不需要GUI相关的元素，因此我们可以取消勾选“Generate Layout File”，如下图所示：

![4.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/4.png)

最后再次点击“Next”按钮，配置C++选项。由于我们后面要使用的是C，不是C++，因此这里使用默认的工具链配置即可。此外，下面的“Exception Support”与“Runtime Type Information Support”都**不要勾选**上。点击“Finish”按钮之后完成所有配置。我们就进入了项目工程界面。此时，Android Studio将会自动编译构建整个项目，完成之后就会跳入Activity的Java代码源文件界面。

我们下面要做的事情是删除不需要的文件。我们先打开操作系统的文件管理器，进入CTest项目目录下的`app/src/`目录中，把整个“test”文件以及“androidTest”夹全都删除，如下图所示：

![5.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/5.png)

然后进入`app/src/main/`目录下，把整个`java`文件夹全都删除，如下图所示。

![6.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/6.png)

然后进入`app/src/main/cpp`目录，将“native-lib.cpp”改名为“native-lib.c”。

完成之后，我们回到Android Studio，此时Android Studio会自动刷新同步代码，我们会发现原本所包含的“native-lib.cpp”源文件不见了，我们不用捉急，先点开“manifests”文件夹中的“AndroidManifest.xml”文件，我们要对它进行编辑，如下图所示：

![7.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/7.png)

我们需要在`<application>`标签中添加`android:hasCode="false"`属性，表示我们不需要Java代码。然后替换掉整个`<activity>`标签内容。修改内容如下图所示：

![8.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/8.png)

下面列出完整的“AndroidManifest.xml”文件的代码：

```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.zenny_chen.ctest">

    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:roundIcon="@mipmap/ic_launcher_round"
        android:supportsRtl="true"
        android:theme="@style/AppTheme"
        android:hasCode="false">
        <!-- 我们不需要Java代码 -->

        <!-- Our activity is the built-in NativeActivity framework class.
             This will take care of integrating with our NDK code. -->
        <activity android:name="android.app.NativeActivity"
            android:label="@string/app_name"
            android:configChanges="orientation|keyboardHidden">
            <!-- Tell NativeActivity the name of our .so -->
            <meta-data android:name="android.app.lib_name"
                android:value="native-lib" />
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>

    </application>

</manifest>
```



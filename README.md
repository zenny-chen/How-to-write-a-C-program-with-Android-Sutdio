最近笔者用上了Android Studio 3.2，感觉这个版本对C语言的支持变得更友好了，在IDE层面对C语言错误的解析上也完全兼容了GNU11标准，可谓非常舒服。所以笔者也强烈推荐各位使用Windows系统的小伙伴们可以考虑Android Studio这款完全免费的工具来开发学习C语言！

大家可以在此处下载当前最新版的Android Studio：https://developer.android.google.cn/studio/。
各位放心，这个链接是Google专门针对中国开发者所设立的，因此我们无需开启任何VPN或其他服务器代理即可快速下载，而后续的插件也是能被快速下载，不需要做其他额外网络上的工作。

<br />

Android Studio默认采用的是Java编程语言，所以如果我们要做C语言开发的话需要额外下载一些插件，各位可以先打开Android Sutdio，然后在欢迎界面处点击“Configure”，再选择“SDK Manager”，如下图所示：

![1.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/1.png)

完成之后，Android Studio将会弹出偏好设置对话框，并且应该默认定格在“Android SDK”这个配置选项上。各位点击“SDK Tools”选项卡，然后这里需要勾选上“CMake”、“LLDB”以及“NDK”这三项。而x86模拟器加速器也建议选中。如下图所示：



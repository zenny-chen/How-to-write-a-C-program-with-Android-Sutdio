最近笔者用上了Android Studio 3.2，感觉这个版本对C语言的支持变得更友好了，在IDE层面对C语言错误的解析上也完全兼容了GNU11标准，可谓非常舒服。所以笔者也强烈推荐各位使用Windows系统的小伙伴们可以考虑Android Studio这款完全免费的工具来开发学习C语言！

大家可以在此处下载当前最新版的Android Studio：[https://developer.android.google.cn/studio/](https://developer.android.google.cn/studio/)。

各位放心，这个链接是Google专门针对中国开发者所设立的，因此我们无需开启任何VPN或其他服务器代理即可快速下载，而后续的插件也是能被快速下载，不需要做其他额外网络配置上的工作。

<br />

Android Studio默认采用的是Java编程语言，所以如果我们要做C语言开发的话需要额外下载一些插件，各位可以先打开Android Sutdio，然后在欢迎界面处点击“Configure”，再选择“SDK Manager”，如下图所示：

![1.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/1.png)

完成之后，Android Studio将会弹出偏好设置对话框，并且应该默认定格在“Android SDK”这个配置选项上。各位点击“SDK Tools”选项卡，然后这里需要勾选上“CMake”、“LLDB”以及“NDK”这三项。而x86模拟器加速器也建议选中。如下图所示：

![2.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/2.png)

点击OK之后，Android Studio就开始安装了。

<br />

安装完上述插件之后，我们就可以开始创建一个C语言项目工程了。由于一个Android项目默认使用Java，如果要用C语言的话需要通过JNI进行桥接。如果各位感觉麻烦的话，笔者这里提供了一种可以完全使用纯C语言的项目配置方法。
各位先在欢迎界面上点击“Start a new Android Studio project”。然后进入项目名配置界面，笔者这里就使用CTest。公司名随便填，但一般格式为xxx.com，或者xxx.yyy.com。当然，这里最最重要的是**必须要勾选上**“Include C++ Support”。如下图所示：

![3.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/3.png)

随后我们点击“Next”按钮，进入到SDK配置界面，这里我们使用默认配置即可。再点击“Next”，进入Activity配置界面，这里同样，我们使用默认的“Empty Activity”即可。再点击“Next”按钮，进入到Activity的创建界面。由于我们这里不需要Java端的代码，也不需要GUI相关的元素，因此我们可以取消勾选“Generate Layout File”，如下图所示：

![4.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/4.png)

最后再次点击“Next”按钮，配置C++选项。由于我们后面要使用的是C，不是C++，因此这里使用默认的工具链配置即可。此外，下面的“Exception Support”与“Runtime Type Information Support”都**不要勾选**上。点击“Finish”按钮之后完成所有配置。我们就进入了项目工程界面。此时，Android Studio将会自动编译构建整个项目，完成之后就会跳入Activity的Java代码源文件界面。

<br />

我们下面要做的事情是删除不需要的文件。我们先打开操作系统的文件管理器，进入CTest项目目录下的`app/src/`目录中，把整个“test”文件以及“androidTest”夹全都删除，如下图所示：

![5.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/5.png)

然后进入`app/src/main/`目录下，把整个`java`文件夹全都删除，如下图所示。

![6.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/6.png)

然后进入`app/src/main/cpp`目录，将“native-lib.cpp”改名为“native-lib.c”。

<br />

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

<br />

Manifest文件编辑完成后，我们接着就要开始编辑gradle文件了。我们找到“build.gradle (Module:app)”这个文件，然后点开，如下图所示：

![9.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/9.png)

各位注意，项目工程中有两个gradle文件，别点错了。在此gradle文件中，我们只编辑`android`块中的内容。改动的地方如下图所示：

![10.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/10.png)

在上图中，ndk块中的abiFilters参数指定了我们当前NDK C语言项目将针对哪些处理器架构生成目标代码。这里指定了三个：'x86_64'表示针对64位x86处理器，'armeabi-v7a'表示针对ARMv7-A架构处理器，'arm64-v8a'表示针对ARMv8-A架构处理器。所以对于本demo项目而言，最终生成的目标代码中不包含32位的x86架构，因此我们后面在设置模拟器的时候需要先下载64位的x86模拟器，后面会详细描述。

下面给出完整的gradle文件内容：

```gradle
apply plugin: 'com.android.application'

android {
    compileSdkVersion 28
    defaultConfig {
        applicationId "com.zenny_chen.ctest"
        minSdkVersion 17
        targetSdkVersion 28
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                // 对于ARMv7-A架构处理器的平台，使用NEON技术；对于所有ARM架构处理器的平台，使用ARM模式指令
                arguments "-DANDROID_ARM_NEON=TRUE", "-DANDROID_ARM_MODE=arm"

                // C语言使用GNU11标准，并以代码最小尺寸最快速度进行优化
                cFlags "-std=gnu11", "-Os"
            }
        }
        ndk {
            // Specifies the ABI configurations of your native
            // libraries Gradle should build and package with your APK.
            abiFilters 'x86_64', 'armeabi-v7a', 'arm64-v8a'
        }
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
    externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
        }
    }
}

dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar'])
    implementation 'com.android.support:appcompat-v7:28.0.0'
    testImplementation 'junit:junit:4.12'
    androidTestImplementation 'com.android.support.test:runner:1.0.2'
    androidTestImplementation 'com.android.support.test.espresso:espresso-core:3.0.2'
}
```

<br />

这个完成之后，我们最后再要编辑一下CMakeLists.txt文件。它在下图所示的位置：

![11.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/11.png)

由于Android Studio使用的是CMake工具，因此对源文件的编译配置以及动态库的生成都在这个文件中指定。该文件内容如下所示：

```cmake
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

include(AndroidNdkModules)
android_ndk_import_module_cpufeatures()
android_ndk_import_module_native_app_glue()

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.c)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib
        cpufeatures
        android
        native_app_glue

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
```

对于上述配置代码，对于初学者而言只需知道以下几点：
1. `add_library`下面的第一行用于指定当前编译构建项目最终所生成的库的名称。
1. `add_library`下面的第二行用于指定当前项目使用的是静态库还是动态库。由于我们整个项目最终要被Android系统的Java虚拟机调用，所以这里需要生成动态库，用`SHARED`指定。
1. `add_library`下面的第三行起就用于指定我们整个项目所要编译构建的源文件。这里可以填写.c源文件、.cpp源文件或.asm汇编源文件。多个源文件之间用**空格符**或**换行符**进行分隔。这里还需注意的是，源文件的根目录为`src/main/cpp/`，这个需要加上，当然，我们在此根目录下还可以再添加自己的文件夹。
1. 在`target_link_libraries`中需要把我们当前生成的库本身加进去，一般就放在第一个位置。

<br />

这些配置工作就全都完成之后，Android Studio会在右上角弹出“Sync”超链接样式的按钮，我们点击它，对整个项目重新构建。然后我们就能看到“native-lib.c”这个源文件了。它在下图所示的位置：

![12.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/12.png)

<br />

一般情况下，Android Studio默认使用的文件编码格式为UTF-8，但在Windows系统上可能默认设置为GBK，所以如果我们想要用跨平台通用的UTF-8文件编码格式的话可以在Android Studio的偏好设置中查看当前设置的文件编码格式是否为UTF-8。我们先找到设置文件编码格式的地方，如下图所示：

![13.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/13.png)

随后，我们在右侧可以看到当前配置的全局编码格式与项目编码格式，我们都将它们配置为UTF-8编码格式，如下图所示。

![14.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/14.png)

此外，在下方我们再检查一下“Create UTF-8 Files”是否为“with NO BOM”，我们所创建的UTF-8文件应该不带有BOM。

![15.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/15.png)

都设置完成后点击右下方的“OK”按钮即可。

<br />

下面我们就可以开始编辑native-lib.c源文件了。由于这是一个Android应用，因此它是没有传统上的`main`函数的，取而代之的是以`android_main`作为函数入口。此外，Android NDK中也去除了C语言的标准输入输出流，因为它不是一个控制台应用，所以如果我们需要动态输入数据的话只能通过读写文件进行。
这里简单介绍一下Android的文件系统资源管理体系。如果我们要在当前App中去读某个**只读**的资源文件的话，可以将该文件放在***assets***目录内，assets目录会被拷贝到当前App相关的目录中。由于Android Studio所生成的项目中没有默认生成assets文件夹，因此我们需要手工在当前项目工程的`app/src/main/`目录下创建assets目录，如下图所示：

![16.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/16.png)

随后，我们将需要被App所读的文件放入该assets文件夹里即可。Android NDK提供了一套接口用于访问assets中的文件资源：

```c 
AAsset *AAssetManager_open(AAssetManager *mgr, const char *filename, int mode)
```
这个函数用于打开指定文件名的资源文件。其中，参数mgr在`struct android_app`对象中即可获得。`mode`参数表示资源文件读取的模式，如果我们要一次读完当前文件的所有数据，那么使用`AASSET_MODE_BUFFER`即可。该函数返回一个AAsset对象引用，它相当于一个FILE句柄。

```c
off_t AAsset_getLength(AAsset *asset)
```
用于获取当前资源文件的长度。

```c
int AAsset_read(AAsset *asset, void *buf, size_t count)
```
该函数就用于读取资源文件。

```c
void AAsset_close(AAsset *asset)
```
在读完资源文件之后，我们用这个函数将它关闭。

如果我们要写文件，我们可以使用`app->activity->internalDataPath`所指定的路径。在该路径下的文件可以被用户创建、修改。因此我们可以用标准C的文件读写方式即可对它进行访问。

下面，我们给出本demo的所有native-lib.c源文件的内容：

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdatomic.h>

#include <cpu-features.h>
#include <syslog.h>

#include <android/hardware_buffer.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <android/rect.h>
#include <android/window.h>
#include <android_native_app_glue.h>

#ifndef var
#define var     __auto_type
#endif


/*
 * 对内联汇编代码进行测试的函数。对于x86-64架构计算除法运算；对于其他架构计算平方差。
 * @param a 对于x86-64架构作为被除数；对于其他架构作为被减数
 * @param b 对于x86-64架构作为除数；对于其他架构作为减数
 * @return 运算结果
 */
static int __attribute__((naked, pure)) InlineASMTest(int a, int b)
{
#ifdef __x86_64__
    // 以下代码针对x86-64架构，计算 a / b
    asm("pmuldq     %xmm1, %xmm0");
    asm("vpmuldq    %ymm1, %ymm0, %ymm0");
    asm("mov    %edi, %eax");
    asm("xor    %edx, %edx");
    asm("idiv   %esi");
    asm("ret");

#elif defined(__arm__)
    // 以下代码针对ARMv7以及更低版本的ARM架构
    asm("mul    r0, r0, r0");
    asm("mls    r0, r1, r1, r0");
    asm("bx     lr");

#elif defined(__aarch64__)
    // 以下代码针对ARM64处理器（比如ARMv8-A），计算a * a - b * b
    asm("mul    w0, w0, w0");
    asm("msub   w0, w1, w1, w0");
    asm("ret");

#endif
}


/*
 * 用于Android应用的消息响应处理的回调函数
 * @param app 表示当前应用的对象
 * @param cmd 表示当前接收到的命令
 */
static void Android_handle_cmd(struct android_app *app, int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
        {
            // The window is being shown, get it ready.
            syslog(LOG_INFO, "Activity awoken!!");

            break;
        }

        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            break;

        case APP_CMD_STOP:
            // Command from main thread: the app's activity has been stopped.
            syslog(LOG_INFO, "Activity stopped!!");

            break;

        case APP_CMD_DESTROY:

            syslog(LOG_INFO, "The app is destroyed!!");
            break;

        default:
            syslog(LOG_INFO, "event not handled: %d", cmd);
            break;
    }
}

/*
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* app)
{
    // TODO: 我们可以在这里写测试程序
    syslog(LOG_INFO, "Entering android main function!");

    // 根据文件名打开资源文件
    AAsset* asset = AAssetManager_open(app->activity->assetManager, "test.info", AASSET_MODE_BUFFER);
    if(asset == NULL)
    {
        syslog(LOG_EMERG, "test.info not found!");
        return;
    }
    // 获取文件长度
    const size_t length = (size_t)AAsset_getLength(asset);

    // 分配存放文件内容的缓存
    char *buffer = malloc(length);

    // 读取文件内容
    AAsset_read(asset, buffer, length);

    // 关闭文件句柄
    AAsset_close(asset);

    syslog(LOG_INFO, "The file content is: %s", buffer);

    char filePath[256];
    strcpy(filePath, app->activity->internalDataPath);
    syslog(LOG_INFO, "File path: %s", filePath);
    strcat(filePath, "/output.txt");

    // 准备将缓存内容写入设备的SD卡内
    FILE *fp = fopen(filePath, "w");
    if(fp != NULL)
    {
        fwrite(buffer, 1, length, fp);
        fclose(fp);
    }
    else
        syslog(LOG_EMERG, "file cannot be opened!");

    free(buffer);

    // 汇编函数测试
#ifdef __x86_64__
    int remainder;
    const int quotient = InlineASMTest(10, 7);
    asm("mov    %%edx, %0" :"=r"(remainder));
    syslog(LOG_INFO, u8"除法结果商为：%d，余数为：%d", quotient, remainder);
#else
    const int result = InlineASMTest(5, 4);
    syslog(LOG_INFO, u8"平方差的结果为：%d", result);
#endif

    // 对原子操作进行测试
    atomic_flag flag = ATOMIC_FLAG_INIT;
    if(!atomic_flag_test_and_set(&flag))
        syslog(LOG_INFO, "flag is set!");
    else
        syslog(LOG_INFO, "flag is locked!");

    if(!atomic_flag_test_and_set(&flag))
        syslog(LOG_INFO, "flag is set!");
    else
        syslog(LOG_INFO, "flag is locked!");

    atomic_flag_clear(&flag);

    if(!atomic_flag_test_and_set(&flag))
        syslog(LOG_INFO, "flag is set!");
    else
        syslog(LOG_INFO, "flag is locked!");

    atomic_int atomInt;
    atomic_init(&atomInt, 10);
    var value = atomic_fetch_add(&atomInt, 5);
    const var desired = atomic_load(&atomInt);
    syslog(LOG_INFO, "original value = %d, added value = %d", value, desired);

    if(!atomic_compare_exchange_strong(&atomInt, &value, desired))
        syslog(LOG_INFO, "CAS failed!");
    else
        syslog(LOG_INFO, "CAS succeeded!");

    if(!atomic_compare_exchange_strong(&atomInt, &value, desired))
        syslog(LOG_INFO, "CAS failed!");
    else
        syslog(LOG_INFO, "CAS succeeded!");

    // 对C11的泛型进行测试
    var type = _Generic("abc", const char*:1, char*:2, const char[4]:3, char[4]:4, default:0);
    syslog(LOG_INFO, "type = %d", type);

    // 系统消息循环处理
    app->onAppCmd = Android_handle_cmd;

    // Main loop
    do {
        int events;
        struct android_poll_source *source;

        // Poll all pending events.
        if(ALooper_pollAll(0, NULL, &events, (void**)&source) >= 0)
        {
            // Process each polled events
            if(source != NULL)
                source->process(app, source);
        }
    }
    // Check if system requested to quit the application
    while(app->destroyRequested == 0);
}
```

对于在Android Studio中写汇编，笔者建议使用C语言的内联汇编形式比较靠谱。因为单独的汇编源文件无法设置断点，因此不易于调试。

<br />

编辑完之后，我们点击工具栏的绿色箭头即可运行此代码，如下图所示：

![17.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/17.png)

或者使用底部**Run**标签里的绿色箭头，如下图所示：

![18.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/18.png)

<br />

在点击运行按钮之后会弹出对话框，要求选择用哪种设备运行当前App。如果我们要用模拟器运行的话可以创建一台虚拟设备。点击“Create New Virtual Device”按钮，进入到下面的对话框。

![19.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/19.png)

这里要求我们选择一台设备的型号屏幕尺寸。笔者建议，如果各位当前屏幕分辨率不太高的话选择分辨率低一点的虚拟设备，如果高的话可以考虑分辨率高的设备。我们选完之后点击“Next”按钮。

![20.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/20.png)

这里展现的是选择设备镜像。由于本demo使用的是x86-64架构，因此我们必须选择x86_64的镜像文件。此外，我们最好再要选择当前Android Studio所支持的最高版本的Android系统。如果当前镜像需要下载，我们点击“Download”下载即可。

<br />

再点击“Next”按钮之后就是对当前虚拟设备的命名配置，我们起好一个名之后点击“Finish”按钮即可完成。回到之前的对话框，选择我们刚建立好的虚拟设备即可开始运行程序了。我们在IDE底部的Run标签栏中可以看到输出log信息。

![21.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/21.png)

如果我们用Android设备进行真机调试，Run标签栏中可能无法打印出相关信息，此时我们可以观察Logcat中的信息，如下图所示：

![22.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/22.png)

<br />

由于Logcat设计得比较“灵活”，因此默认情况下它针对的是之前所选择的设备做log捕获，因此我们在连接好新的设备之后，需要选择左上角红框框出来的选择框，然后选择当前调试的设备。然后，在我们点击运行按钮之后（注意，必须让设备开始运行当前App），在过滤器下拉框中选择“Show only selected application”，这样能让当前设备输出打印的不需要的内容大幅减少。如下图所示：

![23.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/23.png)

<br />

我们在本demo中有写文件功能。那么我们如何查看所写文件的结果呢？
无论是在Android模拟器中还是对于真机设备，如果我们要查看模拟器设备里所写入的文件内容，那么我们可以利用Android Studio自带的Device File Explorer工具。在版本较老的Android Studio中，Device File Explorer在工具栏上，可见这篇博文：https://blog.csdn.net/zhaoyanjun6/article/details/72284543

而在较新的版本中它藏在整个IDE工具的右下角，如下所示：

![24.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/24.png)

这界面设计得非常诡异……然后我们选择需要查看的设备，再根据打印输出的路径找到我们所写的文件。然后鼠标右键点击它，再选择“Save As”另存为到我们自己桌面系统上的某个路径下即可，如下图所示：

![25.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/25.png)

<br />

下面我们将谈谈Android Studio对程序的调试。跟其他主流IDE一样，Android Studio也允许我们使用断点调试。不过目前能支持断点的只有Java（包括Kotlin）、C和C++源文件，汇编源文件不支持断点调试。设置断点的方法也跟其他主流IDE一样，在代码编辑框左侧显示行号的灰色区域鼠标左键点击一下即可。当程序执行到断点这一行后将会暂停，此时我们可以观察各个局部变量以及全局变量的值。不过这里需要注意的是，使用Android Studio调试程序时不能点击运行按钮，而是必须点击**调试按钮**，它位于下图所示的工具栏：

![26.png](https://github.com/zenny-chen/How-to-write-a-C-program-with-Android-Sutdio/blob/master/26.png)

<br />


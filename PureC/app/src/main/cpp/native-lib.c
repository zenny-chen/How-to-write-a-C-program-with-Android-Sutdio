#include "zf_sys/zf_sys.h"
#include <string.h>
#include <math.h>
#include <stdatomic.h>

#include <cpu-features.h>

#include <android/hardware_buffer.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <android/rect.h>
#include <android/window.h>
#include <android_native_app_glue.h>


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
    puts("Entering android main function!");

    // 根据文件名打开资源文件
    AAsset* asset = AAssetManager_open(app->activity->assetManager, "test.info", AASSET_MODE_BUFFER);
    if(asset == NULL)
    {
        puts("test.info not found!");
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

    printf("The file content is: %s\n", buffer);

    char filePath[256];
    strcpy(filePath, app->activity->internalDataPath);
    printf("File path: %s\n", filePath);
    strcat(filePath, "/output.txt");

    // 准备将缓存内容写入设备的SD卡内
    FILE *fp = fopen(filePath, "w");
    if(fp != NULL)
    {
        fwrite(buffer, 1, length, fp);
        fclose(fp);
    }
    else
        puts("file cannot be opened!");

    free(buffer);

    // 汇编函数测试
#ifdef __x86_64__
    int remainder;
    const int quotient = InlineASMTest(10, 7);
    asm("mov    %%edx, %0" :"=r"(remainder));
    printf(u8"除法结果商为：%d，余数为：%d\n", quotient, remainder);
#else
    const int result = InlineASMTest(5, 4);
    printf(u8"平方差的结果为：%d\n", result);
#endif

    // 对原子操作进行测试
    atomic_flag flag = ATOMIC_FLAG_INIT;
    if(!atomic_flag_test_and_set(&flag))
        puts("flag is set!");
    else
        puts("flag is locked!");

    if(!atomic_flag_test_and_set(&flag))
        puts("flag is set!");
    else
        puts("flag is locked!");

    atomic_flag_clear(&flag);

    if(!atomic_flag_test_and_set(&flag))
        puts("flag is set!");
    else
        puts("flag is locked!");

    atomic_int atomInt;
    atomic_init(&atomInt, 10);
    let value = atomic_fetch_add(&atomInt, 5);
    const let desired = atomic_load(&atomInt);
    printf("original value = %d, added value = %d\n", value, desired);

    if(!atomic_compare_exchange_strong(&atomInt, &value, desired))
        puts("CAS failed!");
    else
        puts("CAS succeeded!");

    if(!atomic_compare_exchange_strong(&atomInt, &value, desired))
        puts("CAS failed!");
    else
        puts("CAS succeeded!");

    // 对C11的泛型进行测试
    let type = _Generic("abc", const char*:1, char*:2, const char[4]:3, char[4]:4, default:0);
    printf("type = %d", type);

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


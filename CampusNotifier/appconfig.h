#ifndef APPCONFIG_H
#define APPCONFIG_H

// 应用程序类型枚举
enum class AppType {
    Pusher,      // 推送端，负责发布、修改、删除通知
    Receiver     // 接收端，只负责接收和查看通知
};

// 当前应用程序类型，默认为推送端
// 可以通过编译选项或配置文件修改
// 例如：在.pro文件中添加 DEFINES += APP_TYPE=Receiver
#ifndef APP_TYPE
#define APP_TYPE AppType::Pusher
#endif

class AppConfig {
public:
    // 获取当前应用程序类型
    static AppType getAppType() {
        return APP_TYPE;
    }
    
    // 判断是否为推送端
    static bool isPusher() {
        return getAppType() == AppType::Pusher;
    }
    
    // 判断是否为接收端
    static bool isReceiver() {
        return getAppType() == AppType::Receiver;
    }
};

#endif // APPCONFIG_H
#ifndef GLOBALDATA_H
#define GLOBALDATA_H
#include<QObject>

class GlobalData : public QObject
{
    Q_OBJECT
public:
    // 获取单例实例的静态方法
    static GlobalData* instance()
    {
        static GlobalData data; // 静态实例，保证线程安全（C++11之后）
        return &data;
    }

    // 共享变量的访问接口
    QString getSharedString() const { return m_globalAdress; }
    void setSharedString(const QString& str) { m_globalAdress = str; }

    int getSharedValue() const { return m_sharedValue; }
    void setSharedValue(int value) { m_sharedValue = value; }

private:
    // 构造函数私有化，禁止外部创建实例
    explicit GlobalData(QObject *parent = nullptr) : QObject(parent) {}
    ~GlobalData() = default;

    // 禁止拷贝和赋值
    GlobalData(const GlobalData&) = delete;
    GlobalData& operator=(const GlobalData&) = delete;

private:
    // 共享变量
    QString m_globalAdress="http://110.42.231.40:5146";
    int m_sharedValue = 0;
};

#endif // GLOBALDATA_H

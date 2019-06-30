#ifndef KEEPASSX_MEMORYENCRYPTION_H
#define KEEPASSX_MEMORYENCRYPTION_H

#include <QString>

class MemoryEncryption
{
public:
    MemoryEncryption();
    ~MemoryEncryption() = default;

    QString encrypt(const QString& value, bool *ok) const;
    QString decrypt(const QString& value, bool *ok) const;

    static MemoryEncryption *instance();

private:
    MemoryEncryption(const MemoryEncryption&) = delete;
    MemoryEncryption& operator=(const MemoryEncryption&) = delete;

    QByteArray m_preKey;

    static MemoryEncryption *m_instance;
};

#endif

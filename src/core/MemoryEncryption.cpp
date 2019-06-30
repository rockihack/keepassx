#include "MemoryEncryption.h"

#include "crypto/CryptoHash.h"
#include "crypto/Random.h"
#include "crypto/SymmetricCipher.h"

#define MEMORY_ENCRYPTION_PREKEY_SIZE (16 * 4096)
#define MEMORY_ENCRYPTION_IV_SIZE 8

MemoryEncryption *MemoryEncryption::m_instance(nullptr);

MemoryEncryption *MemoryEncryption::instance()
{
    if (!m_instance) {
        m_instance = new MemoryEncryption();
    }

    return m_instance;
}

MemoryEncryption::MemoryEncryption()
    : m_preKey(Random::instance()->randomArray(MEMORY_ENCRYPTION_PREKEY_SIZE))
{
}

QString MemoryEncryption::encrypt(const QString& value, bool *ok) const
{
    SymmetricCipher cipher(SymmetricCipher::Salsa20, SymmetricCipher::Stream, SymmetricCipher::Encrypt);
    QByteArray key = CryptoHash::hash(m_preKey, CryptoHash::Sha256);
    QByteArray iv = Random::instance()->randomArray(MEMORY_ENCRYPTION_IV_SIZE);
    QByteArray ciphertext = value.toUtf8();

    if (!cipher.init(key, iv) || !cipher.processInPlace(ciphertext)) {
        *ok = false;
        return QString();
    }

    *ok = true;
    return QString::fromLatin1(iv.append(ciphertext).toBase64());
}

QString MemoryEncryption::decrypt(const QString& protectedValue, bool *ok) const
{
    SymmetricCipher cipher(SymmetricCipher::Salsa20, SymmetricCipher::Stream, SymmetricCipher::Decrypt);
    QByteArray value = QByteArray::fromBase64(protectedValue.toLatin1());
    QByteArray key = CryptoHash::hash(m_preKey, CryptoHash::Sha256);
    QByteArray iv = value.left(MEMORY_ENCRYPTION_IV_SIZE);
    QByteArray plaintext = value.mid(MEMORY_ENCRYPTION_IV_SIZE);

    if (!cipher.init(key, iv) || !cipher.processInPlace(plaintext)) {
        *ok = false;
        return QString();
    }

    *ok = true;
    return QString::fromUtf8(plaintext);
}

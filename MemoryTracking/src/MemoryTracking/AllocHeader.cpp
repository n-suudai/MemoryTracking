#include "AllocHeader.h"


namespace MemoryTracking
{
    // ヘッダ情報の全体サイズ
    size_t AllocHeader::s_HeaderSize = 0;

    // ヘッダ情報メンバアドレスへのオフセット値
    std::array<size_t, AllocHeader::HEADER_INFO_NUM> AllocHeader::s_HeaderInfoOffsets = { 0 };

    // 有効なヘッダ情報
    HeaderInfoFlags AllocHeader::s_HeaderInfos = HeaderInfoFlagBits::Required;


    // どのヘッダ情報を保持するかを渡して初期化
    bool AllocHeader::Initialize(HeaderInfoFlags headerInfos)
    {
        if ((headerInfos & HeaderInfoFlagBits::Required) != HeaderInfoFlagBits::Required)
        {
            return false;
        }

        s_HeaderSize = 0;
        s_HeaderInfoOffsets.fill(0);
        s_HeaderInfos = headerInfos;

        // アドレス
        if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBlock)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBlock)] = s_HeaderSize;
            s_HeaderSize += sizeof(void*);
        }

        // 確保サイズ
        if (s_HeaderInfos & HeaderInfoFlagBits::MemoryBytes)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::MemoryBytes)] = s_HeaderSize;
            s_HeaderSize += sizeof(size_t);
        }

        // ファイル名
        if (s_HeaderInfos & HeaderInfoFlagBits::FileName)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FileName)] = s_HeaderSize;
            s_HeaderSize += sizeof(const char*);
        }

        // 行数
        if (s_HeaderInfos & HeaderInfoFlagBits::Line)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Line)] = s_HeaderSize;
            s_HeaderSize += sizeof(std::uint32_t);
        }

        // 関数名
        if (s_HeaderInfos & HeaderInfoFlagBits::FunctionName)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::FunctionName)] = s_HeaderSize;
            s_HeaderSize += sizeof(const char*);
        }

        // 確保日時
        if (s_HeaderInfos & HeaderInfoFlagBits::DateTime)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::DateTime)] = s_HeaderSize;
            s_HeaderSize += sizeof(time_t);
        }

        // バックトレースのハッシュ値
        if (s_HeaderInfos & HeaderInfoFlagBits::BackTraceHash)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::BackTraceHash)] = s_HeaderSize;
            s_HeaderSize += sizeof(size_t);
        }

        // メモリ破壊検出用シグネチャ
        if (s_HeaderInfos & HeaderInfoFlagBits::Signature)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Signature)] = s_HeaderSize;
            s_HeaderSize += sizeof(Signature*);
        }

        // ブックマーク
        if (s_HeaderInfos & HeaderInfoFlagBits::Bookmark)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Bookmark)] = s_HeaderSize;
            s_HeaderSize += sizeof(size_t);
        }

        // 親ヒープ領域
        if (s_HeaderInfos & HeaderInfoFlagBits::Heap)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Heap)] = s_HeaderSize;
            s_HeaderSize += sizeof(Heap*);
        }

        // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
        if (s_HeaderInfos & HeaderInfoFlagBits::Next)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Next)] = s_HeaderSize;
            s_HeaderSize += sizeof(AllocHeader*);
        }

        // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
        if (s_HeaderInfos & HeaderInfoFlagBits::Prev)
        {
            s_HeaderInfoOffsets[static_cast<int>(HeaderInfoIndex::Prev)] = s_HeaderSize;
            s_HeaderSize += sizeof(AllocHeader*);
        }

        return true;
    }

    // 終了
    void AllocHeader::Terminate()
    {
        s_HeaderSize = 0;
        s_HeaderInfoOffsets.fill(0);
        s_HeaderInfos = HeaderInfoFlagBits::Required;
    }

    // 有効化されているか
    bool AllocHeader::IsEnabled(HeaderInfoFlags headerInfos)
    {
        return (bool)(s_HeaderInfos & headerInfos);
    }

    // オフセット取得
    size_t AllocHeader::GetHeaderOffset(HeaderInfoIndex headerInfoIndex)
    {
        return s_HeaderInfoOffsets[static_cast<int>(headerInfoIndex)];
    }

    // ヘッダ情報の全体サイズを取得
    size_t AllocHeader::GetHeaderSize()
    {
        return s_HeaderSize;
    }

    // アドレス
    const void* AllocHeader::GetBlock() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::MemoryBlock);
        return ptr;
    }

    void* AllocHeader::GetBlock()
    {
        void* ptr = this + GetHeaderOffset(HeaderInfoIndex::MemoryBlock);
        return ptr;
    }

    // 確保サイズ
    size_t AllocHeader::GetBytes() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::MemoryBytes);
        return *reinterpret_cast<const size_t*>(ptr);
    }

    // ファイル名
    const char* AllocHeader::GetFileName() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::FileName);
        return reinterpret_cast<const char*>(ptr);
    }

    // 行数
    std::uint32_t AllocHeader::GetLine() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Line);
        return *reinterpret_cast<const std::uint32_t*>(ptr);
    }

    // 関数名
    const char* AllocHeader::GetFunctionName() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::FunctionName);
        return reinterpret_cast<const char*>(ptr);
    }

    // 確保日時
    time_t AllocHeader::GetDateTime() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::DateTime);
        return *reinterpret_cast<const time_t*>(ptr);
    }

    // バックトレースのハッシュ値
    size_t AllocHeader::GetBackTraceHash() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::BackTraceHash);
        return *reinterpret_cast<const size_t*>(ptr);
    }

    // メモリ破壊検出用シグネチャ
    AllocHeader::Signature AllocHeader::GetSignature() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Signature);
        return *reinterpret_cast<const Signature*>(ptr);
    }

    // ブックマーク
    size_t AllocHeader::GetBookmark() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Bookmark);
        return *reinterpret_cast<const size_t*>(ptr);
    }

    // 親ヒープ領域
    const Heap* AllocHeader::GetHeap() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Heap);
        return reinterpret_cast<const Heap*>(ptr);
    }

    Heap* AllocHeader::GetHeap()
    {
        void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Heap);
        return reinterpret_cast<Heap*>(ptr);
    }

    // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
    const AllocHeader* AllocHeader::GetNext() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Next);
        return reinterpret_cast<const AllocHeader*>(ptr);
    }

    AllocHeader* AllocHeader::GetNext()
    {
        void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Next);
        return reinterpret_cast<AllocHeader*>(ptr);
    }

    // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
    const AllocHeader* AllocHeader::GetPrev() const
    {
        const void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Prev);
        return reinterpret_cast<const AllocHeader*>(ptr);
    }

    AllocHeader* AllocHeader::GetPrev()
    {
        void* ptr = this + GetHeaderOffset(HeaderInfoIndex::Prev);
        return reinterpret_cast<AllocHeader*>(ptr);
    }
}


#pragma once


#include <cstdint>
#include <array>
#include "Flags.h"


namespace MemoryTracking
{
    class Heap;

    enum class HeaderInfoIndex : std::uint16_t
    {
          MemoryBlock       // アドレス
        , MemoryBytes       // 確保サイズ
        , FileName          // ファイル名
        , Line              // 行数
        , FunctionName      // 関数名
        , DateTime          // 確保日時
        , BackTraceHash     // バックトレースのハッシュ値
        , Signature         // メモリ破壊検出用シグネチャ
        , Bookmark          // ブックマーク
        , Heap              // 親ヒープ領域
        , Next              // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
        , Prev              // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
        , Num
    };

    enum class HeaderInfoFlagBits
    {
          MemoryBlock   = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::MemoryBlock))   // アドレス
        , MemoryBytes   = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::MemoryBytes))   // 確保サイズ
        , FileName      = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::FileName))      // ファイル名
        , Line          = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::Line))          // 行数
        , FunctionName  = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::FunctionName))  // 関数名
        , DateTime      = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::DateTime))      // 確保日時
        , BackTraceHash = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::BackTraceHash)) // バックトレースのハッシュ値
        , Signature     = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::Signature))     // メモリ破壊検出用シグネチャ
        , Bookmark      = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::Bookmark))      // ブックマーク
        , Heap          = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::Heap))          // 親ヒープ領域
        , Next          = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::Next))          // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
        , Prev          = (1 << static_cast<std::uint16_t>(HeaderInfoIndex::Prev))          // 前のヘッダへのポインタ (ヒープをウォークするのに必要)

        // 必須
        , Required = MemoryBlock
            | MemoryBytes
            | Heap
            | Next
            | Prev

        // すべて
        , All = MemoryBlock
            | MemoryBytes
            | FileName
            | Line
            | FunctionName
            | DateTime
            | BackTraceHash
            | Signature
            | Bookmark
            | Heap
            | Next
            | Prev
    };

    typedef Flags<HeaderInfoFlagBits> HeaderInfoFlags;


    // ヘッダ情報
    class AllocHeader
    {
    public:
        typedef std::uint32_t Signature;
        static constexpr Signature SIGNATURE = 0xCDCDCDCD;                          // シグネチャ定義
        static constexpr size_t HEADER_INFO_NUM = sizeof(HeaderInfoFlagBits) * 8;   // ヘッダ情報の最大メンバ数

    public:
        // どのヘッダ情報を保持するかを渡して初期化
        static bool Initialize(HeaderInfoFlags headerInfos = HeaderInfoFlagBits::Required);

        // 終了
        static void Terminate();

        // 有効化されているか
        static bool IsEnabled(HeaderInfoFlags headerInfos);

        // オフセット取得
        static size_t GetHeaderOffset(HeaderInfoIndex headerInfoIndex);

        // ヘッダ情報の全体サイズを取得
        static size_t GetHeaderSize();

        // アドレス
        const void* GetBlock() const;
        void* GetBlock();

        // 確保サイズ
        size_t GetBytes() const;

        // ファイル名
        const char* GetFileName() const;

        // 行数
        std::uint32_t GetLine() const;

        // 関数名
        const char* GetFunctionName() const;

        // 確保日時
        time_t GetDateTime() const;

        // バックトレースのハッシュ値
        size_t GetBackTraceHash() const;

        // メモリ破壊検出用シグネチャ
        Signature GetSignature() const;

        // ブックマーク
        size_t GetBookmark() const;

        // 親ヒープ領域
        const Heap* GetHeap() const;
        Heap* GetHeap();

        // 次のヘッダへのポインタ (ヒープをウォークするのに必要)
        const AllocHeader* GetNext() const;
        AllocHeader* GetNext();

        // 前のヘッダへのポインタ (ヒープをウォークするのに必要)
        const AllocHeader* GetPrev() const;
        AllocHeader* GetPrev();

    private:
        static size_t s_HeaderSize; // ヘッダ情報の全体サイズ
        static std::array<size_t, HEADER_INFO_NUM> s_HeaderInfoOffsets; // ヘッダ情報メンバアドレスへのオフセット値
        static HeaderInfoFlags s_HeaderInfos; // 有効なヘッダ情報
    };
}


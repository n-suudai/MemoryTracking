#pragma once

#include <cstdint>


template <typename BitType, typename MaskType = std::uint64_t>
class Flags
{
public:
    constexpr Flags()
        : m_mask(0)
    {}

    constexpr Flags(BitType bit)
        : m_mask(static_cast<MaskType>(bit))
    {}

    constexpr Flags(Flags<BitType> const& rhs)
        : m_mask(rhs.m_mask)
    {}

    constexpr explicit Flags(MaskType flags)
        : m_mask(flags)
    {}

    Flags<BitType>& operator=(Flags<BitType> const& rhs)
    {
        m_mask = rhs.m_mask;
        return *this;
    }

    Flags<BitType>& operator|=(Flags<BitType> const& rhs)
    {
        m_mask |= rhs.m_mask;
        return *this;
    }

    Flags<BitType>& operator&=(Flags<BitType> const& rhs)
    {
        m_mask &= rhs.m_mask;
        return *this;
    }

    Flags<BitType>& operator^=(Flags<BitType> const& rhs)
    {
        m_mask ^= rhs.m_mask;
        return *this;
    }

    constexpr Flags<BitType> operator|(Flags<BitType> const& rhs) const
    {
        return Flags<BitType>(m_mask | rhs.m_mask);
    }

    constexpr Flags<BitType> operator&(Flags<BitType> const& rhs) const
    {
        return Flags<BitType>(m_mask & rhs.m_mask);
    }

    constexpr Flags<BitType> operator^(Flags<BitType> const& rhs) const
    {
        return Flags<BitType>(m_mask ^ rhs.m_mask);
    }

    constexpr bool operator!() const
    {
        return !m_mask;
    }

    constexpr Flags<BitType> operator~() const
    {
        return Flags<BitType>(m_mask ^ FlagTraits<BitType>::allFlags);
    }

    constexpr bool operator==(Flags<BitType> const& rhs) const
    {
        return m_mask == rhs.m_mask;
    }

    constexpr bool operator!=(Flags<BitType> const& rhs) const
    {
        return m_mask != rhs.m_mask;
    }

    explicit constexpr operator bool() const
    {
        return !!m_mask;
    }

    explicit constexpr operator MaskType() const
    {
        return m_mask;
    }

private:
    MaskType  m_mask;
};

template <typename BitType>
constexpr Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
{
    return flags | bit;
}

template <typename BitType>
constexpr Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
{
    return flags & bit;
}

template <typename BitType>
constexpr Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
{
    return flags ^ bit;
}

template <typename BitType>
constexpr bool operator==(BitType bit, Flags<BitType> const& flags)
{
    return flags == bit;
}

template <typename BitType>
constexpr bool operator!=(BitType bit, Flags<BitType> const& flags)
{
    return flags != bit;
}


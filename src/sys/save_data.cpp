#include "sys/save_data.h"

#include "ibn_sram_rw.h"

#include <type_traits>

namespace bq::sys
{

namespace
{

constexpr bn::string_view SAVE_MAGIC = "BNQST";

constexpr unsigned SAVE_LOCATION_0 = 0;
constexpr unsigned SAVE_LOCATION_1 = bn::sram::size() / 2;

constexpr auto LANG_MIN = static_cast<ldtk::gen::lang>(0);
constexpr auto LANG_MAX = static_cast<ldtk::gen::lang>(31);
static_assert(static_cast<ldtk::gen::lang>(
                  static_cast<std::underlying_type_t<ldtk::gen::lang>>(ldtk::gen::lang::max_count) - 1) <= LANG_MAX);

constexpr std::uint32_t FOOTER = 0x5A7EF001; // SAVE FOOT

} // namespace

save_data::save_data()
{
    reset();
}

void save_data::reset()
{
    _lang = ldtk::gen::lang::eng;
}

bool save_data::load()
{
    ibn::sram_rw rw(SAVE_MAGIC, SAVE_LOCATION_0, SAVE_LOCATION_1);

    const bool loaded = rw.read(*this);

    // If read fails, it might be halfway-loaded (inconsistent state),
    // so we reset again.
    if (!loaded)
        reset();

    return loaded;
}

void save_data::save()
{
    ibn::sram_rw rw(SAVE_MAGIC, SAVE_LOCATION_0, SAVE_LOCATION_1);
    rw.write(*this);
}

auto save_data::language() const -> ldtk::gen::lang
{
    return _lang;
}

void save_data::set_language(ldtk::gen::lang lang_)
{
    _lang = lang_;
}

void save_data::set_next_language()
{
    _lang = static_cast<ldtk::gen::lang>(((int)_lang + 1) % (int)ldtk::gen::lang::max_count);
}

void save_data::measure(ibn::bit_stream_measurer& measurer) const
{
    measurer
        .write(_lang, LANG_MIN, LANG_MAX) // [0..31]: 5 bits
        .write(FOOTER);                   // footer: 32 bits
}

void save_data::write(ibn::bit_stream_writer& writer) const
{
    writer
        .write(_lang, LANG_MIN, LANG_MAX) // [0..31]: 5 bits
        .write(FOOTER);                   // footer: 32 bits
}

void save_data::read(ibn::bit_stream_reader& reader)
{
    std::uint32_t footer = 0;

    reader
        .read(_lang, LANG_MIN, LANG_MAX) // [0..31]: 5 bits
        .read(footer);                   // footer: 32 bits

    if (footer != FOOTER)
        reader.set_fail();
}

} // namespace bq::sys

#if defined(LOCALIZE)

#include "debug.h"
#include "filesystem.h"
#include "string_formatter.h"
#include "translation_document.h"

InvalidTranslationDocumentException::InvalidTranslationDocumentException(
    const std::string_view path,
    const std::string_view message ) : std::runtime_error(
            string_format( "Invalid MO document %1$s: %2$s", path, message ) )
{
}

std::uint8_t TranslationDocument::GetByte( const std::size_t byteIndex ) const
{
    return static_cast<std::uint8_t>( data[byteIndex] );
}

std::uint32_t TranslationDocument::GetUint32BE( const std::size_t byteIndex ) const
{
    std::uint32_t a = GetByte( byteIndex );
    std::uint32_t b = GetByte( byteIndex + 1 );
    std::uint32_t c = GetByte( byteIndex + 2 );
    std::uint32_t d = GetByte( byteIndex + 3 );
    return ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
}

std::uint32_t TranslationDocument::GetUint32LE( const std::size_t byteIndex ) const
{
    std::uint32_t a = GetByte( byteIndex + 3 );
    std::uint32_t b = GetByte( byteIndex + 2 );
    std::uint32_t c = GetByte( byteIndex + 1 );
    std::uint32_t d = GetByte( byteIndex );
    return ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
}

std::uint32_t TranslationDocument::GetUint32( const std::size_t byteIndex ) const
{
    return ( this->*GetUint32FPtr )( byteIndex );
}

TranslationDocument::TranslationDocument( const std::string &path )
{
    try {
        const uintmax_t file_size = fs::file_size( fs::u8path( path ) );
        if( file_size < 20 ) {
            throw InvalidTranslationDocumentException( path, "file too small" );
        }
        mmap_message_object = mmap_file::map_file( path );
        if( !mmap_message_object ) {
            throw InvalidTranslationDocumentException( path, "unable to read the file" );
        }
        if( mmap_message_object->len != file_size ) {
            throw InvalidTranslationDocumentException( path, "unable to read the file in its entirety" );
        }
    } catch( const fs::filesystem_error &e ) {
        throw InvalidTranslationDocumentException( path, e.what() );
    }

    data = reinterpret_cast<const char *>( mmap_message_object->base );
    data_size = mmap_message_object->len;

    if( GetByte( 0 ) == 0x95U &&
        GetByte( 1 ) == 0x04U &&
        GetByte( 2 ) == 0x12U &&
        GetByte( 3 ) == 0xdeU ) {
        endianness = Endianness::BigEndian;
        GetUint32FPtr = &TranslationDocument::GetUint32BE;
    } else if( GetByte( 0 ) == 0xdeU &&
               GetByte( 1 ) == 0x12U &&
               GetByte( 2 ) == 0x04U &&
               GetByte( 3 ) == 0x95U ) {
        endianness = Endianness::LittleEndian;
        GetUint32FPtr = &TranslationDocument::GetUint32LE;
    } else {
        throw InvalidTranslationDocumentException( path, "does not contain MO magic number" );
    }
    const std::uint32_t version = GetUint32( 4 );
    constexpr std::uint32_t supported_version = 0;
    if( version != supported_version ) {
        throw InvalidTranslationDocumentException( path,
                string_format( "unsupported MO format revision %zu, expect %zu", version, supported_version ) );
    }
    number_of_strings = GetUint32( 8 );
    if( number_of_strings < 1 ) {
        throw InvalidTranslationDocumentException( path,
                string_format( "Too few messages contained in MO" ) );
    }
    original_strings_table_offset = GetUint32( 12 );
    translated_strings_table_offset = GetUint32( 16 );
    if( original_strings_table_offset + 8ULL * number_of_strings > data_size ) {
        throw InvalidTranslationDocumentException( path,
                string_format( "original strings table offset %zu with %zu entries exceeds buffer size %zu",
                               original_strings_table_offset, number_of_strings, data_size ) );
    }
    if( translated_strings_table_offset + 8ULL * number_of_strings > data_size ) {
        throw InvalidTranslationDocumentException( path,
                string_format( "translated strings table offset %zu with %zu entries exceeds buffer size %zu",
                               translated_strings_table_offset, number_of_strings, data_size ) );
    }
    for( std::size_t i = 0; i < number_of_strings; i++ ) {
        const std::size_t length = GetUint32( original_strings_table_offset + 8 * i );
        const std::size_t offset = GetUint32( original_strings_table_offset + 8 * i + 4 );
        if( offset >= data_size || length >= data_size || offset + length >= data_size ) {
            throw InvalidTranslationDocumentException( path,
                    string_format( "original string %zu offset %zu with length %zu exceeds buffer size %zu",
                                   i, offset, length, data_size ) );
        }
        if( data[offset + length] != '\0' ) {
            throw InvalidTranslationDocumentException( path,
                    string_format( "original string %zu offset %zu with length %zu not terminated by '\\0'",
                                   i, offset, length ) );
        }
    }
    for( std::size_t i = 0; i < number_of_strings; i++ ) {
        const std::size_t length = GetUint32( translated_strings_table_offset + 8 * i );
        const std::size_t offset = GetUint32( translated_strings_table_offset + 8 * i + 4 );
        if( offset >= data_size || length >= data_size || offset + length >= data_size ) {
            throw InvalidTranslationDocumentException( path,
                    string_format( "translated string %zu offset %zu with length %zu exceeds buffer size %zu",
                                   i, offset, length, data_size ) );
        }
        if( data[offset + length] != '\0' ) {
            throw InvalidTranslationDocumentException( path,
                    string_format( "translated string %zu offset %zu with length %zu not terminated by '\\0'",
                                   i, offset, length ) );
        }
    }
}

std::size_t TranslationDocument::Count() const
{
    return number_of_strings;
}

std::pair<const char *, std::size_t> TranslationDocument::GetSourceString(
    const std::size_t index ) const
{
    if( index > number_of_strings ) {
        throw std::out_of_range{"TranslationDocument::GetSourceString"};
    }
    const std::size_t length = GetUint32( original_strings_table_offset + 8 * index );
    const std::size_t offset = GetUint32( original_strings_table_offset + 8 * index + 4 );
    return std::make_pair( &data[offset], length );
}

std::pair<const char *, std::size_t> TranslationDocument::GetTranslatedString(
    const std::size_t index ) const
{
    if( index > number_of_strings ) {
        throw std::out_of_range{"TranslationDocument::GetTranslatedString"};
    }
    const std::size_t length = GetUint32( translated_strings_table_offset + 8 * index );
    const std::size_t offset = GetUint32( translated_strings_table_offset + 8 * index + 4 );
    return std::make_pair( &data[offset], length );
}

#endif // defined(LOCALIZE)

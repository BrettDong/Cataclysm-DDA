#include "debug.h"
#include "filesystem.h"
#include "string_formatter.h"
#include "translation_document.h"

InvalidTranslationDocumentException::InvalidTranslationDocumentException( const std::string &path )
{
    this->path = path;
    this->error_message = string_format( "Invalid translation document: %s", path );
}

const char *InvalidTranslationDocumentException::GetPath() const noexcept
{
    return path.c_str();
}

const char *InvalidTranslationDocumentException::what() const noexcept
{
    return error_message.c_str();
}

std::uint8_t TranslationDocument::GetByte( const std::size_t byteIndex ) const
{
    return static_cast<std::uint8_t>( data.at( byteIndex ) );
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

const char *TranslationDocument::GetString( const std::size_t byteIndex ) const
{
    return data.c_str() + byteIndex;
}

std::size_t TranslationDocument::EvaluatePluralForm( std::size_t n ) const
{
    // TODO: evaluate based on plural_rules
    return n != 1;
}

TranslationDocument::TranslationDocument( const std::string &path )
{
    DebugLog( D_INFO, DC_ALL ) << "[i18n] Loading MO file: " << path;
    this->path = path;
    cata::ifstream fin( fs::u8path( path ) );
    if( !fin ) {
        throw std::runtime_error( string_format( "Cannot load %s", path ) );
    }
    data = std::string( ( std::istreambuf_iterator<char>( fin ) ), std::istreambuf_iterator<char>() );
    if( data.size() < 4 ) {
        throw InvalidTranslationDocumentException( path );
    }
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
        throw InvalidTranslationDocumentException( path );
    }
    number_of_strings = GetUint32( 8 );
    original_strings_table_offset = GetUint32( 12 );
    translated_strings_table_offset = GetUint32( 16 );
    if( original_strings_table_offset >= data.size() ) {
        throw InvalidTranslationDocumentException( path );
    }
    if( translated_strings_table_offset >= data.size() ) {
        throw InvalidTranslationDocumentException( path );
    }
    for( std::size_t i = 0; i < number_of_strings; i++ ) {
        std::size_t length = GetUint32( original_strings_table_offset + 8 * i );
        std::size_t offset = GetUint32( original_strings_table_offset + 8 * i + 4 );
        if( offset + length >= data.size() ) {
            throw InvalidTranslationDocumentException( path );
        }
        if( data[offset + length] != '\0' ) {
            throw InvalidTranslationDocumentException( path );
        }
        original_offsets.emplace_back( offset );
    }
    for( std::size_t i = 0; i < number_of_strings; i++ ) {
        std::vector<std::size_t> offsets;
        std::size_t length = GetUint32( translated_strings_table_offset + 8 * i );
        std::size_t offset = GetUint32( translated_strings_table_offset + 8 * i + 4 );
        if( offset + length >= data.size() ) {
            throw InvalidTranslationDocumentException( path );
        }
        if( data[offset + length] != '\0' ) {
            throw InvalidTranslationDocumentException( path );
        }
        offsets.emplace_back( offset );
        for( std::size_t idx = offset; idx + 1 < offset + length; idx++ ) {
            if( data[idx] == '\0' ) {
                offsets.emplace_back( idx + 1 );
            }
        }
        translated_offsets.emplace_back( offsets );
    }
}

std::size_t TranslationDocument::Count() const
{
    return number_of_strings;
}

const char *TranslationDocument::GetOriginalString( const std::size_t index ) const
{
    return GetString( original_offsets[index] );
}

const char *TranslationDocument::GetTranslatedString( const std::size_t index ) const
{
    return GetString( translated_offsets[index][0] );
}

const char *TranslationDocument::GetTranslatedStringPlural( const std::size_t index,
        std::size_t n ) const
{
    return GetString( translated_offsets[index][EvaluatePluralForm( n )] );
}

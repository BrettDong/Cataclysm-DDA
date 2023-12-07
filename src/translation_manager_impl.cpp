#if defined(LOCALIZE)

#include <cstring>

#include "cached_options.h"
#include "debug.h"
#include "filesystem.h"
#include "path_info.h"
#include "translations.h"
#include "translation_manager_impl.h"

TranslationManager::Impl::IndexEntryHeader::IndexEntryHeader( std::size_t ProbeSequenceLength,
        std::size_t PartialHash, const std::array<std::size_t, 3> &PluralOffsets )
{
    RawValue = ( ProbeSequenceLength << 56 ) | ( PluralOffsets[2] << 44 ) |
               ( PluralOffsets[1] << 32 ) | ( PluralOffsets[0] << 20 ) | ( PartialHash & 0xFFFFFU );
}

std::size_t TranslationManager::Impl::IndexEntryHeader::GetProbeSequenceLength() const noexcept
{
    return RawValue >> 56;
}

std::size_t TranslationManager::Impl::IndexEntryHeader::GetPluralOffset(
    std::size_t pluralIndex ) const noexcept
{
    if( 1 <= pluralIndex && pluralIndex <= 3 ) {
        return RawValue >> ( 12 * pluralIndex + 8 ) & 0xFFFU;
    } else {
        return 0;
    }
}

std::size_t TranslationManager::Impl::IndexEntryHeader::GetPartialHash() const noexcept
{
    return RawValue & 0xFFFFFU;
}

TranslationManager::Impl::IndexEntry::IndexEntry( std::size_t ProbeSequenceLength,
        std::size_t PartialHash, std::array<std::size_t, 3> PluralOffsets, const char *pSourceString,
        const char *pTranslatedString ) : Header( ProbeSequenceLength, PartialHash, PluralOffsets ),
    pSourceString( pSourceString ), pTranslatedString( pTranslatedString ) {}

bool TranslationManager::Impl::IndexEntry::IsEmpty() const noexcept
{
    return pSourceString == nullptr;
}

std::uint64_t TranslationManager::Impl::Hash( const char *str ) noexcept
{
    std::uint64_t hash = 5381UL;
    while( *str != '\0' ) {
        hash = hash * 33UL + ( *str++ );
    }
    return hash;
}

std::size_t TranslationManager::Impl::MaskPartialHash( const std::uint64_t hash ) noexcept
{
    return ( hash >> 16 ) & 0xFFFFFU;
}

std::size_t TranslationManager::Impl::MaskBucketSlot( const std::uint64_t hash ) const noexcept
{
    return hash & index_capacity_mask;
}

void TranslationManager::Impl::InsertIndex( const char *source, const char *translated,
        std::array<std::size_t, 3> pluralOffsets )
{
    const std::uint64_t hash = Hash( source );
    for( std::size_t psb = 0, bucket = MaskBucketSlot( hash ), partialHash = MaskPartialHash( hash );
         psb <= MaxProbeSequenceLength; bucket = ( bucket + 1 ) & index_capacity_mask, psb++ ) {
        if( index[bucket].IsEmpty() || strcmp( index[bucket].pSourceString, source ) == 0 ) {
            index[bucket] = IndexEntry{ psb, partialHash, pluralOffsets, source, translated };
            return;
        }
        if( index[bucket].Header.GetProbeSequenceLength() > psb ) {
            const IndexEntryHeader headerAtBucket = index[bucket].Header;
            const char *pSourceAtBucket = index[bucket].pSourceString;
            const char *pTranslatedAtBucket = index[bucket].pTranslatedString;
            index[bucket] = IndexEntry{ psb, partialHash, pluralOffsets, source, translated };
            psb = headerAtBucket.GetProbeSequenceLength();
            partialHash = headerAtBucket.GetPartialHash();
            for( std::size_t pluralIndex = 1; pluralIndex <= 3; pluralIndex++ ) {
                pluralOffsets[pluralIndex - 1] = headerAtBucket.GetPluralOffset( pluralIndex );
            }
            source = pSourceAtBucket;
            translated = pTranslatedAtBucket;
        }
    }
    throw std::runtime_error{"cannot find available bucket within allowed maximum probe sequence length"};
}

void TranslationManager::Impl::InsertMessage( const char *source, const char *translated,
        std::size_t translatedLength )
{
    std::vector<std::size_t> pluralOffsets;
    for( std::size_t i = 0; i + 1 < translatedLength; i++ ) {
        if( translated[i] == '\0' ) {
            pluralOffsets.push_back( ++i );
        }
    }
    std::array<std::size_t, 3> pluralOffsetsArray{0, 0, 0};
    for( std::size_t pluralIndex = 0; pluralIndex <= 2; pluralIndex++ ) {
        if( pluralOffsets.size() > pluralIndex && pluralOffsets[pluralIndex] <= 0xFFFU ) {
            pluralOffsetsArray[pluralIndex] = pluralOffsets[pluralIndex];
        }
    }
    InsertIndex( source, translated, pluralOffsetsArray );
    if( !pluralOffsets.empty() ) {
        plural_offset_map.emplace( translated, std::move( pluralOffsets ) );
    }
}

void TranslationManager::Impl::LoadPluralRulesEvaluator( const std::string_view metadata )
{
    constexpr std::string_view plural_rules_header{"Plural-Forms:"};
    std::size_t plural_rules_header_pos = metadata.find( plural_rules_header );
    if( plural_rules_header_pos != std::string::npos ) {
        plural_rules_header_pos += plural_rules_header.length();
        const std::size_t new_line_pos = metadata.find( '\n', plural_rules_header_pos );
        std::string plural_rules_str;
        if( new_line_pos != std::string::npos ) {
            plural_rules_str = metadata.substr( plural_rules_header_pos,
                                                new_line_pos - plural_rules_header_pos );
        } else {
            plural_rules_str = metadata.substr( plural_rules_header_pos );
        }
        plural_rules_evaluator = std::make_unique<TranslationPluralRulesEvaluator>( plural_rules_str );
    } else {
        throw std::runtime_error{"cannot find plural rules in MO file"};
    }
}

auto TranslationManager::Impl::Lookup( const char *query ) const ->
std::pair<IndexEntryHeader, const char *>
{
    if( index_capacity_mask == 0 ) {
        return {IndexEntryHeader{}, nullptr};
    }
    const std::uint64_t hash = Hash( query );
    const std::size_t partialHash = MaskPartialHash( hash );
    for( std::size_t bucket = MaskBucketSlot( hash ), psb = 0; psb <= MaxProbeSequenceLength;
         bucket = ( bucket + 1 ) & index_capacity_mask, psb++ ) {
        if( index[bucket].IsEmpty() ) {
            return {IndexEntryHeader{}, nullptr};
        }
        if( index[bucket].Header.GetProbeSequenceLength() > psb ) {
            return {IndexEntryHeader{}, nullptr};
        }
        if( index[bucket].Header.GetPartialHash() != partialHash ) {
            continue;
        }
        if( strcmp( query, index[bucket].pSourceString ) == 0 ) {
            return {index[bucket].Header, index[bucket].pTranslatedString};
        }
    }
    return {IndexEntryHeader{}, nullptr};
}

std::string TranslationManager::Impl::LanguageCodeOfPath( const std::string_view path )
{
    const std::size_t end = path.rfind( "/LC_MESSAGES" );
    if( end == std::string::npos ) {
        return std::string();
    }
    const std::size_t begin = path.rfind( '/', end - 1 ) + 1;
    if( begin == std::string::npos ) {
        return std::string();
    }
    return std::string( path.substr( begin, end - begin ) );
}

void TranslationManager::Impl::ScanTranslationDocuments()
{
    if( dir_exist( locale_dir() ) ) {
        DebugLog( D_INFO, DC_ALL ) << "[i18n] Scanning core translations from " << locale_dir();
        for( const std::string &dir : get_files_from_path( "LC_MESSAGES", locale_dir(), true ) ) {
            std::vector<std::string> mo_files = get_files_from_path( "cataclysm-dda.mo", dir, false, false );
            if( !mo_files.empty() ) {
                core_mo_files[LanguageCodeOfPath( mo_files.front() )] = mo_files.front();
            }
        }
    }
    if( dir_exist( PATH_INFO::user_moddir() ) ) {
        DebugLog( D_INFO, DC_ALL ) << "[i18n] Scanning mod translations from " << PATH_INFO::user_moddir();
        for( const std::string & dir
             : get_files_from_path( "LC_MESSAGES", PATH_INFO::user_moddir(), true ) ) {
            std::vector<std::string> mo_files = get_files_from_path( ".mo", dir, false, true );
            std::copy( mo_files.begin(), mo_files.end(),
                       std::back_inserter( mod_mo_files[LanguageCodeOfPath( mo_files.front() )] ) );
        }
    }
}

void TranslationManager::Impl::Reset()
{
    index.clear();
    index_capacity_mask = 0;
    plural_offset_map.clear();
    plural_rules_evaluator.reset();
    documents.clear();
}

TranslationManager::Impl::Impl()
{
    current_language_code = "en";
}

std::unordered_set<std::string> TranslationManager::Impl::GetAvailableLanguages()
{
    if( core_mo_files.empty() ) {
        ScanTranslationDocuments();
    }
    std::unordered_set<std::string> languages;
    for( const auto &[lang, path] : core_mo_files ) {
        languages.insert( lang );
    }
    return languages;
}

void TranslationManager::Impl::SetLanguage( const std::string &language_code )
{
    if( language_code == current_language_code ) {
        return;
    }
    if( core_mo_files.empty() ) {
        ScanTranslationDocuments();
    }
    current_language_code = language_code;
    if( core_mo_files.count( current_language_code ) == 0 ) {
        Reset();
        return;
    }
    LoadDocuments( core_mo_files[current_language_code], mod_mo_files[current_language_code] );
}

std::string TranslationManager::Impl::GetCurrentLanguage() const
{
    return current_language_code;
}

// Round n up to the nearest power of 2
static std::size_t bit_ceil( std::size_t n )
{
    if( n <= 1 ) {
        return 2;
    }
    std::size_t power = 2;
    --n;
    while( n >>= 1 ) {
        power <<= 1;
    }
    return power;
}

void TranslationManager::Impl::LoadDocuments( const std::string &core_mo_file,
        const std::vector<std::string> &mod_mo_files )
{
    Reset();
    if( core_mo_file.empty() || !file_exist( core_mo_file ) ) {
        return;
    }
    try {
        documents.emplace_back( core_mo_file );
        const auto [metadata, metadataLength] = documents.front().GetTranslatedString( 0 );
        LoadPluralRulesEvaluator( std::string_view{metadata, metadataLength} );
        for( const std::string &path : mod_mo_files ) {
            // Skip loading MO files from TEST_DATA mods if not in test mode
            if( !test_mode ) {
                if( path.find( "TEST_DATA" ) != std::string::npos ) {
                    continue;
                }
            }
            documents.emplace_back( path );
        }
        std::size_t numStrings = 0;
        for( const TranslationDocument &document : documents ) {
            numStrings += document.Count();
        }
        const std::size_t capacity = bit_ceil( numStrings * 1.5 );
        index.resize( capacity );
        index_capacity_mask = capacity - 1;
        for( const TranslationDocument &document : documents ) {
            for( std::size_t i = 1; i < document.Count(); i++ ) {
                const auto [source, sourceLength] = document.GetSourceString( i );
                const auto [translated, translatedLength] = document.GetTranslatedString( i );
                if( source[0] != '\0' ) {
                    InsertMessage( source, translated, translatedLength );
                }
            }
        }
    } catch( const std::exception &e ) {
        DebugLog( D_ERROR, DC_ALL ) << "[i18n] " << e.what();
        Reset();
    }
}

const char *TranslationManager::Impl::Translate( const std::string &message ) const
{
    return Translate( message.c_str() );
}

const char *TranslationManager::Impl::Translate( const char *message ) const
{
    if( const auto [header, translated] = Lookup( message ); translated ) {
        return translated;
    }
    return message;
}

const char *TranslationManager::Impl::TranslatePlural( const char *singular, const char *plural,
        const std::size_t n ) const
{
    if( const auto [header, translated] = Lookup( singular ); translated ) {
        const std::size_t pluralIndex = plural_rules_evaluator->Evaluate( n );
        if( pluralIndex == 0 ) {
            return translated;
        }
        if( const std::size_t pluralOffset = header.GetPluralOffset( pluralIndex ) ) {
            return translated + pluralOffset;
        }
        if( const auto it = plural_offset_map.find( translated ); it != plural_offset_map.end() &&
            it->second.size() >= pluralIndex ) {
            return translated + it->second[pluralIndex - 1];
        }
    }
    return ( n == 1 ) ? singular : plural;
}

std::string TranslationManager::Impl::ConstructContextualQuery( const char *context,
        const char *message )
{
    std::string query;
    query.reserve( strlen( context ) + 1 + strlen( message ) );
    query.append( context );
    query.append( "\004" );
    query.append( message );
    return query;
}

const char *TranslationManager::Impl::TranslateWithContext( const char *context,
        const char *message ) const
{
    const std::string query = ConstructContextualQuery( context, message );
    if( const auto [header, translated] = Lookup( query.c_str() ); translated ) {
        return translated;
    }
    return message;
}

const char *TranslationManager::Impl::TranslatePluralWithContext( const char *context,
        const char *singular,
        const char *plural,
        const std::size_t n ) const
{
    const std::string query = ConstructContextualQuery( context, singular );
    if( const auto [header, translated] = Lookup( query.c_str() ); translated ) {
        const std::size_t pluralIndex = plural_rules_evaluator->Evaluate( n );
        if( pluralIndex == 0 ) {
            return translated;
        }
        if( const std::size_t pluralOffset = header.GetPluralOffset( pluralIndex ) ) {
            return translated + pluralOffset;
        }
        if( const auto it = plural_offset_map.find( translated ); it != plural_offset_map.end() &&
            it->second.size() >= pluralIndex ) {
            return translated + it->second[pluralIndex - 1];
        }
    }
    return ( n == 1 ) ? singular : plural;
}

#endif // defined(LOCALIZE)

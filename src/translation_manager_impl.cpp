#if defined(LOCALIZE)

#include <cstring>

#include "cached_options.h"
#include "debug.h"
#include "filesystem.h"
#include "path_info.h"
#include "translations.h"
#include "translation_manager_impl.h"

std::uint32_t TranslationManager::Impl::Hash( const char *str )
{
    std::uint32_t hash = 5381U;
    while( *str != '\0' ) {
        hash = hash * 33 + ( *str++ );
    }
    return hash;
}

std::uint64_t TranslationManager::Impl::CreateIndexBytes( const std::uint8_t docIndex,
        const std::uint32_t localIndex, const std::uint16_t partialHash, const std::uint8_t psb )
{
    return ( static_cast<std::uint64_t>( psb ) << 56 ) | ( static_cast<std::uint64_t>
            ( partialHash ) << 40 ) | ( static_cast<std::uint64_t>( docIndex ) << 32 ) | localIndex;
}

std::uint8_t TranslationManager::Impl::MaskProbeSequenceLength( const std::uint64_t index )
{
    return index >> 56;
}

std::uint16_t TranslationManager::Impl::MaskPartialHash( const std::uint64_t index )
{
    return ( index >> 40 ) & 0xFFFFU;
}

std::uint8_t TranslationManager::Impl::MaskDocIndex( const std::uint64_t index )
{
    return ( index >> 32 ) & 0xFFU;
}

std::uint32_t TranslationManager::Impl::MaskLocalIndex( const std::uint64_t index )
{
    return index & 0xFFFFFFFFU;
}

void TranslationManager::Impl::AllocateIndex( const std::size_t n )
{
    index.clear();
    indexCapacityMask = 0;
    if( n == 0 ) {
        return;
    }
    const std::size_t capacity = std::pow( 2.0, std::ceil( std::log( n * 1.5 ) / std::log( 2 ) ) );
    index.resize( capacity );
    indexCapacityMask = capacity - 1;
}

void TranslationManager::Impl::InsertIndex( std::uint8_t docIndex, std::uint32_t localIndex,
        const std::size_t hash )
{
    std::uint8_t psb = 0;
    std::uint16_t partialHash = hash & 0xFFFFU;
    std::size_t bucket = hash & indexCapacityMask;
    for( ; psb <= 100; bucket = ( bucket + 1 ) & indexCapacityMask, psb++ ) {
        const std::uint64_t indexAtBucket = index[bucket];
        if( indexAtBucket == 0 ) {
            index[bucket] = CreateIndexBytes( docIndex, localIndex, partialHash, psb );
            return;
        }
        if( MaskProbeSequenceLength( indexAtBucket ) > psb ) {
            index[bucket] = CreateIndexBytes( docIndex, localIndex, partialHash, psb );
            psb = MaskProbeSequenceLength( indexAtBucket );
            partialHash = MaskPartialHash( indexAtBucket );
            docIndex = MaskDocIndex( indexAtBucket );
            localIndex = MaskLocalIndex( indexAtBucket );
        }
    }
    if( psb == 100 ) {
        throw std::runtime_error( "cannot find available bucket" );
    }
}

std::uint64_t TranslationManager::Impl::Lookup( const char *query ) const
{
    if( indexCapacityMask == 0 ) {
        return 0;
    }
    const std::size_t hash = Hash( query );
    const std::uint16_t partialHash = hash & 0xFFFFU;
    for( std::size_t bucket = hash & indexCapacityMask, psb = 0; psb <= 100;
         bucket = ( bucket + 1 ) & indexCapacityMask, psb++ ) {
        const std::uint64_t indexAtBucket = index[bucket];
        if( indexAtBucket == 0 ) {
            return 0;
        }
        if( MaskProbeSequenceLength( indexAtBucket ) > psb ) {
            return 0;
        }
        if( MaskPartialHash( indexAtBucket ) != partialHash ) {
            continue;
        }
        const std::uint8_t docIndex = MaskDocIndex( indexAtBucket );
        const std::uint32_t localIndex = MaskLocalIndex( indexAtBucket );
        if( strcmp( query, documents[docIndex].GetOriginalString( localIndex ) ) == 0 ) {
            return indexAtBucket;
        }
    }
    throw std::runtime_error( "psb overflow" );
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
    std::vector<std::pair<std::string, std::string>> mo_dirs;
    if( dir_exist( PATH_INFO::user_moddir() ) ) {
        DebugLog( D_INFO, DC_ALL ) << "[i18n] Scanning mod translations from " << PATH_INFO::user_moddir();
        for( const std::string & dir
             : get_files_from_path( "LC_MESSAGES", PATH_INFO::user_moddir(), true ) ) {
            mo_dirs.emplace_back( dir, ".mo" );
        }
    }
    if( dir_exist( locale_dir() ) ) {
        DebugLog( D_INFO, DC_ALL ) << "[i18n] Scanning core translations from " << locale_dir();
        for( const std::string &dir : get_files_from_path( "LC_MESSAGES", locale_dir(), true ) ) {
            mo_dirs.emplace_back( dir, "cataclysm-dda.mo" );
        }
    }
    for( const std::pair<std::string, std::string> &entry : mo_dirs ) {
        const std::string &dir = entry.first;
        const std::string &pattern = entry.second;
        std::vector<std::string> mo_dir_files = get_files_from_path( pattern, dir, false, true );
        for( const std::string &file : mo_dir_files ) {
            const std::string lang = LanguageCodeOfPath( file );
            if( mo_files.count( lang ) == 0 ) {
                mo_files[lang] = std::vector<std::string>();
            }
            mo_files[lang].emplace_back( file );
        }
    }
}

void TranslationManager::Impl::Reset()
{
    index.clear();
    indexCapacityMask = 0;
    documents.clear();
}

TranslationManager::Impl::Impl()
{
    current_language_code = "en";
}

std::unordered_set<std::string> TranslationManager::Impl::GetAvailableLanguages()
{
    if( mo_files.empty() ) {
        ScanTranslationDocuments();
    }
    std::unordered_set<std::string> languages;
    for( const auto &kv : mo_files ) {
        languages.insert( kv.first );
    }
    return languages;
}

void TranslationManager::Impl::SetLanguage( const std::string &language_code )
{
    if( mo_files.empty() ) {
        ScanTranslationDocuments();
    }
    if( language_code == current_language_code ) {
        return;
    }
    current_language_code = language_code;
    if( mo_files.count( current_language_code ) == 0 ) {
        Reset();
        return;
    }
    LoadDocuments( mo_files[current_language_code] );
}

std::string TranslationManager::Impl::GetCurrentLanguage() const
{
    return current_language_code;
}

void TranslationManager::Impl::LoadDocuments( const std::vector<std::string> &files )
{
    Reset();
    for( const std::string &file : files ) {
        try {
            // Skip loading MO files from TEST_DATA mods if not in test mode
            if( !test_mode ) {
                if( file.find( "TEST_DATA" ) != std::string::npos ) {
                    continue;
                }
            }
            if( file_exist( file ) ) {
                documents.emplace_back( file );
            }
        } catch( const InvalidTranslationDocumentException &e ) {
            DebugLog( D_ERROR, DC_ALL ) << e.what();
        }
    }
    std::size_t numStrings = 0;
    for( const TranslationDocument &document : documents ) {
        numStrings += document.Count();
    }
    AllocateIndex( numStrings );
    for( std::size_t document = 0; document < documents.size(); document++ ) {
        for( std::size_t i = 0; i < documents[document].Count(); i++ ) {
            const char *message = documents[document].GetOriginalString( i );
            if( message[0] != '\0' ) {
                InsertIndex( document, i, Hash( message ) );
            }
        }
    }
}

const char *TranslationManager::Impl::Translate( const std::string &message ) const
{
    return Translate( message.c_str() );
}

const char *TranslationManager::Impl::Translate( const char *message ) const
{
    const std::uint64_t index = Lookup( message );
    if( index == 0 ) {
        return message;
    }
    const std::uint8_t docIndex = MaskDocIndex( index );
    const std::uint32_t localIndex = MaskLocalIndex( index );
    return documents[docIndex].GetTranslatedString( localIndex );
}

const char *TranslationManager::Impl::TranslatePlural( const char *singular, const char *plural,
        std::size_t n ) const
{
    const std::uint64_t index = Lookup( singular );
    if( index == 0 ) {
        if( n == 1 ) {
            return singular;
        } else {
            return plural;
        }
    }
    const std::uint8_t docIndex = MaskDocIndex( index );
    const std::uint32_t localIndex = MaskLocalIndex( index );
    return documents[docIndex].GetTranslatedStringPlural( localIndex, n );
}

std::string TranslationManager::Impl::ConstructContextualQuery( const char *context,
        const char *message ) const
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
    const std::uint64_t index = Lookup( query.c_str() );
    if( index == 0 ) {
        return message;
    }
    const std::uint8_t docIndex = MaskDocIndex( index );
    const std::uint32_t localIndex = MaskLocalIndex( index );
    return documents[docIndex].GetTranslatedString( localIndex );
}

const char *TranslationManager::Impl::TranslatePluralWithContext( const char *context,
        const char *singular,
        const char *plural,
        std::size_t n ) const
{
    const std::string query = ConstructContextualQuery( context, singular );
    const std::uint64_t index = Lookup( query.c_str() );
    if( index == 0 ) {
        if( n == 1 ) {
            return singular;
        } else {
            return plural;
        }
    }
    const std::uint8_t docIndex = MaskDocIndex( index );
    const std::uint32_t localIndex = MaskLocalIndex( index );
    return documents[docIndex].GetTranslatedStringPlural( localIndex, n );
}

#endif // defined(LOCALIZE)

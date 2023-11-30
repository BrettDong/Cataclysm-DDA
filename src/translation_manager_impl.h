#pragma once
#ifndef CATA_SRC_TRANSLATION_MANAGER_IMPL_H
#define CATA_SRC_TRANSLATION_MANAGER_IMPL_H

#if defined(LOCALIZE)

#include <unordered_map>

#include "translation_document.h"
#include "translation_manager.h"

class TranslationManager::Impl
{
    private:
        std::vector<TranslationDocument> documents;
        std::vector<std::uint64_t> index;
        std::size_t indexCapacityMask = 0;

        static std::uint32_t Hash( const char *str );
        static std::uint8_t MaskProbeSequenceLength( std::uint64_t index );
        static std::uint16_t MaskPartialHash( std::uint64_t index );
        static std::uint8_t MaskDocIndex( std::uint64_t index );
        static std::uint32_t MaskLocalIndex( std::uint64_t index );
        static std::uint64_t CreateIndexBytes( std::uint8_t docIndex, std::uint32_t localIndex,
                                               std::uint16_t partialHash, std::uint8_t psb );
        void AllocateIndex( std::size_t n );
        void InsertIndex( std::uint8_t docIndex, std::uint32_t localIndex, std::size_t hash );
        std::uint64_t Lookup( const char *query ) const;

        std::unordered_map<std::string, std::vector<std::string>> mo_files;
        static std::string LanguageCodeOfPath( std::string_view path );
        void ScanTranslationDocuments();
        std::string ConstructContextualQuery( const char *context, const char *message ) const;
        void Reset();
        std::string current_language_code;
    public:
        Impl();
        std::unordered_set<std::string> GetAvailableLanguages();
        void SetLanguage( const std::string &language_code );
        std::string GetCurrentLanguage() const;
        void LoadDocuments( const std::vector<std::string> &files );

        const char *Translate( const std::string &message ) const;
        const char *Translate( const char *message ) const;
        const char *TranslatePlural( const char *singular, const char *plural, std::size_t n ) const;
        const char *TranslateWithContext( const char *context, const char *message ) const;
        const char *TranslatePluralWithContext( const char *context, const char *singular,
                                                const char *plural, std::size_t n ) const;
};

#endif // defined(LOCALIZE)

#endif // CATA_SRC_TRANSLATION_MANAGER_IMPL_H

#pragma once
#ifndef CATA_SRC_TRANSLATION_MANAGER_IMPL_H
#define CATA_SRC_TRANSLATION_MANAGER_IMPL_H

#if defined(LOCALIZE)

#include <unordered_map>

#include "translation_document.h"
#include "translation_manager.h"
#include "translation_plural_evaluator.h"

class TranslationManager::Impl
{
    private:
        static constexpr std::size_t MaxProbeSequenceLength = 200;

        struct IndexEntryHeader {
            /* ^ Most Significant Bit (63)
             * +-----------------------------+
             * | 8-bit Probe Sequence Length |
             * +-----------------------------+
             * | 12-bit Fast PluralOffset[3] |
             * +-----------------------------+
             * | 12-bit Fast PluralOffset[2] |
             * +-----------------------------+
             * | 12-bit Fast PluralOffset[1] |
             * +-----------------------------+
             * | 20-bit PartialHash          |
             * +-----------------------------+
             * v Least Significant Bit (0) */
            std::uint64_t RawValue = 0;

            IndexEntryHeader() = default;
            IndexEntryHeader( std::size_t ProbeSequenceLength, std::size_t PartialHash,
                              const std::array<std::size_t, 3> &PluralOffsets );
            std::size_t GetProbeSequenceLength() const noexcept;
            std::size_t GetPluralOffset( std::size_t pluralIndex ) const noexcept;
            std::size_t GetPartialHash() const noexcept;
        };

        struct IndexEntry {
            IndexEntryHeader Header;
            const char *pSourceString = nullptr;
            const char *pTranslatedString = nullptr;

            IndexEntry() = default;
            IndexEntry( std::size_t ProbeSequenceLength, std::size_t PartialHash,
                        std::array<std::size_t, 3> PluralOffsets, const char *pSoureString,
                        const char *pTranslatedString );
            bool IsEmpty() const noexcept;
        };

        // Stores the currently loaded mmap'ed MO files
        std::vector<TranslationDocument> documents;

        // A robin hood hash table for source-translation key-value pairs and fast-path look up of plural offsets
        std::vector<IndexEntry> index;
        std::size_t index_capacity_mask = 0;

        // Slow-path look up table for plural offsets of plural messages
        std::unordered_map<const char *, std::vector<std::size_t>> plural_offset_map;

        std::unique_ptr<TranslationPluralRulesEvaluator> plural_rules_evaluator;

        static std::uint64_t Hash( const char *str ) noexcept;
        static std::size_t MaskPartialHash( std::uint64_t hash ) noexcept;
        std::size_t MaskBucketSlot( std::uint64_t hash ) const noexcept;
        void InsertIndex( const char *source, const char *translated,
                          std::array<std::size_t, 3> pluralOffsets );
        void InsertMessage( const char *source, const char *translated, std::size_t translatedLength );
        void LoadPluralRulesEvaluator( std::string_view metadata );
        std::pair<IndexEntryHeader, const char *> Lookup( const char *query ) const;

        std::unordered_map<std::string, std::string> core_mo_files;
        std::unordered_map<std::string, std::vector<std::string>> mod_mo_files;
        static std::string LanguageCodeOfPath( std::string_view path );
        void ScanTranslationDocuments();
        static std::string ConstructContextualQuery( const char *context, const char *message );
        void Reset();
        std::string current_language_code;
    public:
        Impl();
        std::unordered_set<std::string> GetAvailableLanguages();
        void SetLanguage( const std::string &language_code );
        std::string GetCurrentLanguage() const;
        void LoadDocuments( const std::string &core_mo_file, const std::vector<std::string> &mod_mo_files = {} );

        const char *Translate( const std::string &message ) const;
        const char *Translate( const char *message ) const;
        const char *TranslatePlural( const char *singular, const char *plural, std::size_t n ) const;
        const char *TranslateWithContext( const char *context, const char *message ) const;
        const char *TranslatePluralWithContext( const char *context, const char *singular,
                                                const char *plural, std::size_t n ) const;
};

#endif // defined(LOCALIZE)

#endif // CATA_SRC_TRANSLATION_MANAGER_IMPL_H

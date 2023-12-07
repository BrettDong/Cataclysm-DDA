#pragma once
#ifndef CATA_SRC_TRANSLATION_DOCUMENT_H
#define CATA_SRC_TRANSLATION_DOCUMENT_H

#if defined(LOCALIZE)

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "mmap_file.h"
#include "translation_plural_evaluator.h"

class InvalidTranslationDocumentException final : public std::runtime_error
{
    public:
        explicit InvalidTranslationDocumentException( std::string_view path, std::string_view message );
};

enum class Endianness {
    BigEndian,
    LittleEndian
};

/**
 * Represents a GNU gettext Message Object (.mo) document
 * Format specification: https://www.gnu.org/software/gettext/manual/html_node/MO-Files.html
 */
class TranslationDocument
{
    private:
        std::shared_ptr<mmap_file> mmap_message_object;
        const char *data;
        std::size_t data_size;
        std::size_t number_of_strings; // N
        std::size_t original_strings_table_offset; // O
        std::size_t translated_strings_table_offset; // T
        // std::size_t hash_table_size; // S
        // std::size_t hash_table_offset; // H
        Endianness endianness;

        std::uint8_t GetByte( std::size_t byteIndex ) const;
        std::uint32_t GetUint32BE( std::size_t byteIndex ) const;
        std::uint32_t GetUint32LE( std::size_t byteIndex ) const;
        std::uint32_t ( TranslationDocument::*GetUint32FPtr )( const std::size_t ) const;
        std::uint32_t GetUint32( std::size_t byteIndex ) const;
    public:
        TranslationDocument() = delete;
        explicit TranslationDocument( const std::string &path );

        std::size_t Count() const;
        std::pair<const char *, std::size_t> GetSourceString( std::size_t index ) const;
        std::pair<const char *, std::size_t> GetTranslatedString( std::size_t index ) const;
};

#endif // defined(LOCALIZE)

#endif // CATA_SRC_TRANSLATION_DOCUMENT_H

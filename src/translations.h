#pragma once
#ifndef CATA_SRC_TRANSLATIONS_H
#define CATA_SRC_TRANSLATIONS_H

// on some systems <locale> pulls in libintl.h anyway,
// so preemptively include it before the gettext overrides.
#include <locale> // IWYU pragma: keep

#include "translation.h"
#include "translation_manager.h"

class language_cache_invalidator
{
    public:
        language_cache_invalidator();

        bool invalidated();

        static void on_global_language_changed();
    private:
        std::uint32_t counter;
        static std::uint32_t global_counter;
};

#if !defined(translate_marker)
/**
 * Marks a string literal to be extracted for translation. This is only for running `xgettext` via
 * "lang/update_pot.sh". Use `_` to extract *and* translate at run time. The macro itself does not
 * do anything, the argument is passed through it without any changes.
 */
#define translate_marker(x) x
#endif
#if !defined(translate_marker_context)
/**
 * Same as @ref translate_marker, but also provides a context (string literal). This is similar
 * to @ref pgettext, but it does not translate at run time. Like @ref translate_marker it just
 * passes the *second* argument through.
 */
#define translate_marker_context(c, x) x
#endif

#if defined(LOCALIZE)

#if defined(__GNUC__)
#  define ATTRIBUTE_FORMAT_ARG(a) __attribute__((format_arg(a)))
#else
#  define ATTRIBUTE_FORMAT_ARG(a)
#endif

void select_language();

#define _(msgid) (TranslationManager::GetInstance().Translate( msgid ))

inline const char *n_gettext( const char *msgid, const char *msgid_plural,
                              std::size_t n ) ATTRIBUTE_FORMAT_ARG( 1 );

inline const char *n_gettext( const char *msgid, const char *msgid_plural,
                              std::size_t n )
{
    return TranslationManager::GetInstance().TranslatePlural( msgid, msgid_plural, n );
}

inline const char *pgettext( const char *context, const char *msgid ) ATTRIBUTE_FORMAT_ARG( 2 );

inline const char *pgettext( const char *context, const char *msgid )
{
    return TranslationManager::GetInstance().TranslateWithContext( context, msgid );
}

inline const char *npgettext( const char *context, const char *msgid,
                              const char *msgid_plural, unsigned long long n ) ATTRIBUTE_FORMAT_ARG( 2 );

inline const char *npgettext( const char *const context, const char *const msgid,
                              const char *const msgid_plural, const unsigned long long n )
{
    return TranslationManager::GetInstance().TranslatePluralWithContext( context, msgid, msgid_plural,
            n );
}

#else // !LOCALIZE

#define _(STRING) (STRING)

#define n_gettext(STRING1, STRING2, COUNT) ((COUNT) == 1 ? _(STRING1) : _(STRING2))
#define pgettext(STRING1, STRING2) _(STRING2)
#define npgettext(STRING0, STRING1, STRING2, COUNT) n_gettext(STRING1, STRING2, COUNT)

#endif // LOCALIZE

// Avoid using these functions from libintl.h which won't work because we have
// our own implementations with different names.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wredundant-decls"
[[deprecated( "Use _( ... ) instead" )]]
//NOLINTNEXTLINE(readability-redundant-declaration,readability-inconsistent-declaration-parameter-name,cata-no-long)
char *gettext( const char *msgid );
[[deprecated( "Use n_gettext( ... ) instead" )]]
//NOLINTNEXTLINE(readability-redundant-declaration,readability-inconsistent-declaration-parameter-name,cata-no-long)
char *ngettext( const char *msgid, const char *msgid2, unsigned long int n );
#pragma GCC diagnostic pop

std::string locale_dir();

void set_language_from_options();
void set_language( const std::string &lang );

#endif // CATA_SRC_TRANSLATIONS_H

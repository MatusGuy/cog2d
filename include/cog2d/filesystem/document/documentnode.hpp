// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

// NOTE: What a disaster of a class... It was attempt to make a parser-agnostic
// interface for documents, but I don't know how to implement it in a way that
// doesn't cause a lot of overhead

/*
#include "cog2d/filesystem/iodevice.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class DocumentNode
{
public:
    struct array_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = DocumentNode;
        using pointer = std::unique_ptr<value_type>;
        using reference = value_type&;

        virtual reference operator++() { return *this; }
        virtual pointer operator->() { return nullptr; }
        virtual bool operator!=() { return false; }
    };

    struct dict_iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<std::string, DocumentNode>;
        using pointer = std::unique_ptr<value_type>;
        using reference = value_type&;

        virtual reference operator++() { return *this; }
        virtual pointer operator->() { return nullptr; }
        virtual bool operator!=() { return false; }
    };

public:
    DocumentNode() {}

    enum ValueType
    {
        VALUE_INT,
        VALUE_FLOAT,
        VALUE_STRING,
        VALUE_BOOL,
        VALUE_ARRAY,
        VALUE_DICT
    };

    virtual void load(IoDevice&& device) = 0;

    virtual std::unique_ptr<DocumentNode> get(std::string_view key) = 0;
    virtual std::unique_ptr<DocumentNode> get(int key) = 0;

    virtual int as_int() = 0;
    virtual float as_float() = 0;
    virtual std::string as_string() = 0;
    virtual bool as_bool() = 0;

    virtual bool contains(std::string_view key) = 0;
    virtual ValueType type() = 0;
    virtual bool is_root() { return m_root; }

    virtual array_iterator abegin() = 0;
    virtual array_iterator aend() = 0;

    virtual dict_iterator dbegin() = 0;
    virtual dict_iterator dend() = 0;

protected:
    DocumentNode(bool root)
        : m_root(root)
    {
    }

    bool m_root = true;
};

COG2D_NAMESPACE_END_DECL
*/

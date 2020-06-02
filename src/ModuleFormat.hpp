//
// Created by chorm on 2020-05-28.
//

#ifndef ORKA_FRAMEWORK_MODULEFORMAT_HPP
#define ORKA_FRAMEWORK_MODULEFORMAT_HPP

#include <lclib-c++/BinaryIOHelpers.hpp>
#include <lclib-c++/Version.hpp>

#include <array>

namespace mia::framework{
    constexpr lclib::io::MagicNumbers<uint8_t[4]> MOD_MAGIC{{0xEE,0x4F,0x4B,0x4D}};
    constexpr lclib::io::MagicNumbers<uint16_t> VTABLE_LEN{2};
    enum ConstantTag:uint8_t{
        Const_Utf8,
        Const_i32,
        Const_i64,
        Const_u32,
        Const_u64,
        Const_Type,
        Const_Version
    };
    template<ConstantTag> struct ConstantEntry{
        uint16_t item;
        [[nodiscard]] bool isZero()const{
            return item==0;
        }
        friend lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream& in,ConstantEntry& entry){
            return in >> entry.item;
        }
        friend lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream& out,const ConstantEntry& entry){
            return out << entry.item;
        }
    };
    struct Constant{
        std::variant<std::string,std::int32_t,std::int64_t,std::uint32_t,std::uint64_t,ConstantEntry<ConstantTag::Const_Utf8>,ConstantEntry<ConstantTag::Const_Utf8>>
            value;
        friend lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,Constant&);
        friend lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const Constant&);
    };
    lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,Constant&);
    lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const Constant&);
    template<ConstantTag tag> decltype(auto) get_entry(ConstantEntry<tag> entry,const std::vector<Constant>& pool){
        return std::get<static_cast<std::size_t>(tag)>(pool.at(entry.item));
    }
    template<ConstantTag tag> std::optional<std::reference_wrapper<const Constant>> get_nonzero_entry(ConstantEntry<tag> entry,const std::vector<Constant>& pool){
        if(entry.isZero())
            return std::nullopt;
        else
            return std::get<static_cast<std::size_t>(tag)>(pool.at(entry.item));
    }
    enum class DependencyOrder:uint8_t{
        RequiredAfter,
        OptionalAfter,
        OptionalBefore,
        OptionalUnordered,
        RequiredBefore,
        RequiredUnordered,
        Init,
        Intercept
    };
    struct Dependency{
        ConstantEntry<ConstantTag::Const_Utf8> modName;
        ConstantEntry<ConstantTag::Const_Version> modVersion;
        DependencyOrder order;

        friend lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,Dependency&);
        friend lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const Dependency&);
    };



    lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,Dependency&);
    lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const Dependency&);

    struct Attribute{
        ConstantEntry<ConstantTag::Const_Utf8> tag;
        lclib::io::variable_sized<std::vector<std::uint8_t>,std::uint16_t> content;

        friend lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,Attribute&);
        friend lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const Attribute&);
    };
    lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,Attribute&);
    lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const Attribute&);

    enum class ItemKind:uint8_t{
        Field,
        Function,
        Interface,
        Type
    };

    struct Item{
        ItemKind kind;
        ConstantEntry<ConstantTag::Const_Utf8> name;
        ConstantEntry<ConstantTag::Const_Type> type;
        ConstantEntry<ConstantTag::Const_Utf8> value;
        lclib::io::variable_sized<std::vector<Attribute>,std::uint16_t> attrs;
    };

    lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream& in,Item& item);
    lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream& out,const Item& item);

    enum class TypeKind: uint8_t{
        Struct,
        Class,
        Interface,
        ProvidedInterface
    };

    using items = lclib::io::variable_array<Item,std::uint16_t>;

    struct Type{
        TypeKind kind;
        ConstantEntry<ConstantTag::Const_Utf8> name;
        std::variant<items,ConstantEntry<ConstantTag::Const_Type>> items;
        lclib::io::variable_array<Attribute> attrs;
    };

    lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream& in,Type& type);
    lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream& out,const Type& type);

    struct ModuleFile{
        lclib::io::MagicNumbers<uint8_t[4]> magic{MOD_MAGIC};
        lclib::version::Version ver;
        lclib::io::variable_sized<std::vector<Constant>,uint16_t> constants;
        ConstantEntry<ConstantTag::Const_Utf8> mod_name;
        ConstantEntry<ConstantTag::Const_Version> mod_version;
        lclib::io::variable_sized<std::vector<Dependency>,uint16_t> dependencies;
        lclib::io::variable_array<Item> items;
        lclib::io::variable_array<Type> types;
        std::array<ConstantEntry<ConstantTag::Const_Utf8>,6> initTable;

        friend lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,ModuleFile&);
        friend lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const ModuleFile&);
    };

    lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream&,ModuleFile&);
    lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream&,const ModuleFile&);
}

#endif //ORKA_FRAMEWORK_MODULEFORMAT_HPP

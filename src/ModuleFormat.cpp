//
// Created by chorm on 2020-05-28.
//

#include "ModuleFormat.hpp"

lclib::io::DataOutputStream & mia::framework::operator<<(lclib::io::DataOutputStream & out, const mia::framework::Constant & entry) {
    return out << static_cast<ConstantTag>(entry.value.index()) << entry.value;
}

lclib::io::DataInputStream &mia::framework::operator>>(lclib::io::DataInputStream & in, mia::framework::Constant& item) {
    auto tag{in.read<ConstantTag>()};
    switch(tag){
        case ConstantTag::Const_Utf8:
            item.value.emplace<ConstantTag::Const_Utf8>();
            break;
        case ConstantTag::Const_i32:
            item.value.emplace<ConstantTag::Const_i32>();
            break;
        case ConstantTag::Const_i64:
            item.value.emplace<ConstantTag::Const_i64>();
            break;
        case ConstantTag::Const_u32:
            item.value.emplace<ConstantTag::Const_u32>();
            break;
        case ConstantTag::Const_u64:
            item.value.emplace<ConstantTag::Const_u64>();
            break;
        case ConstantTag::Const_Type:
            item.value.emplace<ConstantTag::Const_Type>();
            break;
        case ConstantTag::Const_Version:
            item.value.emplace<ConstantTag::Const_Version>();
            break;
        default:
            throw lclib::io::IOException{"Invalid Constant Pool tag"};
    }
    return in >> item.value;
}

lclib::io::DataInputStream &mia::framework::operator>>(lclib::io::DataInputStream & in, mia::framework::Dependency & dep) {
    return in >> dep.modName >> dep.modVersion >> dep.order;
}

lclib::io::DataOutputStream &
mia::framework::operator<<(lclib::io::DataOutputStream & out, const mia::framework::Dependency & dep) {
    return out << dep.modName << dep.modVersion << dep.order;
}

lclib::io::DataInputStream &mia::framework::operator>>(lclib::io::DataInputStream& in, mia::framework::ModuleFile& mod) {
    return in >> mod.magic >> mod.ver >> mod.constants >> mod.mod_name >> mod.mod_version >> mod.dependencies
        >> mod.items >> mod.types >> mod.initTable;
}

lclib::io::DataOutputStream &
mia::framework::operator<<(lclib::io::DataOutputStream & out, const mia::framework::ModuleFile & mod) {
    return out << mod.magic << mod.ver << mod.constants << mod.mod_name << mod.mod_version
        << mod.dependencies << mod.items << mod.types << mod.initTable;
}

lclib::io::DataInputStream &mia::framework::operator>>(lclib::io::DataInputStream & in, mia::framework::Attribute & attr) {
    return in >> attr.tag >> attr.content;
}

lclib::io::DataOutputStream &
mia::framework::operator<<(lclib::io::DataOutputStream & out, const mia::framework::Attribute & attr) {
    return out << attr.tag << attr.content;
}

using namespace mia::framework;

lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream& in,Item& item){
    return in >> item.kind >> item.name >> item.value >> item.type >> item.attrs;
}
lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream& out,const Item& item){
    return out << item.kind << item.name << item.value << item.type << item.attrs;
}

lclib::io::DataInputStream& operator>>(lclib::io::DataInputStream& in,Type& type){
    in >> type.kind >> type.name;
    switch(type.kind){
        case TypeKind::Struct:
        case TypeKind::Class:
        case TypeKind::Interface:
            type.items.emplace<0>();
            break;
        case TypeKind::ProvidedInterface:
            type.items.emplace<1>();
            break;
    }
    return in >> type.items >> type.attrs;
}
lclib::io::DataOutputStream& operator<<(lclib::io::DataOutputStream& out,const Type& type){
    return out << type.kind << type.name << type.items << type.attrs;
}

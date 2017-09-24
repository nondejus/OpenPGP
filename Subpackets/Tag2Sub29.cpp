#include "Tag2Sub29.h"

bool Revoke::is_key_revocation(const uint8_t code){
    return ((NO_REASON_SPECIFIED <= code) &&
            (code <= KEY_IS_NO_LONGER_USED));
}

Tag2Sub29::Tag2Sub29()
    : Tag2Subpacket(Tag2Subpacket::REASON_FOR_REVOCATION),
      code(),
      reason()
{}

Tag2Sub29::Tag2Sub29(const std::string & data)
    : Tag2Sub29()
{
    read(data);
}

void Tag2Sub29::read(const std::string & data){
    if (data.size()){
        code = data[0];
        reason = data.substr(1, data.size() - 1);
        size = data.size();
    }
}

std::string Tag2Sub29::show(const std::size_t indents, const std::size_t indent_size) const{
    const std::string indent(indents * indent_size, ' ');
    const std::string tab(indent_size, ' ');

    const decltype(Revoke::NAME)::const_iterator revoke_it = Revoke::NAME.find(code);

    std::string out = indent + show_title() + "\n" +
                      indent + tab + "Reason " + std::to_string(code) + " - " + ((revoke_it == Revoke::NAME.end())?"Unknown":(revoke_it -> second));
    if (code){
        out += "\n" + indent + tab + "Comment - " + reason;
    }

    return out;
}

std::string Tag2Sub29::raw() const{
    return std::string(1, code) + reason;
}

uint8_t Tag2Sub29::get_code() const{
    return code;
}

std::string Tag2Sub29::get_reason() const{
    return reason;
}

void Tag2Sub29::set_code(const uint8_t c){
    code = c;
}

void Tag2Sub29::set_reason(const std::string & r){
    reason = r;
}

Tag2Subpacket::Ptr Tag2Sub29::clone() const{
    return std::make_shared <Tag2Sub29> (*this);
}

typedef boost::uint16_t code_point_t;

enum xml_parse_attribute_t {
    xml_none        = 0,
    xml_qualified   = 1,
    xml_reserved    = 2
};

xml_parse_attribute_t& operator|=(xml_parse_attribute_t& r, xml_parse_attribute_t x)
{ r = xml_parse_attribute_t(r | x); return r; }

bool in_base_char(code_point_t);
bool in_ideographic(code_point_t);
bool in_combining_char(code_point_t);
bool in_digit(code_point_t);
bool in_extender(code_point_t);

template <typename I> // I models InputIterator
// require value_type(I) == code_point_t
struct xml_parser
{

    bool is_letter(code_point_t& c) { return is_base_char(c) || is_ideographic(c); }

    bool is_base_char(code_point_t& c) { return is_match(in_base_char, c); }
    bool is_ideographic(code_point_t& c) { return is_match(in_ideographic, c); }
    bool is_combining_char(code_point_t& c) { return is_match(in_combining_char, c); }
    bool is_digit(code_point_t& c) { return is_match(in_digit, c); }
    bool is_extender(code_point_t& c) { return is_match(in_extender, c); }

    bool is_name_char(code_point_t& c, xml_parse_attribute_t& a)
    {
        if (is_match(':', c)) { a |= xml_qualified; return true; }

        return is_letter(c) || is_digit(c) || is_match('.', c)
            || is_match('-', c) || is_match('_', c) || is_combining_char(c) || is_extender(c);
    }

    bool is_name(xml_parse_attribute_t& a)
    {
        code_point_t c;

        if (is_match(':', c)) { a |= xml_qualified; }
        else if (is_letter(c) || is_match('_', c)) ; else return false;
        bool reserved = c == code_point_t('X') || c == code_point_t('x');

        if (is_name_char(c, a)) ; else return true;
        reserved &= (c == code_point_t('M') || c == code_point_t('m'));

        if (is_name_char(c, a)) ; else return true;
        reserved &= (c == code_point_t('L') || c == code_point_t('l'));

        while (is_name_char(c, a)) ;

        if (reserved) a |= xml_reserved;

        return true;
    }

    I f_m;
    I l_m;

 private:

    bool is_match(bool (*p)(code_point_t), code_point_t& c)
    {
        if (f_m == l_m || !p(*f_m)) return false;
        c = *f_m; ++f_m; return true;
    }

    bool is_match(code_point_t x, code_point_t& c)
    {
        if (f_m == l_m || *f_m != x) return false;
        c = x; ++f_m; return true;
    }
};

template <typename I> // I models InputIterator
// require value_type(I) == code_point_t
bool valid_name_unqualified_unreserved(I f, I l)
{
    xml_parser<I> parser = { f, l };
    xml_parse_attribute_t a = xml_none; 
    return parser.is_name(a) && parser.f_m == l && a == xml_none;
}

} // namespace adobe_example

// ----

// .cpp file

#include <boost/range/begin.hpp>
#include <adobe/algorithm/lower_bound.hpp>

namespace adobe_example {

bool in_base_char(code_point_t c)
{
    static const code_point_t table[] =
    {
        0x0040U, 0x005AU,
        0x0060U, 0x007AU,
        0x00BFU, 0x00D6U,
        0x00D7U, 0x00F6U,
        0x00F7U, 0x00FFU,
        0x00FFU, 0x0131U,
        0x0133U, 0x013EU,
        0x0140U, 0x0148U,
        0x0149U, 0x017EU,
        0x017FU, 0x01C3U,
        0x01CCU, 0x01F0U,
        0x01F3U, 0x01F5U,
        0x01F9U, 0x0217U,
        0x024FU, 0x02A8U,
        0x02BAU, 0x02C1U,
        0x0385U, 0x0386U,
        0x0387U, 0x038AU,
        0x038BU, 0x038CU,
        0x038DU, 0x03A1U,
        0x03A2U, 0x03CEU,
        0x03CFU, 0x03D6U,
        0x03D9U, 0x03DAU,
        0x03DBU, 0x03DCU,
        0x03DDU, 0x03DEU,
        0x03DFU, 0x03E0U,
        0x03E1U, 0x03F3U,
        0x0400U, 0x040CU,
        0x040DU, 0x044FU,
        0x0450U, 0x045CU,
        0x045DU, 0x0481U,
        0x048FU, 0x04C4U,
        0x04C6U, 0x04C8U,
        0x04CAU, 0x04CCU,
        0x04CFU, 0x04EBU,
        0x04EDU, 0x04F5U,
        0x04F7U, 0x04F9U,
        0x0530U, 0x0556U,
        0x0558U, 0x0559U,
        0x0560U, 0x0586U,
        0x05CFU, 0x05EAU,
        0x05EFU, 0x05F2U,
        0x0620U, 0x063AU,
        0x0640U, 0x064AU,
        0x0670U, 0x06B7U,
        0x06B9U, 0x06BEU,
        0x06BFU, 0x06CEU,
        0x06CFU, 0x06D3U,
        0x06D4U, 0x06D5U,
        0x06E4U, 0x06E6U,
        0x0904U, 0x0939U,
        0x093CU, 0x093DU,
        0x0957U, 0x0961U,
        0x0984U, 0x098CU,
        0x098EU, 0x0990U,
        0x0992U, 0x09A8U,
        0x09A9U, 0x09B0U,
        0x09B1U, 0x09B2U,
        0x09B5U, 0x09B9U,
        0x09DBU, 0x09DDU,
        0x09DEU, 0x09E1U,
        0x09EFU, 0x09F1U,
        0x0A04U, 0x0A0AU,
        0x0A0EU, 0x0A10U,
        0x0A12U, 0x0A28U,
        0x0A29U, 0x0A30U,
        0x0A31U, 0x0A33U,
        0x0A34U, 0x0A36U,
        0x0A37U, 0x0A39U,
        0x0A58U, 0x0A5CU,
        0x0A5DU, 0x0A5EU,
        0x0A71U, 0x0A74U,
        0x0A84U, 0x0A8BU,
        0x0A8CU, 0x0A8DU,
        0x0A8EU, 0x0A91U,
        0x0A92U, 0x0AA8U,
        0x0AA9U, 0x0AB0U,
        0x0AB1U, 0x0AB3U,
        0x0AB4U, 0x0AB9U,
        0x0ABCU, 0x0ABDU,
        0x0ADFU, 0x0AE0U,
        0x0B04U, 0x0B0CU,
        0x0B0EU, 0x0B10U,
        0x0B12U, 0x0B28U,
        0x0B29U, 0x0B30U,
        0x0B31U, 0x0B33U,
        0x0B35U, 0x0B39U,
        0x0B3CU, 0x0B3DU,
        0x0B5BU, 0x0B5DU,
        0x0B5EU, 0x0B61U,
        0x0B84U, 0x0B8AU,
        0x0B8DU, 0x0B90U,
        0x0B91U, 0x0B95U,
        0x0B98U, 0x0B9AU,
        0x0B9BU, 0x0B9CU,
        0x0B9DU, 0x0B9FU,
        0x0BA2U, 0x0BA4U,
        0x0BA7U, 0x0BAAU,
        0x0BADU, 0x0BB5U,
        0x0BB6U, 0x0BB9U,
        0x0C04U, 0x0C0CU,
        0x0C0DU, 0x0C10U,
        0x0C11U, 0x0C28U,
        0x0C29U, 0x0C33U,
        0x0C34U, 0x0C39U,
        0x0C5FU, 0x0C61U,
        0x0C84U, 0x0C8CU,
        0x0C8DU, 0x0C90U,
        0x0C91U, 0x0CA8U,
        0x0CA9U, 0x0CB3U,
        0x0CB4U, 0x0CB9U,
        0x0CDDU, 0x0CDEU,
        0x0CDFU, 0x0CE1U,
        0x0D04U, 0x0D0CU,
        0x0D0DU, 0x0D10U,
        0x0D11U, 0x0D28U,
        0x0D29U, 0x0D39U,
        0x0D5FU, 0x0D61U,
        0x0E00U, 0x0E2EU,
        0x0E2FU, 0x0E30U,
        0x0E31U, 0x0E33U,
        0x0E3FU, 0x0E45U,
        0x0E80U, 0x0E82U,
        0x0E83U, 0x0E84U,
        0x0E86U, 0x0E88U,
        0x0E89U, 0x0E8AU,
        0x0E8CU, 0x0E8DU,
        0x0E93U, 0x0E97U,
        0x0E98U, 0x0E9FU,
        0x0EA0U, 0x0EA3U,
        0x0EA4U, 0x0EA5U,
        0x0EA6U, 0x0EA7U,
        0x0EA9U, 0x0EABU,
        0x0EACU, 0x0EAEU,
        0x0EAFU, 0x0EB0U,
        0x0EB1U, 0x0EB3U,
        0x0EBCU, 0x0EBDU,
        0x0EBFU, 0x0EC4U,
        0x0F3FU, 0x0F47U,
        0x0F48U, 0x0F69U,
        0x109FU, 0x10C5U,
        0x10CFU, 0x10F6U,
        0x10FFU, 0x1100U,
        0x1101U, 0x1103U,
        0x1104U, 0x1107U,
        0x1108U, 0x1109U,
        0x110AU, 0x110CU,
        0x110DU, 0x1112U,
        0x113BU, 0x113CU,
        0x113DU, 0x113EU,
        0x113FU, 0x1140U,
        0x114BU, 0x114CU,
        0x114DU, 0x114EU,
        0x114FU, 0x1150U,
        0x1153U, 0x1155U,
        0x1158U, 0x1159U,
        0x115EU, 0x1161U,
        0x1162U, 0x1163U,
        0x1164U, 0x1165U,
        0x1166U, 0x1167U,
        0x1168U, 0x1169U,
        0x116CU, 0x116EU,
        0x1171U, 0x1173U,
        0x1174U, 0x1175U,
        0x119DU, 0x119EU,
        0x11A7U, 0x11A8U,
        0x11AAU, 0x11ABU,
        0x11ADU, 0x11AFU,
        0x11B6U, 0x11B8U,
        0x11B9U, 0x11BAU,
        0x11BBU, 0x11C2U,
        0x11EAU, 0x11EBU,
        0x11EFU, 0x11F0U,
        0x11F8U, 0x11F9U,
        0x1DFFU, 0x1E9BU,
        0x1E9FU, 0x1EF9U,
        0x1EFFU, 0x1F15U,
        0x1F17U, 0x1F1DU,
        0x1F1FU, 0x1F45U,
        0x1F47U, 0x1F4DU,
        0x1F4FU, 0x1F57U,
        0x1F58U, 0x1F59U,
        0x1F5AU, 0x1F5BU,
        0x1F5CU, 0x1F5DU,
        0x1F5EU, 0x1F7DU,
        0x1F7FU, 0x1FB4U,
        0x1FB5U, 0x1FBCU,
        0x1FBDU, 0x1FBEU,
        0x1FC1U, 0x1FC4U,
        0x1FC5U, 0x1FCCU,
        0x1FCFU, 0x1FD3U,
        0x1FD5U, 0x1FDBU,
        0x1FDFU, 0x1FECU,
        0x1FF1U, 0x1FF4U,
        0x1FF5U, 0x1FFCU,
        0x2125U, 0x2126U,
        0x2129U, 0x212BU,
        0x212DU, 0x212EU,
        0x217FU, 0x2182U,
        0x3040U, 0x3094U,
        0x30A0U, 0x30FAU,
        0x3104U, 0x312CU,
        0xABFFU, 0xD7A3U
    };
    return (adobe::lower_bound(table, c) - boost::begin(table)) & 0x01;
}

bool in_ideographic(code_point_t c)
{
    static const code_point_t table[] =
    {
        0x4DFFU, 0x9FA5U,
        0x3006U, 0x3007U,
        0x3020U, 0x3029U
    };

    return (adobe::lower_bound(table, c) - boost::begin(table)) & 0x01;
}

bool in_combining_char(code_point_t c)
{
    static const code_point_t table[] =
    {
        0x02FFU, 0x0345U,
        0x035FU, 0x0361U,
        0x0482U, 0x0486U,
        0x0590U, 0x05A1U,
        0x05A2U, 0x05B9U,
        0x05BAU, 0x05BDU,
        0x05BEU, 0x05BFU,
        0x05C0U, 0x05C2U,
        0x05C3U, 0x05C4U,
        0x064AU, 0x0652U,
        0x066FU, 0x0670U,
        0x06D5U, 0x06DCU,
        0x06DCU, 0x06DFU,
        0x06DFU, 0x06E4U,
        0x06E6U, 0x06E8U,
        0x06E9U, 0x06EDU,
        0x0900U, 0x0903U,
        0x093BU, 0x093CU,
        0x093DU, 0x094CU,
        0x094CU, 0x094DU,
        0x0950U, 0x0954U,
        0x0961U, 0x0963U,
        0x0980U, 0x0983U,
        0x09BBU, 0x09BCU,
        0x09BDU, 0x09BEU,
        0x09BEU, 0x09BFU,
        0x09BFU, 0x09C4U,
        0x09C6U, 0x09C8U,
        0x09CAU, 0x09CDU,
        0x09D6U, 0x09D7U,
        0x09E1U, 0x09E3U,
        0x0A01U, 0x0A02U,
        0x0A3BU, 0x0A3CU,
        0x0A3DU, 0x0A3EU,
        0x0A3EU, 0x0A3FU,
        0x0A3FU, 0x0A42U,
        0x0A46U, 0x0A48U,
        0x0A4AU, 0x0A4DU,
        0x0A6FU, 0x0A71U,
        0x0A80U, 0x0A83U,
        0x0ABBU, 0x0ABCU,
        0x0ABDU, 0x0AC5U,
        0x0AC6U, 0x0AC9U,
        0x0ACAU, 0x0ACDU,
        0x0B00U, 0x0B03U,
        0x0B3BU, 0x0B3CU,
        0x0B3DU, 0x0B43U,
        0x0B46U, 0x0B48U,
        0x0B4AU, 0x0B4DU,
        0x0B55U, 0x0B57U,
        0x0B81U, 0x0B83U,
        0x0BBDU, 0x0BC2U,
        0x0BC5U, 0x0BC8U,
        0x0BC9U, 0x0BCDU,
        0x0BD6U, 0x0BD7U,
        0x0C00U, 0x0C03U,
        0x0C3DU, 0x0C44U,
        0x0C45U, 0x0C48U,
        0x0C49U, 0x0C4DU,
        0x0C54U, 0x0C56U,
        0x0C81U, 0x0C83U,
        0x0CBDU, 0x0CC4U,
        0x0CC5U, 0x0CC8U,
        0x0CC9U, 0x0CCDU,
        0x0CD4U, 0x0CD6U,
        0x0D01U, 0x0D03U,
        0x0D3DU, 0x0D43U,
        0x0D45U, 0x0D48U,
        0x0D49U, 0x0D4DU,
        0x0D56U, 0x0D57U,
        0x0E30U, 0x0E31U,
        0x0E33U, 0x0E3AU,
        0x0E46U, 0x0E4EU,
        0x0EB0U, 0x0EB1U,
        0x0EB3U, 0x0EB9U,
        0x0EBAU, 0x0EBCU,
        0x0EC7U, 0x0ECDU,
        0x0F17U, 0x0F19U,
        0x0F34U, 0x0F35U,
        0x0F36U, 0x0F37U,
        0x0F38U, 0x0F39U,
        0x0F3DU, 0x0F3EU,
        0x0F3EU, 0x0F3FU,
        0x0F70U, 0x0F84U,
        0x0F85U, 0x0F8BU,
        0x0F8FU, 0x0F95U,
        0x0F96U, 0x0F97U,
        0x0F98U, 0x0FADU,
        0x0FB0U, 0x0FB7U,
        0x0FB8U, 0x0FB9U,
        0x20CFU, 0x20DCU,
        0x20E0U, 0x20E1U,
        0x3029U, 0x302FU,
        0x3098U, 0x3099U,
        0x3099U, 0x309AU
    };

    return (adobe::lower_bound(table, c) - boost::begin(table)) & 0x01;
}

bool in_digit(code_point_t c)
{
    static const code_point_t table[] =
    {
        0x002FU, 0x0039U,
        0x065FU, 0x0669U,
        0x06EFU, 0x06F9U,
        0x0965U, 0x096FU,
        0x09E5U, 0x09EFU,
        0x0A65U, 0x0A6FU,
        0x0AE5U, 0x0AEFU,
        0x0B65U, 0x0B6FU,
        0x0BE6U, 0x0BEFU,
        0x0C65U, 0x0C6FU,
        0x0CE5U, 0x0CEFU,
        0x0D65U, 0x0D6FU,
        0x0E4FU, 0x0E59U,
        0x0ECFU, 0x0ED9U,
        0x0F1FU, 0x0F29U
    };

    return (adobe::lower_bound(table, c) - boost::begin(table)) & 0x01;
}

bool in_extender(code_point_t c)
{
    static const code_point_t table[] =
    {
        0x00B7U, 0x00B7U,
        0x02D0U, 0x02D0U,
        0x02D1U, 0x02D1U,
        0x0387U, 0x0387U,
        0x0640U, 0x0640U,
        0x0E46U, 0x0E46U,
        0x0EC6U, 0x0EC6U,
        0x3005U, 0x3005U,
        0x3031U, 0x3035U,
        0x309DU, 0x309EU,
        0x30FCU, 0x30FEU,
    };

    return (adobe::lower_bound(table, c) - boost::begin(table)) & 0x01;
}

} // namespace adobe_example

// ----

#include <adobe/vector.hpp>
#include <adobe/algorithm/copy.hpp>
#include <iterator>

int main()
{
    using namespace adobe_example;
    using namespace adobe;
    using namespace boost;

    {
    unsigned char test[] = "_valid_id_0";
    vector<code_point_t> v;
    copy(test, std::back_inserter(v)); v.pop_back();
    
    std::cout << test << " -> " << valid_name_unqualified_unreserved(begin(v), end(v)) << std::endl;
    }

    {
    unsigned char test[] = "not:valid";
    vector<code_point_t> v;
    copy(test, std::back_inserter(v)); v.pop_back();
    std::cout << test << " -> " << valid_name_unqualified_unreserved(begin(v), end(v)) << std::endl;
    }

    {
    unsigned char test[] = "xmLvalid";
    vector<code_point_t> v;
    copy(test, std::back_inserter(v)); v.pop_back();
    std::cout << test << " -> " << valid_name_unqualified_unreserved(begin(v), end(v)) << std::endl;
    }

    {
    unsigned char test[] = "xm";
    vector<code_point_t> v;
    copy(test, std::back_inserter(v)); v.pop_back();
    std::cout << test << " -> " << valid_name_unqualified_unreserved(begin(v), end(v)) << std::endl;
    }
}

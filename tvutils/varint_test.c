/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <stdio.h>
#include <tvutils/tvutils.h>

char                buffer[1024];
tvu_permhashtable_t *table = (tvu_permhashtable_t *)buffer;

int main(int argc __attribute__((unused)), char *argv[])
{
    varint_t *e = tvu_varint_alloca(64);
    varint_t *a = tvu_varint_alloca(64);
    varint_t *b = tvu_varint_alloca(64);
    varint_t *br = tvu_varint_alloca(64);
    varint_t *c = tvu_varint_alloca(64);

    varint_t *result = tvu_varint_alloca(255);
    varint_t *expect = tvu_varint_alloca(255);

    TVU_UNITTEST_INIT
    tvu_init(argv[0]);

    tvu_varint_from_hex(result, "9 12345678 11223344 55667788 11112222 33334444 55556666 77778888");
    TVU_UNITTEST_CMP(result->nr_digits, 4);
    TVU_UNITTEST_CMP(result->v[0], 0x5555666677778888);
    TVU_UNITTEST_CMP(result->v[1], 0x1111222233334444);
    TVU_UNITTEST_CMP(result->v[2], 0x1122334455667788);
    TVU_UNITTEST_CMP(result->v[3], 0x0000000912345678);

    tvu_varint_from_hex(
        a,
        "64CFCAC5 F1872E51 B1F9FBB5 BF16FBE7 9689FC09 03A801E3 D4802FB8 D329550D"
        "C8C9D3D9 22EECE9A 5475DB33 DB7B83BB 5C0E13D1 68049BBC 86C58176 47B088D1"
        "EAC9BE0B 31EB3185 40E4069D 556E9DD0 9D5D89D7 DE4A75C8 8BB49316 C106E4E0"
        "14B636E6 0FEBC292 E6249105 F5B195FE 906EEF7D 26C90A17 477122CE 125FB665"
    );
    
    tvu_varint_from_hex(
        b,
        "ffffffff ffffffff ffffffff ffffffff eeeeeeee eeeeeeee eeeeeeee eeeeeeee"
        "dddddddd dddddddd cccccccc cccccccc bbbbbbbb bbbbbbbb aaaaaaaa aaaaaaaa"
        "99999999 88888888 77777777 66666666 55555555 44444444 33333333 22222222"
        "11110000 ffffeeee ddddcccc bbbbaaaa 99998888 77776666 55554444 33332222"
    );

    tvu_varint_from_hex(
        br,
        "1"
        "00000000 00000000 00000000 00000000 11111111 11111111 11111111 11111111"
        "23456789 ABCDF012 456789AB CDF01234 69E3A1A3 EA754457 C2A2C72F DCCE037D"
        "4FF3F1D2 2E6D27E7 496DFD80 909424CB 26B5C6DA 95B8C683 DF0E2B62 0174E9CF"
        "C1135B20 C2FACAF4 AA83A4DF 4FE4EA01 A7C24032 5CC5122A 0A28F85D B9C777A5"
    );
    br->scale_nr_digits = (1024 / 64);

    tvu_varint_from_hex(
        c,
        "64CFCAC5 F1872E51 B1F9FBB5 FFFFFFFF 8FD1770C E27CDCAB 2F091EF0 F99F4442"
        "95E5F9CB 56DA9830 55D75352 FFFFFFFF CBA5B475 D12335B0 45491E6E B88FCCC2"
        "8A57E2B7 6E3D255E 50662357 FFFFFFFF 4F1018F3 8C59914C 509147CD 59C5B9AA"
        "E8E0FD01 200AF4BE EA1F6ECD FFFFFFFF 923F74E4 794B3790 745274EB 4B4BC7B4"
        "A4DDFE04 19DA4A22 F28A2A3A FFFFFFFF E282906C D83F9295 7034BCE5 DB140EE1"
        "BB60E2AB 8BBACBBB C67D944A FFFFFFFF 4072E2CF 3644802B 5D43C4AE EDD49391"
        "3C9BED95 98B9F6FA F2ACFE94 FFFFFFFF 050CFA9E 6539D73D 76707588 1DFBF539"
        "45198CC1 BF64F8CF F2588FE5 FFFFFFFF D61DEA79 B27CFB40 E6C4FF0A 9C0EA36A"
    );

    // Test accumilator add.
    tvu_varint_copy(result, a);
    tvu_varint_iadd(result, b);

    tvu_varint_from_hex(
        expect,
        "1"
        "64CFCAC5 F1872E51 B1F9FBB5 BF16FBE7 8578EAF7 F296F0D2 C36F1EA7 C21843FC"
        "A6A7B1B7 00CCAC78 2142A800 A8485088 17C9CF8D 23C05778 31702C20 F25B337C"
        "846357A4 BA73BA0D B85B7E14 BBD50436 F2B2DF2D 228EBA0C BEE7C649 E3290702"
        "25C736E7 0FEBB181 C4025DD2 B16D40A9 2A087805 9E40707D 9CC66712 4592D887"
    );
    TVU_UNITTEST_VARINTCMP(result, expect);
   
    // Test accumilator sub.
    tvu_varint_copy(result, b);
    tvu_varint_isub(result, a);

    tvu_varint_from_hex(
        expect,
        "9B30353A 0E78D1AE 4E06044A 40E90418 5864F2E5 EB46ED0B 1A6EBF36 1BC599E1"
        "15140A04 BAEF0F43 7856F198 F1514911 5FADA7EA 53B71FFF 23E52934 62FA21D8"
        "AECFDB8E 569D5703 369370DA 10F7C895 B7F7CB7D 65F9CE7B A77EA01C 611B3D41"
        "FC5AC91A F0142C5B F7B93BC6 C60A14AC 092A990B 50AE5C4F 0DE42176 20D36BBD"
    );
    TVU_UNITTEST_VARINTCMP(result, expect);
    
    // Test accumilator sub.
    tvu_varint_copy(result, a);
    tvu_varint_irshift(result, 4);

    tvu_varint_from_hex(
        expect,
        "0 64CFCAC5 F1872E51 B1F9FBB5 BF16FBE7 9689FC09 03A801E3 D4802FB8 D329550D"
        "C8C9D3D9 22EECE9A 5475DB33 DB7B83BB 5C0E13D1 68049BBC 86C58176 47B088D1"
        "EAC9BE0B 31EB3185 40E4069D 556E9DD0 9D5D89D7 DE4A75C8 8BB49316 C106E4E0"
        "14B636E6 0FEBC292 E6249105 F5B195FE 906EEF7D 26C90A17 477122CE 125FB66"  // Removed one nibble here.
    );
    TVU_UNITTEST_VARINTCMP(result, expect);
    
    // Test fused multiply-add single digit
    tvu_varint_from_int(result, 0);
    tvu_varint_ifma_digit(result, a, 0xfedcba9876543210, 0);

    tvu_varint_from_hex(
        expect,
        "645D173D AB0CD681"
        "2594BBA4 4B6F005A BB8EA1DD 02700197 20409C74 60B486DF 58B6BD44 E3496E78"
        "7DAB81C1 5C6C16F5 49C2333F 1D4765D5 87079EBD 88CB625C AF0FC78B 05DECEF0"
        "E2EF153C B82CEE00 FC4D7AE8 EC7AF489 1ECEE754 A70431A3 63F57209 EDCA34F8"
        "A6E09448 5AFF0076 3E4EE8CE F0678C2D D43D5B80 B8E3E980 601A3FEA 3EBF2050"
    );
    TVU_UNITTEST_VARINTCMP(result, expect);
    
    // Test fused multiply-add
    tvu_varint_from_int(result, 0);
    tvu_varint_ifma(result, a, b);

    tvu_varint_from_hex(
        expect,
        "64CFCAC5 F1872E51 B1F9FBB5 BF16FBE7 8FD1770C E27CDCAB 2F091EF0 F99F4442"
        "95E5F9CB 56DA9830 55D75352 7D5D25E9 CBA5B475 D12335B0 45491E6E B88FCCC2"
        "8A57E2B7 6E3D255E 50662357 CEBE573A 4F1018F3 8C59914C 509147CD 59C5B9AA"
        "E8E0FD01 200AF4BE EA1F6ECD A147E55A 923F74E4 794B3790 745274EB 4B4BC7B4"
        "A4DDFE04 19DA4A22 F28A2A3A 902B468E E282906C D83F9295 7034BCE5 DB140EE1"
        "BB60E2AB 8BBACBBB C67D944A 86216E4B 4072E2CF 3644802B 5D43C4AE EDD49391"
        "3C9BED95 98B9F6FA F2ACFE94 263DE89A 050CFA9E 6539D73D 76707588 1DFBF539"
        "45198CC1 BF64F8CF F2588FE5 92DA877A D61DEA79 B27CFB40 E6C4FF0A 9C0EA36A"
    );
    TVU_UNITTEST_VARINTCMP(result, expect);

    // Test modulus.
    tvu_varint_mod(result, c, b, br);
    tvu_varint_from_hex(
        expect,
        "4A908338 6B1A94BE 230DA281 7500AEED 839EFBE7 B2E16AF4 8F862150 6A0558C7"
        "9C1F86BE B711EF60 9E1EC71F 3B037D62 2EFEF61B 53D264CB 0611AA9D D14036E9"
        "FD5A370E 0EEFA6A2 208F0355 1095443E 93B97EDE 52BC8F9C 6AAB6FAD 2ADBCC4D"
        "0EBE194A 6C805A75 B93B93D9 94DE95D6 89924C0A 8D81CD9F 254AC236 98D9E086"
    );
    TVU_UNITTEST_VARINTCMP(result, expect);
    

    return 0;
}

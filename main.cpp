#include <iostream>
#include <stdio.h>

#include "reader.h"
#include "fixnum.h"
#include "special.h"


int main(int argc, char **argv) {
	int64_t zero, one, n_one, big, n_big;
	value_t wzero, wone, wn_one, wbig, wn_big;
	int64_t uzero, uone, un_one, ubig, un_big;

	zero = 0;
	wzero = wrap_fixnum(zero);
	uzero = unwrap_fixnum(wzero);

	one = 1;
	wone = wrap_fixnum(one);
	uone = unwrap_fixnum(wone);

	n_one = -1;
	wn_one = wrap_fixnum(n_one);
	un_one = unwrap_fixnum(wn_one);

	big = 0x03FFFFFFFFFFFFFF;
	wbig = wrap_fixnum(big);
	ubig = unwrap_fixnum(wbig);

	n_big = -big -1;
	wn_big = wrap_fixnum(n_big);
	un_big = unwrap_fixnum(wn_big);
	
	std::cout << zero << " " << wzero <<   " " << std::endl << uzero << std::endl << std::endl;
	std::cout << one << " " << wone <<   " "   << std::endl << uone << std::endl  << std::endl;
	std::cout << n_one << " " << wn_one << " " << std::endl << un_one << std::endl << std::endl;
	std::cout << big << " " << wbig << " "     << std::endl << ubig << std::endl  << std::endl;
	std::cout << n_big << " " << wn_big << " " << std::endl << un_big << std::endl << std::endl;
	
	std::cout << std::endl;
	value_t fixnum, rewrapped;
	int64_t unwrapped;

	fixnum = fixnum_make_from_string("288230376151711743");
	unwrapped = unwrap_fixnum(fixnum);
	rewrapped = wrap_fixnum(unwrapped);
	std::string format = fixnum_format(fixnum);
	std::cout << fixnum << " " << unwrapped << " " << rewrapped << " " << format << std::endl;

	std::cout << std::hex << wrap_boolean(true) << " " <<
		         unwrap_boolean(wrap_boolean(true)) << std::endl;
	std::cout << std::hex << wrap_boolean(false) << " " <<
	             unwrap_boolean(wrap_boolean(false)) << std::endl;

	return 0;
}

string convertToBase(ll number, int base) {
	if (base < 2 || base > 36) {
		throw invalid_argument("Base must be in the range 2-36.");
	}
	if (number == 0) {
		return "0";
	}
  string result;
	bool isNegative = (number < 0);
	if (isNegative) {
		number = -number;
	}
	while (number > 0) {
		int remainder = number % base;
		if (remainder < 10) {
			result += '0' + remainder;
		} else {
			result += 'A' + (remainder - 10);
		}
		number /= base;
	}
	if (isNegative) {
		result += '-';
	}
	reverse(result.begin(), result.end());
	return result;
}

function power(base, exp) {
    if (exp == 0) {
        return 1;
    } else {
        return base * power(base, exp - 1);
    }
}

function main() {
    return power(2, 8);  // 2^8 = 256
}

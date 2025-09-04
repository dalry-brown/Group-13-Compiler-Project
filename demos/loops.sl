function sum_to_n(n) {
    var total = 0;
    var i = 1;
    while (i <= n) {
        total = total + i;
        i = i + 1;
    }
    return total;
}

function main() {
    return sum_to_n(10);
}

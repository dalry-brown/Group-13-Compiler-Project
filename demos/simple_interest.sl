function calculate_simple_interest(principal, rate, time) {
    var interest = (principal * rate * time) / 100;
    return interest;
}

function calculate_total_amount(principal, rate, time) {
    var interest = calculate_simple_interest(principal, rate, time);
    var total = principal + interest;
    return total;
}

function main() {
    // Example: $1000 at 5% for 3 years
    var principal = 1000;
    var rate = 5;
    var time = 3;
    
    var simple_interest = calculate_simple_interest(principal, rate, time);
    var total_amount = calculate_total_amount(principal, rate, time);
    
    return simple_interest;
}
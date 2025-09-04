#!/bin/bash
echo "=== SimpleLang Compiler Test Suite ==="
echo

cd ..  # Go to project root

echo "Test 1: Factorial (5! = 120)"
result=$(./build/simplelang -r demos/factorial.sl | grep "Return value:" | cut -d' ' -f3)
if [ "$result" = "120" ]; then
    echo "✅ PASS"
else
    echo "❌ FAIL (got $result)"
fi

echo "Test 2: Fibonacci (fib(10) = 55)"
result=$(./build/simplelang -r demos/fibonacci.sl | grep "Return value:" | cut -d' ' -f3)
if [ "$result" = "55" ]; then
    echo "✅ PASS"
else
    echo "❌ FAIL (got $result)"
fi

echo "Test 3: Simple Interest (SI = 150)"
result=$(./build/simplelang -r demos/simple_interest.sl | grep "Return value:" | cut -d' ' -f3)
if [ "$result" = "150" ]; then
    echo "✅ PASS"
else
    echo "❌ FAIL (got $result)"
fi

echo "Test 4: Loops - Sum 1 to 10 (sum = 55)"
result=$(./build/simplelang -r demos/loops.sl | grep "Return value:" | cut -d' ' -f3)
if [ "$result" = "55" ]; then
    echo "✅ PASS"
else
    echo "❌ FAIL (got $result)"
fi

echo "Test 5: Mathematics - Power (2^8 = 256)"
result=$(./build/simplelang -r demos/mathematics.sl | grep "Return value:" | cut -d' ' -f3)
if [ "$result" = "256" ]; then
    echo "✅ PASS"
else
    echo "❌ FAIL (got $result)"
fi

echo
echo "=== Test Summary ==="
total_tests=5
echo "Total tests: $total_tests"
echo "All tests completed!"

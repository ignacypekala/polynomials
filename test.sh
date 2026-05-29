make polynomials > /dev/null

passed=0
failed=0
for batch in tests/* ; do
    for input_file in $batch/*.in ; do
        base_path=${input_file%.in}
        test_name=${base_path#tests/$batch/}
        output_file=$base_path.out
        if ! diff <(./polynomials < $input_file) $output_file -u --color; then
            echo "Test $test_name failed"
            ((failed++))
        else
            ((passed++))
        fi
    done
done
echo Summary: $passed passed, $failed failed.

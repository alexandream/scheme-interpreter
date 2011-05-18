

all:
	@./configure
	@make --no-print-directory -f .makefile

clean:
	@./configure
	@make --no-print-directory -f .makefile clean

test:
	@make --no-print-directory clean > /dev/null
	@echo "Building project..."
	@make --no-print-directory > /dev/null
	@echo "Running tests.\n"
	@find tests -type f -exec ./tester '{}' %% ./main ';'
	@echo ""
	@make --no-print-directory clean > /dev/null



all:
	@./configure
	@make --no-print-directory -f .makefile

clean:
	@./configure
	@make --no-print-directory -f .makefile clean

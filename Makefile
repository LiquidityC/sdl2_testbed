all:
	@ninja -C build
.PHONY: all

clean:
	@ninja -C build clean
.PHONY: clean

run: all
	@./build/testbed

setup:
	@mkdir -p build
	@cd build && \
		cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. && \
		cd -
	@ln -s build/compile_commands.json
.PHONY: setup

teardown:
	@rm -rf build
	@rm -f compile_commands.json
.PHONY: teardown

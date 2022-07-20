CXX=g++
CFLAGS=-Wall -std=c++17 -g -fPIC -lm -DLDL_LONG -DDLONG

HEADER=${wildcard include/*.h include/impl/*.h include/impl/utils/*.h}

SOURCE=${wildcard src/*.cpp}
SOURCE_OBJ=${patsubst src/%.cpp,build/%.o,${SOURCE}}

TEST=${wildcard test/*.cpp}
TEST_BIN=${patsubst test/%.cpp,bin/%,${TEST}}

INCLUDES=\
	-I include \
	-I external/ecos/include \
	-I external/ecos/external/SuiteSparse_config

INCLUDES_TEST=\
	${INCLUDES} \
	-I external/libcester/include \
	-I .

.PHONY: test
test: ${TEST_BIN}
	./bin/bonsai_ecos_test

bin/%: test/%.cpp external/ecos/libecos.a lib/libbonsai.a
	mkdir -p ${dir $@}
	${CXX} -o $@ $^ ${CFLAGS} ${INCLUDES_TEST}

build/%.o: src/%.cpp ${HEADER}
	mkdir -p ${dir $@}
	${CXX} -o $@ $< -c ${CFLAGS} ${INCLUDES}

lib/libbonsai.a: ${SOURCE_OBJ}
	mkdir -p lib
	ar cr $@ $^

external/ecos/libecos.a:
	make -C external/ecos libecos.a

.PHONY: clean
clean:
	make -C external/ecos clean
	rm -r build lib bin || true

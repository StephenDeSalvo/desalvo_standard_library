# Build and run the unit tests.
#
#   make          build and run everything
#   make build    build only
#   make clean
#
# Catch2 2.6.1 is vendored in Catch2/. It needs
# CATCH_CONFIG_NO_POSIX_SIGNALS on current glibc, which unit_tests.cpp
# defines; see the note there.

CXX      ?= g++
CXXFLAGS ?= -std=c++14 -O1 -Wall
# -iquote, not -I: a plain -I on the parent directory would put the library
# ahead of the system headers for angle-bracket includes too.
INCLUDES  = -iquote .. -iquote .

SOURCES = unit_tests.cpp integer_partition_tests.cpp asep_tests.cpp
TARGET  = dsl_tests

.PHONY: all build clean
all: build
	./$(TARGET)

build: $(TARGET)

$(TARGET): $(SOURCES) $(wildcard ../*.h) $(wildcard ../polymorphic/*.h)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SOURCES)

clean:
	rm -f $(TARGET)

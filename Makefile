SHELL = /bin/bash

ifndef .VERBOSE
  .SILENT:
endif

define Compile
printf "%b" "\033[0;97mCompiling $(1)..."
gcc -I/FPT/HeadersC/ -I/FPT/Source/ /FPT/DotosC/*.o $(2) -lm -lX11 -lXext -o $(1) 2> $@.log;\
  RESULT=$$?;\
  if [ $$RESULT -ne 0 ]; then \
    printf "%b\n" "\033[0;31m\xE2\x9D\x8C\033[0m";\
  else \
    printf "%b\n" "\033[0;32m\xE2\x9C\x94\033[0m";\
  fi;\
  cat $@.log;\
  rm -f $@.log
endef

define Clean
printf "%b" "\033[0;97mRemoving $(1)..."
rm $(1)
printf "%b\n" "\033[0;32m\xE2\x9C\x94\033[0m"
endef

all: lab_2.a
	printf "%b\n" "\033[0;32mCompiled all projects\033[0m"

clean:
	$(call Clean,lab_2.a)
	printf "%b\n" "\033[0;33mRemoved all projects\033[0m"

lab_2.a: lab_2.c matrix_math.c
	$(call Compile,$@,$^)

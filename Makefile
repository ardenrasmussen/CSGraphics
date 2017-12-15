SHELL = /bin/bash

ifndef .VERBOSE
  .SILENT:
endif

define New_Compile
printf "%b" "\033[0;97mCompiling $(1)..."
gcc $(2) -lm -lFPT -lSDL2 -lSDL2_ttf -lSDL2_image -o $(1) 2> $@.log;\
  RESULT=$$?;\
  if [ $$RESULT -ne 0 ]; then \
    printf "%b\n" "\033[0;31m\xE2\x9D\x8C\033[0m";\
  else \
    printf "%b\n" "\033[0;32m\xE2\x9C\x94\033[0m";\
  fi;\
  cat $@.log;\
  rm -f $@.log
endef

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

all: lab_1.a lab_2.a lab_3.a lab_4.a lab_5.a lab_6.a lab_9.a lab_10.a practice_midterm.a
	printf "%b\n" "\033[0;32mCompiled all projects\033[0m"

clean:
	$(call Clean,lab_1.a)
	$(call Clean,lab_2.a)
	$(call Clean,lab_3.a)
	$(call Clean,lab_4.a)
	$(call Clean,lab_5.a)
	$(call Clean,lab_6.a)
	$(call Clean,lab_9.a)
	$(call Clean,lab_10.a)
	$(call Clean,practice_midterm.a)
	printf "%b\n" "\033[0;33mRemoved all projects\033[0m"

lab_1.a: lab_1.c
	$(call Compile,$@,$^)

lab_2.a: lab_2.c matrix_math.c
	$(call Compile,$@,$^)

lab_3.a: lab_3.c matrix_math.c
	$(call Compile,$@,$^)

lab_4.a: lab_4.c matrix_math.c
	$(call Compile,$@,$^)

lab_5.a: lab_5.c matrix_math.c
	$(call Compile,$@,$^)

lab_6.a: lab_6.c matrix_math.c
	$(call Compile,$@,$^)

lab_9.a: lab_9.c matrix_math.c base.c
	$(call Compile,$@,$^)

lab_10.a: lab_10.c matrix_math.c base.c
	$(call Compile,$@,$^)

practice_midterm.a: practice_midterm.c matrix_math.c
	$(call Compile,$@,$^)

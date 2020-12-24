#======== FORMAT ================

#target [target...] : [dependent ....]
#[ command ...]




#======== BUIL ===================
seprate = 0
INCLUDES = -I "./include"

ifeq ($(seprate),0)	
hello: main.c hello.c
	@echo "Build at 1 time"
	$(CC) $(CFLAGS) $(INCLUDES) $? $(LDFLAGS) -o $@
else
# On below target (hello), dependents (hello.o, main.o) need to be made first-> you have a recursion.
hello: hello.o main.o 
	@echo "Build seprate"
	$(CC) $(INCLUDES) $? -o $@

hello.o: hello.c
	$(CC) $(INCLUDES) -c $<

main.o: main.c
	$(CC) $(INCLUDES) -c $<
# or 
#main.o: main.c hello.h
#	$(CC) -c $<
endif


#========= CLEAN =================
.PHONY: clean install
clean:
	rm -rf hello *.o

install:
	@echo "You have just called make install"


#========= NOTE ==================

#Default macros
# $@ represents target ex: hello
# $? picks up dependence files (all the changed source files.)
# $< picks up 1 src file.
# $(CC), $(CFLAGS), $(LDFLAGS),......

# .PHONY declare "target" follow is NOT a file. ( can use as Making multiple targets https://makefiletutorial.com/)
#(https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html)

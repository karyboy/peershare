UNAME = $(shell uname)
ifeq ($(UNAME), SunOS) # Sun OS
MY_LIBS = -lresolv -lsocket -lnsl
endif
ifeq ($(UNAME), Linux) # Linux
MY_LIBS = -lresolv -lnsl -lpthread
endif
ifeq ($(UNAME), Darwin) # Mac OS
MY_LIBS =
endif

CC := g++

all: 
	${CC} karneshm_proj1.cpp -o karneshm_proj1

##==========================================================================
clean:
	@- $(RM) karneshm_proj1
	@- echo “Data Cleansing Done.Ready to Compile”

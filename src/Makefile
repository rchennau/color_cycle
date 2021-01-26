include ataricc65.mak

target = color.xex
objects= \
	color.o \
	text.s

$(target) : $(objects)
	$(CL) -t $(SYS) -o $@ $(objects) atari.lib -ld-ags -D,__RESERVED_MEMORY__=15339
	$(CP) $@ $(BUILD)/$@
	$(RM) $@

all: $(target)

clean: 
#	$(RM) -f *.tgi
#	$(RM) -f *.s
#	$(RM) -f *.joy
	$(RM) -f *.o
	$(RM) -f $(BUILD)/*.*
	$(RM) -f *.xex

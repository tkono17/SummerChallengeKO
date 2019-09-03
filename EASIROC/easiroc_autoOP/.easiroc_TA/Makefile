OBJS = class_FileManager.o class_ParameterManager.o class_StringManager.o test.o ConnectSiTCP.o
CCR = $(CXX) -g -Wall -O3

easiroc: $(OBJS)
	$(CCR) -o $@ $(OBJS)

.cc.o:
	$(CCR) -c $< -o $@

clean:
	$(RM) $(OBJS) easiroc

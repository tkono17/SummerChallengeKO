OBJS = UDPcontrol.o SiTCP.o sender.o
CCR = $(CXX) -g -Wall -O3

udp102: $(OBJS)
	$(CCR) -o $@ $(OBJS)

.cc.o:
	$(CCR) -c $< -o $@

clean:
	$(RM) $(OBJS) udp102

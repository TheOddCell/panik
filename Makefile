obj-m += panik.o
KERN ?= $(shell uname -r)

all: panik.ko
panik.ko: git.h
	@if [ -z "$(KERN)" ]; then \
		echo "please set the KERN environment variable" >&2; \
		exit 1; \
	fi
	$(MAKE) -C /lib/modules/$(KERN)/build M=$(PWD) modules
git.h:
	@echo "#define GIT_VERSION \"$(shell git rev-parse --abbrev-ref HEAD)/$(shell git rev-parse --short HEAD)\"" > git.h

testimveryverysureiwanttocrashthekernelimverysurethatiwanttodothisokbye: all
	printf 'hi you told make to do this, now reboot'>/tmp/panik
	insmod ./panik.ko
clean:
	@rm ..module-common.o.cmd .module-common.o .Module.symvers.cmd .modules.order.cmd .panik.ko.cmd .panik.mod.cmd .panik.mod.o.cmd .panik.o.cmd Module.symvers modules.order panik.ko panik.mod.c panik.o panik.mod panik.mod.o git.h 2>/dev/null | true

.PHONY: all testimveryverysureiwanttocrashthekernelimverysurethatiwanttodothisokbye git.h

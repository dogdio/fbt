
# export HOGE_ENV=hoge
ifeq ($(HOGE_ENV), hoge)
 HOGE3 = "hoge3"
else

endif

HOGE4 = "hoge4"
HOGE5 = "hoge5"

HOGE1 = $(LANG) "hoge"
HOGE2 := $(LANG) "hoge"
HOGE3 ?= $(LANG) "hoge"
HOGE4 += $(LANG) "hoge"

#HOGE5 = $(HOGE5) "hoge" # NG
HOGE5 := $(HOGE5) "hoge"

LANG = "en"

.PHONY: all #all というファイルがあっても大丈夫
all:
	@echo "1. =  $(HOGE1)"
	@echo "2. := $(HOGE2)"
	@echo "3. ?= $(HOGE3)"
	@echo "4. += $(HOGE4)"
	@echo "5. := $(HOGE5)"


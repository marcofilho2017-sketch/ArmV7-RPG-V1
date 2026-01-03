# --------------------------------------------------------------------------
# Toolchain
# --------------------------------------------------------------------------
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

# --------------------------------------------------------------------------
# Diretórios
# --------------------------------------------------------------------------
SRC_DIR     = src
DRIVERS_DIR = drivers
LIBS_DIR    = libs
BUILD_DIR   = build
IMG_DIR     = img

# --------------------------------------------------------------------------
# Flags de compilação
# --------------------------------------------------------------------------
CFLAGS  = -c -ffreestanding -O2 -mcpu=cortex-a9 -marm
CFLAGS += -I./drivers -I./libs -I./src -I./build/campaign_data
CFLAGS += $(shell find ./src -type d -printf '-I%p ')

ASFLAGS = -c -mcpu=cortex-a9 -marm
LDFLAGS = -nostdlib -nostartfiles -T build_arm.ld

# --------------------------------------------------------------------------
# Arquivos de Dialogo
# --------------------------------------------------------------------------
# Caminhos
DIALOGS_C     = $(BUILD_DIR)/campaign_data/dialogs.c
DIALOGS_H     = $(BUILD_DIR)/campaign_data/dialogs.h
DIALOGS_STAMP = $(BUILD_DIR)/campaign_data/dialogs.stamp

CHARACTERS_C     = $(BUILD_DIR)/campaign_data/characters.c
CHARACTERS_H     = $(BUILD_DIR)/campaign_data/characters.h
CHARACTERS_STAMP = $(BUILD_DIR)/campaign_data/characters.stamp

ITENS_C     = $(BUILD_DIR)/campaign_data/itens.c
ITENS_H     = $(BUILD_DIR)/campaign_data/itens.h
ITENS_STAMP = $(BUILD_DIR)/campaign_data/itens.stamp

SPELLS_C     = $(BUILD_DIR)/campaign_data/spells.c
SPELLS_H     = $(BUILD_DIR)/campaign_data/spells.h
SPELLS_STAMP = $(BUILD_DIR)/campaign_data/spells.stamp

SCENES_C     = $(BUILD_DIR)/campaign_data/scenes.c
SCENES_H     = $(BUILD_DIR)/campaign_data/scenes.h
SCENES_STAMP = $(BUILD_DIR)/campaign_data/scenes.stamp

QUESTS_C     = $(BUILD_DIR)/campaign_data/quests.c
QUESTS_H     = $(BUILD_DIR)/campaign_data/quests.h
QUESTS_STAMP = $(BUILD_DIR)/campaign_data/quests.stamp

CAMPAIGN_FILES = $(DIALOGS_H) $(DIALOGS_C) $(CHARACTERS_H) $(CHARACTERS_C) $(ITENS_H) $(ITENS_C) $(SPELLS_H) $(SPELLS_C) $(SCENES_H) $(SCENES_C)
# --------------------------------------------------------------------------
# Imagens BMP
# --------------------------------------------------------------------------
IMAGES      := $(wildcard $(IMG_DIR)/*.bmp)
IMAGES_OBJS := $(patsubst $(IMG_DIR)/%.bmp, $(BUILD_DIR)/%.bmp.o, $(IMAGES))

IMAGE_H = $(BUILD_DIR)/campaign_data/image.h
IMAGE_C = $(BUILD_DIR)/campaign_data/image.c

# --------------------------------------------------------------------------
# Buscar arquivos recursivamente dentro de src/
# --------------------------------------------------------------------------
SRC_C_SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
SRC_S_SRCS := $(shell find $(SRC_DIR) -type f -name '*.s')

# Manter drivers/ e libs/ não-recursivos
DRV_C_SRCS := $(wildcard $(DRIVERS_DIR)/*.c)
LIB_C_SRCS := $(wildcard $(LIBS_DIR)/*.c)

DRV_S_SRCS := $(wildcard $(DRIVERS_DIR)/*.s)
LIB_S_SRCS := $(wildcard $(LIBS_DIR)/*.s)

# Unir tudo
CSRCS := $(SRC_C_SRCS) $(DRV_C_SRCS) $(LIB_C_SRCS)
ASRCS := $(SRC_S_SRCS) $(DRV_S_SRCS) $(LIB_S_SRCS)

# --------------------------------------------------------------------------
# Gerar objetos (preservando estrutura de src/)
# --------------------------------------------------------------------------
SRC_C_OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC_C_SRCS:.c=.o))
SRC_S_OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC_S_SRCS:.s=.o))

# drivers e libs só recebem nome simples
DRV_C_OBJS := $(patsubst $(DRIVERS_DIR)/%, $(BUILD_DIR)/drivers/%, $(DRV_C_SRCS:.c=.o))
LIB_C_OBJS := $(patsubst $(LIBS_DIR)/%, $(BUILD_DIR)/libs/%, $(LIB_C_SRCS:.c=.o))

DRV_S_OBJS := $(patsubst $(DRIVERS_DIR)/%, $(BUILD_DIR)/drivers/%, $(DRV_S_SRCS:.s=.o))
LIB_S_OBJS := $(patsubst $(LIBS_DIR)/%, $(BUILD_DIR)/libs/%, $(LIB_S_SRCS:.s=.o))

CAMPAIGN_C_SRCS := $(wildcard $(BUILD_DIR)/campaign_data/*.c)
CAMPAIGN_OBJS   := $(patsubst $(BUILD_DIR)/%.c, $(BUILD_DIR)/%.o, $(CAMPAIGN_C_SRCS))


COBJS = $(SRC_C_OBJS) $(DRV_C_OBJS) $(LIB_C_OBJS) $(CAMPAIGN_OBJS)
AOBJS = $(SRC_S_OBJS) $(DRV_S_OBJS) $(LIB_S_OBJS)

OBJS = $(COBJS) $(AOBJS) $(IMAGES_OBJS) #$(BUILD_DIR)/image.o

# --------------------------------------------------------------------------
# Arquivos finais
# --------------------------------------------------------------------------
TARGET = $(BUILD_DIR)/firmware.elf
BIN    = $(BUILD_DIR)/firmware.bin

# --------------------------------------------------------------------------
# Build principal
# --------------------------------------------------------------------------
all: $(BUILD_DIR) $(CAMPAIGN_FILES) $(IMAGE_H) $(IMAGE_C) $(TARGET) $(BIN)

# Criar diretório build/
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --------------------------------------------------------------------------
# Dialog parsing (dialogs.c, dialogs.h gerados automaticamente)
# --------------------------------------------------------------------------

# Definir Dependencias dos seus stamps
# Regras finais dependem do stamp file
$(DIALOGS_C) $(DIALOGS_H): $(DIALOGS_STAMP)
$(ITENS_C) $(ITENS_H): $(ITENS_STAMP)
$(SPELLS_C) $(SPELLS_H): $(SPELLS_STAMP)
$(CHARACTERS_C) $(CHARACTERS_H): $(CHARACTERS_STAMP) $(SPELLS_STAMP) $(ITENS_STAMP)
$(QUESTS_C) $(QUESTS_H): $(QUESTS_STAMP) $(ITENS_STAMP) $(CHARACTERS_STAMP) $(DIALOGS_STAMP)
$(SCENES_C) $(SCENES_H): $(SCENES_STAMP) $(ITENS_STAMP) $(CHARACTERS_STAMP) $(DIALOGS_STAMP) $(IMAGE_H) $(IMAGE_C)

# O stamp depende do script e do diretório build
$(DIALOGS_STAMP): scripts/parse_dialog.py | $(BUILD_DIR)
	@echo "Gerando arquivos de dialogo..."
	python scripts/parse_dialog.py || python3 scripts/parse_dialog.py
	@touch $(DIALOGS_STAMP)

$(ITENS_STAMP): scripts/parse_itens.py | $(BUILD_DIR)
	@echo "Gerando arquivos de itens..."
	python scripts/parse_itens.py || python3 scripts/parse_itens.py
	@touch $(ITENS_STAMP)

$(SPELLS_STAMP): scripts/parse_spells.py | $(BUILD_DIR)
	@echo "Gerando arquivos de feiticos..."
	python scripts/parse_spells.py || python3 scripts/parse_spells.py
	@touch $(SPELLS_STAMP)

$(CHARACTERS_STAMP): scripts/parse_characters.py | $(BUILD_DIR)
	@echo "Gerando arquivos de personagens..."
	python scripts/parse_characters.py || python3 scripts/parse_characters.py
	@touch $(CHARACTERS_STAMP)

$(QUESTS_STAMP): scripts/parse_quests.py | $(BUILD_DIR)
	@echo "Gerando arquivos de Missoes..."
	python scripts/parse_quests.py || python3 scripts/parse_quests.py
	@touch $(QUESTS_STAMP)

$(SCENES_STAMP): scripts/parse_scenes.py | $(BUILD_DIR)
	@echo "Gerando arquivos de cenas..."
	python scripts/parse_scenes.py || python3 scripts/parse_scenes.py
	@touch $(SCENES_STAMP)

# Compilação do dialogs.o
$(BUILD_DIR)/dialogs.o: $(DIALOGS_C) $(DIALOGS_H)
	$(CC) $(CFLAGS) -o $@ $(DIALOGS_C)

# --------------------------------------------------------------------------
# Gerar image.h
# --------------------------------------------------------------------------
$(IMAGE_H): $(IMAGES)
	@echo "Gerando $@"
	@echo "#pragma once"                                  >  $@
	@echo "#include <stdint.h>"                          >> $@
	@echo                                                 >> $@
	@echo "typedef struct {"                             >> $@
	@echo "    const char* name;"                        >> $@
	@echo "    const uint8_t* start;"                    >> $@
	@echo "    const uint8_t* end;"                      >> $@
	@echo "    uint32_t size;"                           >> $@
	@echo "} Image;"                                     >> $@
	@echo                                                 >> $@
	@echo "typedef enum {"                               >> $@

	@for f in $(IMAGES); do \
		base=$$(basename $$f .bmp); \
		echo "    IMAGE_$${base}," >> $@; \
	done

	@echo "    IMAGE__COUNT"                             >> $@
	@echo "} ImageID;"                                   >> $@
	@echo                                                 >> $@
	@echo "extern const Image images[IMAGE__COUNT];"     >> $@

# --------------------------------------------------------------------------
# Gerar image.c
# --------------------------------------------------------------------------
$(IMAGE_C): $(IMAGES) $(IMAGE_H)
	@echo "Gerando $@"
	@echo "#include \"image.h\""              >  $@
	@echo                                     >> $@

	@for f in $(IMAGES); do \
		base=$$(basename $$f .bmp); \
		echo "extern const uint8_t _binary_img_$${base}_bmp_start[];" >> $@; \
		echo "extern const uint8_t _binary_img_$${base}_bmp_end[];"   >> $@; \
		echo "extern const uint8_t _binary_img_$${base}_bmp_size[];"  >> $@; \
		echo >> $@; \
	done

	@echo "const Image images[IMAGE__COUNT] = {" >> $@

	@for f in $(IMAGES); do \
		base=$$(basename $$f .bmp); \
		echo "    {"                                         >> $@; \
		echo "        .name = \"$${base}\","                 >> $@; \
		echo "        .start = _binary_img_$${base}_bmp_start," >> $@; \
		echo "        .end   = _binary_img_$${base}_bmp_end,"   >> $@; \
		echo "        .size  = (uint32_t)_binary_img_$${base}_bmp_size," >> $@; \
		echo "    },"                                       >> $@; \
	done

	@echo "};" >> $@

# --------------------------------------------------------------------------
# Compilar image.c
# --------------------------------------------------------------------------
$(BUILD_DIR)/image.o: $(IMAGE_C) $(IMAGE_H)
	$(CC) $(CFLAGS) -o $@ $(IMAGE_C)

# --------------------------------------------------------------------------
# Compilar C dentro de src/ (recursivo)
# --------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

# --------------------------------------------------------------------------
# Compilar Assembly dentro de src/ (recursivo)
# --------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -o $@ $<

# --------------------------------------------------------------------------
# Compilar drivers/
# --------------------------------------------------------------------------
$(BUILD_DIR)/drivers/%.o: $(DRIVERS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/drivers/%.o: $(DRIVERS_DIR)/%.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -o $@ $<

# --------------------------------------------------------------------------
# Compilar libs/
# --------------------------------------------------------------------------
$(BUILD_DIR)/libs/%.o: $(LIBS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/libs/%.o: $(LIBS_DIR)/%.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -o $@ $<



# Regra genérica com preferência:
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.s
	$(CC) $(ASFLAGS) -o $@ $<

# --------------------------------------------------------------------------
# Converter BMP → ELF object
# --------------------------------------------------------------------------
$(BUILD_DIR)/%.bmp.o: $(IMG_DIR)/%.bmp
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm $< $@

# --------------------------------------------------------------------------
# Linkagem final
# --------------------------------------------------------------------------
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -lgcc

# --------------------------------------------------------------------------
# ELF → BIN
# --------------------------------------------------------------------------
$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)

# --------------------------------------------------------------------------
# Clean
# --------------------------------------------------------------------------
clean:
	find $(BUILD_DIR) -name '*.o' -delete
	find $(BUILD_DIR)/campaign_data -name '*.h' -delete
	find $(BUILD_DIR)/campaign_data -name '*.c' -delete
	find $(BUILD_DIR)/campaign_data -name '*.stamp' -delete
	rm -f $(BUILD_DIR)/firmware.elf
	rm -f $(BUILD_DIR)/firmware.bin
	rm -f $(IMAGE_H)
	rm -f $(IMAGE_C)

.PHONY: all clean

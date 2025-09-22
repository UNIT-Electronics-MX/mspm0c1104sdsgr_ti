# rules.mk - Configuración de compilación y reglas avanzadas para MSPM0C1104

# ============================================================================
# CONFIGURACIÓN DEL COMPILADOR
# ============================================================================

# Ruta al SDK (ajusta según tu instalación)
MSPM0_SDK_INSTALL_DIR ?= /home/mr/Documents/mspm-/mspm0-sdk

# Herramientas del compilador
CC = arm-none-eabi-gcc
LNK = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Directorios de construcción
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
OUTPUT_DIR = $(BUILD_DIR)/output

# Archivos objetivo
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCE_FILES:.c=.obj)))
TARGET = $(OUTPUT_DIR)/$(PROJECT_NAME).out
TARGET_HEX = $(OUTPUT_DIR)/$(PROJECT_NAME).hex

# Flags del compilador para MSPM0C1104
CFLAGS = -I$(MSPM0_SDK_INSTALL_DIR)/source \
         -I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include \
         -I$(MSPM0_SDK_INSTALL_DIR)/source/ti/devices/msp/m0p/mspm0c110x \
         -I$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/dl_m0p \
         -D__MSPM0C1104__ \
         -Ilibs/config \
         -Ilibs/i2c \
         -Ilibs/display \
         -Ilibs/fonts \
         -mcpu=cortex-m0plus \
         -mthumb \
         -mfloat-abi=soft \
         -ffunction-sections \
         -fdata-sections \
         -g \
         -gstrict-dwarf \
         -Wall \
         -I. \
         -O2

# Flags del linker
LFLAGS = -L$(MSPM0_SDK_INSTALL_DIR)/source \
         -l:ti/driverlib/lib/gcc/m0p/mspm0c110x/driverlib.a \
         -mcpu=cortex-m0plus \
         -mthumb \
         -mfloat-abi=soft \
         -static \
         --specs=nano.specs \
         -Wl,--gc-sections \
         -T device_linker.lds \
         -Wl,-Map,$(OUTPUT_DIR)/$(PROJECT_NAME).map

# VPATH para buscar archivos fuente
VPATH = .:libs/config:libs/i2c:libs/display:libs/fonts

# ============================================================================
# REGLAS DE CONSTRUCCIÓN
# ============================================================================

# Crear directorios de construcción
$(BUILD_DIR):
	@mkdir -p $(OBJ_DIR) $(OUTPUT_DIR)

# Compilación de archivos .c a .obj
$(OBJ_DIR)/%.obj: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Linking
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)"
	$(LNK) $(OBJECTS) $(LFLAGS) -o $@

# Crear archivo HEX
$(TARGET_HEX): $(TARGET)
	@echo "Creating HEX file"
	$(OBJCOPY) -O ihex $< $@
	@echo "Build complete!"
	@$(SIZE) $<

# ============================================================================
# REGLAS AVANZADAS DE PROGRAMACIÓN
# ============================================================================

# PROGRAMACIÓN DE EMERGENCIA (múltiples métodos)
flash-emergency: $(TARGET_HEX)
	@echo "=== Programación de Emergencia - Múltiples Métodos ==="
	@echo "Archivo: $(TARGET_HEX)"
	@if [ ! -f "$(TARGET_HEX)" ]; then \
		echo "Error: $(TARGET_HEX) no existe. Ejecuta 'make all' primero."; \
		exit 1; \
	fi
	@echo "Método 1: Autodetección con borrado sector..."
	-pyocd flash -O frequency=100000 --erase=sector $(TARGET_HEX) || echo "Método 1 falló"
	@echo "Método 2: Target específico sin reset..."
	-pyocd flash -t mspm0c1104 -O frequency=100000 --no-reset $(TARGET_HEX) || echo "Método 2 falló"
	@echo "Método 3: Autodetección básica..."
	-pyocd flash -O frequency=100000 $(TARGET_HEX) || echo "Método 3 falló"
	@echo "=== Si ningún método funcionó, revisa la conexión física ==="

# Reset manual del microcontrolador
reset:
	@echo "=== Reset Manual del Microcontrolador ==="
	-pyocd reset -t mspm0c1104 || echo "pyOCD reset falló - usa el botón físico de reset"

# Información detallada del proyecto
info: $(TARGET)
	@echo "=== Información del Proyecto ==="
	@echo "Proyecto: $(PROJECT_NAME)"
	@echo "Archivos fuente: $(SOURCE_FILES)"
	@echo "Dispositivo: MSPM0C1104"
	@echo ""
	@echo "=== Tamaño del binario ==="
	@$(SIZE) $(TARGET)

# Ayuda completa
help-full:
	@echo "=== Makefile para PA24 Example - MSPM0C1104 ==="
	@echo ""
	@echo "Targets principales:"
	@echo "  all             - Compilar proyecto completo"
	@echo "  clean           - Limpiar archivos generados"
	@echo "  flash           - Programar con método verificado (RECOMENDADO) ✅"
	@echo "  help            - Ayuda básica"
	@echo ""
	@echo "Targets avanzados:"
	@echo "  flash-emergency - Múltiples métodos para casos problemáticos"
	@echo "  reset           - Reset manual del microcontrolador"
	@echo "  info            - Información detallada del proyecto"
	@echo "  help-full       - Esta ayuda completa"
	@echo ""
	@echo "Uso típico:"
	@echo "  make clean && make && make flash"
	@echo ""
	@echo "NOTA: El comando 'make flash' usa el método verificado:"
	@echo "  pyocd flash -t mspm0c1104 --connect under-reset \\"
	@echo "    -O pack.debug_sequences.disabled_sequences=..."

.PHONY: flash-emergency reset info help-full

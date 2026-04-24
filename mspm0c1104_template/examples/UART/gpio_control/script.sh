#!/bin/bash

HEX_FILE="build/output/mspm0c1104_template.hex"
TARGET="mspm0c1104"
PORT="/dev/ttyACM0"
BAUD="115200"
SWD_FREQ="100000"
MAX_OK_MESSAGES=4

RESET_ATTEMPTS=3
RESET_WAIT_SECONDS=4
SERIAL_TIMEOUT_SECONDS=10

echo "======================================"
echo " Programación MSPM0C1104 con pyOCD"
echo "======================================"
echo ""

if [ ! -f "$HEX_FILE" ]; then
    echo "ERROR: No se encontró el archivo HEX:"
    echo "$HEX_FILE"
    exit 1
fi

echo "Programando $TARGET..."
echo ""

pyocd flash -t "$TARGET" \
    --connect under-reset \
    --frequency "$SWD_FREQ" \
    -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
    "$HEX_FILE"

PYOCD_STATUS=$?

echo ""

if [ $PYOCD_STATUS -ne 0 ]; then
    echo "ERROR: Falló la programación del MSPM0C1104."
    echo "No se abrirá comunicación serial."
    exit 1
fi

if [ ! -e "$PORT" ]; then
    echo "ERROR: No existe el puerto serial $PORT"
    exit 1
fi

echo "Programación cargada correctamente."
echo "Configurando UART en $PORT a $BAUD baudios..."
echo ""

stty -F "$PORT" "$BAUD" cs8 -cstopb -parenb -ixon -ixoff -crtscts raw

# Abrir el puerto serial en el descriptor 3
exec 3< "$PORT"

OK_COUNT=0
COMM_DETECTED=0

echo "======================================"
echo " Esperando inicio de comunicación UART"
echo "======================================"
echo ""

for ((i=1; i<=RESET_ATTEMPTS; i++)); do
    echo "[$i/$RESET_ATTEMPTS] Presiona el botón RESET para empezar la comunicación..."
    
    if IFS= read -r -t "$RESET_WAIT_SECONDS" LINE <&3; then
        COMM_DETECTED=1
        echo "$LINE"

        if echo "$LINE" | grep -q "{Result: OK}"; then
            OK_COUNT=$((OK_COUNT + 1))
            echo "OK detectado: $OK_COUNT/$MAX_OK_MESSAGES"
        fi

        break
    fi
done

if [ "$COMM_DETECTED" -eq 0 ]; then
    echo ""
    echo "ERROR: No se detectó comunicación serial."
    echo "El operador no presionó RESET o la MSP no está enviando datos por UART."
    echo "Finalizando script."
    exec 3<&-
    exit 1
fi

echo ""
echo "Comunicación serial detectada."
echo "Esperando $MAX_OK_MESSAGES mensajes {Result: OK}..."
echo ""

while [ "$OK_COUNT" -lt "$MAX_OK_MESSAGES" ]; do

    if IFS= read -r -t "$SERIAL_TIMEOUT_SECONDS" LINE <&3; then

        echo "$LINE"

        if echo "$LINE" | grep -q "{Result: OK}"; then
            OK_COUNT=$((OK_COUNT + 1))
            echo "OK detectado: $OK_COUNT/$MAX_OK_MESSAGES"
        fi

    else
        echo ""
        echo "ERROR: Se perdió la comunicación serial o no llegaron más datos."
        echo "No se alcanzaron los $MAX_OK_MESSAGES mensajes {Result: OK}."
        echo "OK recibidos: $OK_COUNT/$MAX_OK_MESSAGES"
        echo "Finalizando script."
        exec 3<&-
        exit 1
    fi

done

echo ""
echo "Programación exitosa, conecta la siguiente MSP"

exec 3<&-
exit 0
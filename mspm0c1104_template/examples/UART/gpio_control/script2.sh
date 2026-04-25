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

program_device() {
    local pyocd_status
    local ok_count=0
    local comm_detected=0
    local line

    echo "======================================"
    echo " Programacion MSPM0C1104 con pyOCD"
    echo "======================================"
    echo ""

    if [ ! -f "$HEX_FILE" ]; then
        echo "ERROR: No se encontro el archivo HEX:"
        echo "$HEX_FILE"
        return 1
    fi

    echo "Programando $TARGET..."
    echo ""

    pyocd flash -t "$TARGET" \
        --connect under-reset \
        --frequency "$SWD_FREQ" \
        -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
        "$HEX_FILE"

    pyocd_status=$?

    echo ""

    if [ "$pyocd_status" -ne 0 ]; then
        echo "ERROR: Fallo la programacion del MSPM0C1104."
        echo "No se abrira comunicacion serial."
        return 1
    fi

    if [ ! -e "$PORT" ]; then
        echo "ERROR: No existe el puerto serial $PORT"
        return 1
    fi

    echo "Programacion cargada correctamente."
    echo "Configurando UART en $PORT a $BAUD baudios..."
    echo ""

    stty -F "$PORT" "$BAUD" cs8 -cstopb -parenb -ixon -ixoff -crtscts raw

    exec 3< "$PORT"

    echo "======================================"
    echo " Esperando inicio de comunicacion UART"
    echo "======================================"
    echo ""

    for ((i=1; i<=RESET_ATTEMPTS; i++)); do
        echo "[$i/$RESET_ATTEMPTS] Presiona el boton RESET para empezar la comunicacion..."

        if IFS= read -r -t "$RESET_WAIT_SECONDS" line <&3; then
            comm_detected=1
            echo "$line"

            if echo "$line" | grep -q "{Result: OK}"; then
                ok_count=$((ok_count + 1))
                echo "OK detectado: $ok_count/$MAX_OK_MESSAGES"
            fi

            break
        fi
    done

    if [ "$comm_detected" -eq 0 ]; then
        echo ""
        echo "ERROR: No se detecto comunicacion serial."
        echo "El operador no presiono RESET o la MSP no esta enviando datos por UART."
        echo "Finalizando intento."
        exec 3<&-
        return 1
    fi

    echo ""
    echo "Comunicacion serial detectada."
    echo "Esperando $MAX_OK_MESSAGES mensajes {Result: OK}..."
    echo ""

    while [ "$ok_count" -lt "$MAX_OK_MESSAGES" ]; do
        if IFS= read -r -t "$SERIAL_TIMEOUT_SECONDS" line <&3; then
            echo "$line"

            if echo "$line" | grep -q "{Result: OK}"; then
                ok_count=$((ok_count + 1))
                echo "OK detectado: $ok_count/$MAX_OK_MESSAGES"
            fi
        else
            echo ""
            echo "ERROR: Se perdio la comunicacion serial o no llegaron mas datos."
            echo "No se alcanzaron los $MAX_OK_MESSAGES mensajes {Result: OK}."
            echo "OK recibidos: $ok_count/$MAX_OK_MESSAGES"
            echo "Finalizando intento."
            exec 3<&-
            return 1
        fi
    done

    echo ""
    echo "Programacion exitosa."

    exec 3<&-
    return 0
}

device_count=1

while true; do
    echo ""
    echo "======================================"
    echo " Dispositivo $device_count"
    echo "======================================"

    program_device

    echo ""
    read -r -p "Conecta el siguiente dispositivo y presiona Enter para continuar, o escribe q para salir: " user_input

    if [ "$user_input" = "q" ] || [ "$user_input" = "Q" ]; then
        echo "Saliendo del proceso continuo."
        exit 0
    fi

    device_count=$((device_count + 1))
done
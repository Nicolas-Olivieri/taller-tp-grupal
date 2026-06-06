#!/usr/bin/env bash

set -e

GREEN='\033[0;32m'
BLUE='\033[94m'
YELLOW='\033[1;33m'
RED='\033[1;31m'
NC='\033[0m'

REPO_URL="https://github.com/Nicolas-Olivieri/taller-tp-grupal"
PROJECT_DIR="taller-tp-grupal"
GAME_NAME="taller_tp"
INSTALL_BIN="/usr/bin"
INSTALL_CONFIG="/etc/${GAME_NAME}"
INSTALL_DATA="/var/${GAME_NAME}"
DESKTOP_DIR="$HOME/Desktop/"

if [ ! -d "$DESKTOP_DIR" ]; then
    DESKTOP_DIR="$HOME/Escritorio"
fi

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}   Instalador de ${GAME_NAME}${NC}"
echo -e "${BLUE}========================================${NC}"

echo -e "${GREEN}[1/6] Actualizando repositorios...${NC}"
sudo apt update

echo -e "${GREEN}[2/6] Instalando dependencias...${NC}"
sudo apt install -y \
    git cmake build-essential ninja-build pkg-config \
    qt6-base-dev \
    libasound2-dev libopus-dev libopusfile-dev libxmp-dev \
    libfluidsynth-dev fluidsynth libwavpack-dev libfreetype-dev \
    libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

echo -e "${GREEN}[3/6] Clonando repositorio...${NC}"
if [ ! -d "$PROJECT_DIR" ]; then
  git clone "$REPO_URL" "$PROJECT_DIR"
  cd "$PROJECT_DIR"
  git fetch --all
  git switch feat/Install_integration
  cd ..
else
  cd "$PROJECT_DIR"
  git pull
  cd ..
fi

echo -e "${GREEN}[4/6] Creando directorios de sistema...${NC}"
sudo mkdir -p "$INSTALL_CONFIG"
sudo mkdir -p "$INSTALL_DATA"

echo -e "${GREEN}    Instalando archivos de configuración...${NC}"

cd "$PROJECT_DIR"

if [ -d "resources" ]; then
    # Busco todos los archivos .toml dentro de resources/
    find resources/ -type f -name "*.toml" | while read -r file; do
        # Ruta relativa sin el prefijo "resources/"
        relative_path="${file#resources/}"

        sudo mkdir -p "$(dirname "${INSTALL_CONFIG}/${relative_path}")"
        echo -e "      -> Copiando ${file} a ${INSTALL_CONFIG}/${relative_path}"
        sudo cp "$file" "${INSTALL_CONFIG}/${relative_path}"
    done
else
    echo -e "${RED}Error: La carpeta 'resources' no existe en el repositorio.${NC}"
    exit 1
fi

echo -e "${GREEN}    Instalando recursos...${NC}"
if [ -d "resources" ]; then
    sudo mkdir -p "$INSTALL_DATA/resources"
    sudo cp -r resources "$INSTALL_DATA/"

    # Elimino los archivos .toml que se copiaron junto con los assets
    sudo find "${INSTALL_DATA}/resources" -type f -name "*.toml" -delete
else
    echo -e "${RED}Error: La carpeta 'resources' no existe.${NC}"
    exit 1
fi

echo -e "${GREEN}    Ajustando permisos...${NC}"
sudo chown -R $USER:$USER "$INSTALL_CONFIG"
sudo chown -R $USER:$USER "$INSTALL_DATA"

# No debería de ser debug, pero de momento rompe si no lo ponemos como tal
echo -e "${GREEN}[5/6] Compilando el proyecto...${NC}"
cmake -S . -B cmake-build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug\
        -DCONFIG_PATH="${INSTALL_CONFIG}" \
        -DDATA_PATH="${INSTALL_DATA}/resources"

cmake --build cmake-build-debug -j$(nproc)

echo -e "${GREEN}    Instalando binarios en ${INSTALL_BIN}...${NC}"
sudo cp cmake-build-debug/taller_client "$INSTALL_BIN"
sudo cp cmake-build-debug/taller_server "$INSTALL_BIN"
sudo cp cmake-build-debug/taller_editor "$INSTALL_BIN"

cd ..

echo -e "${GREEN}[6/6] Copiando lanzadores al escritorio...${NC}"
cp "$PROJECT_DIR/client.sh" "$DESKTOP_DIR/"
cp "$PROJECT_DIR/server.sh" "$DESKTOP_DIR/"
cp "$PROJECT_DIR/editor.sh" "$DESKTOP_DIR/"
chmod +x "$DESKTOP_DIR/client.sh" "$DESKTOP_DIR/server.sh" "$DESKTOP_DIR/editor.sh"

echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}   ¡Instalación completada con éxito!${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "Binarios instalados en: ${INSTALL_BIN}"
echo -e "Configuraciones en:     ${INSTALL_CONFIG}"
echo -e "Datos en:               ${INSTALL_DATA}"
echo -e "Lanzadores en:          ${DESKTOP_DIR}"
echo ""
echo -e "Para ejecutar el servidor: haz doble clic en ${YELLOW}server.sh${NC} (escritorio)"
echo -e "Para ejecutar el cliente:  haz doble clic en ${YELLOW}client.sh${NC} (escritorio)"
echo -e "Para ejecutar el editor:   haz doble clic en ${YELLOW}editor.sh${NC}"

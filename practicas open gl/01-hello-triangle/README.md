========= 5) Build con CMake y ejecución =========

mkdir -p build
cd build
if [[ “$OSTYPE” == “darwin”* ]]; then
cmake -DCMAKE_PREFIX_PATH=”$(brew –prefix)” ..
else
cmake ..
fi
cmake –build . -j
./hello_triangle || true  # no falles el script si cierras la ventana

========= 6) Subir por rama y abrir PR (requiere remoto y gh autenticado) =========

cd ..
cd ../..  # vuelves a la raíz del repo
git checkout -b feat/01-hello-triangle
git add “practicas open gl/01-hello-triangle”
git commit -m “Hello Triangle con CMake: guía para principiantes (GLFW+GLEW)”
git push -u origin feat/01-hello-triangle
if command -v gh >/dev/null 2>&1; then
gh pr create –fill –title “OpenGL: Hello Triangle (CMake)” –body “Proyecto mínimo con comentarios pedagógicos en main.cpp y README.”
fi

echo “Listo: proyecto creado en ‘$BASE_DIR’, compilado y subido en rama ‘feat/01-hello-triangle’.”

# Programmation OpenGL Avancée (POGLA)

## Prérequis

Avant de commencer, assurez-vous d'avoir installé les éléments suivants :
- CMake (pour la compilation)
- Un compilateur C++ compatible (GCC pour Linux, MSVC pour Windows)

## Compiler et exécuter le projet

### Sous Windows

#### Compilation

Ouvrez un terminal et naviguez jusqu'au répertoire racine du projet, puis exécutez :

```sh
mkdir .\build\
cmake -B .\build\
cd .\build\
cmake --build .
```

#### Exécution

Après la compilation, pour lancer l'application, exécutez :

```sh
cd .\Debug\ # ou cd .\build\Debug\ si vous êtes à la racine
.\POGLA.exe
```

### Sous Linux

#### Configuration

Avant de compiler, assurez-vous de changer les chemins des shaders et des textures dans le fichier `./src/main.cpp`.  
Changez les lignes suivantes, se trouvant à partir de la ligne `213` :

```cpp
Shader wallShader("../../shaders/wall.vtx.glsl", "../../shaders/wall.frg.glsl");
Shader shaderProgram("../../shaders/grid.vtx.glsl", "../../shaders/grid.frg.glsl", "../../shaders/grid.tess.ctrl.glsl", "../../shaders/grid.tess.eval.glsl");
Shader shaderCompute("../../shaders/ripple.comp.glsl");

Texture marble("../../textures/marble.png");
```

par celles-ci :

```cpp
Shader wallShader("./shaders/wall.vtx.glsl", "./shaders/wall.frg.glsl");
Shader shaderProgram("./shaders/grid.vtx.glsl", "./shaders/grid.frg.glsl", "./shaders/grid.tess.ctrl.glsl", "./shaders/grid.tess.eval.glsl");
Shader shaderCompute("./shaders/ripple.comp.glsl");

Texture marble("./textures/marble.png");
```

#### Compilation

Dans le terminal, naviguez jusqu'au répertoire racine du projet et exécutez :

```sh
make
```

#### Exécution

Pour lancer l'application, exécutez :

```sh
./myapp
```

## Commandes

Une fois l'application lancée, vous pouvez utiliser les touches suivantes pour interagir avec la scène :

- **Déplacements**
    - **`Z`** : Avancer
    - **`S`** : Reculer
    - **`Q`** : Se déplacer sur la gauche
    - **`D`** : Se déplacer sur la droite
    - **`A`** : Descendre
    - **`E`** : Monter

- **Normales de l'étendue d'eau**
    - **`N`** : Afficher/Masquer les normales de l'étendue d'eau

- **Changement de texture**
    - **`T`** : Changer la texture du mur

- **Mode d'affichage**
    - **`X`** : Basculer entre l'affichage des lignes et des polygones

- **Goutte d'eau**
    - **`Espace`** : Lancer une goutte d'eau (appuyez plusieurs fois pour intensifier)

- **Déplacements de l'impact de la goutte d'eau**
    - **Flèches du clavier** : Déplacer l'endroit de l'impact de la goutte d'eau sur l'étendue d'eau
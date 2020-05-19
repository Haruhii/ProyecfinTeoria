# Manual Técnico

Si se quiere compilar en *visual studio* se recomienda utilizarlo en modo release a x64,
puesto que algunas librerias se compilarón en x64

### Configuración del proyecto

Si se requiere configurar el proyecto, esto es lo que se debe de tener para lograr que compile

En **Release x64**

### Vinculador - entrada

  irrKlang.lib;assimp-vc141-mt.lib;opengl32.lib;glfw3dll.lib;glfw3.lib;glew32s.lib;glew32.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)

### Vinculador - salida

  $(OutDir)$(TargetName)$(TargetExt)

### C++ - general

  include;glm

## Bugs

Hay un bug precente que presenta una excepción, si eso ocurre por favor de limpiar el proyecto
y la solución hasta que se logre ejecutar.

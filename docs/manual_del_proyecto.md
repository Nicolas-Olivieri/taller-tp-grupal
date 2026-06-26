# Manual del proyecto

---

## 1. Organización

### 1.1 Planificación inicial

Durante la semana previa al inicio del trabajo, nos concentramos en la organización y diseño de clases del proyecto. Si bien se intentó tener en cuenta todos los aspectos y *features* del juego desde un primer momento, el objetivo fue plantear una estructura de clases sólida, lo suficientemente flexible para poder acomodar luego factores no considerados. Dada la poca experiencia que teníamos con SDL, el diseño se centró principalmente en el servidor, definiendo para el cliente unicamente las clases relacionadas a la comunicación y lógica.

Si bien todos participamos del diseño, para la etapa de desarrollo se definió que Bogdan Skorokhod y Bono Neer se concentrarían en el servidor, mientras que Nicolás Olivieri y Oriana Toscano trabajarían en el cliente, con QT y SDL2pp respectivamente.

### 1.2 Repartición final del trabajo

A pesar de lo planteado inicialmente, todos los integrantes del equipo escribimos código tanto de cliente como de servidor. Durante la segunda semana de trabajo aproximadamente, llegamos a la conclusión de que era preferible que cada uno implementara *features* completas, sin limitarse a un solo *scope* del proyecto. El sistema permitió trabajar de una manera más independiente, que aportaba mayor valor al proyecto de forma rápida: disminuyó la necesidad de explicarnos código mutuamente, y aseguró que todo comportamiento del servidor se viera plasmado en el cliente.

Por ello, dejamos a continuación una tabla detallando algunas de las *features* más importantes que llevó a cabo cada integrante, de las cuales podemos atribuirnos una autoría casi total. Muchas otras partes del juego fueron implementadas de manera paulatina y colaborativa.

| Bono Neer                                                                                 | Nicolás Olivieri | Bogdan Skorokhod | Oriana Toscano  |
|-------------------------------------------------------------------------------------------| --- | --- | --- |
| • Criaturas (enemigos)<br> • Biomas<br> • Persistencia (users)<br> • Chat<br> • Protocolo |• Clanes<br> • Teletransportación<br> • Conexión (cliente)<br> • Configs (TOML)<br> • Refactor KeyMap |  • Patrón command<br> • NPC aliados<br> • Sonidos<br> • Inventario (UI)<br> • Labels (entidades del cliente)<br>  |   • Lobby<br> • Editor<br> • Persistencia (mapa)<br> • Mejora performance (cliente)<br> • Refactor Sprites/UI  |

## 2. Herramientas

En primer lugar, en cuanto a IDEs, utilizamos principalmente CLion, de JetBrains, y en menor medida Visual Studio Code. Para el chequeo de estilos, como fue indicado por la cátedra y sin importar el IDE, hicimos uso de: clang format, cpp check y cpp lint.

En cuanto a documentación, utilizamos principalmente:

- [CPP Reference](https://cppreference.com/) | [C Plus Plus](https://cplusplus.com/reference/deque/) para referencias generales de C++.
- [Refactoring Guru](https://refactoring.guru/design-patterns/catalog) para el diseño de clases según patrones extensibles.
- [Lazy Foo Tutorials](https://lazyfoo.net/tutorials/SDL/) | [Lib SDL2pp](https://libsdl2pp.github.io/) para SDL.
- [QT Docs](https://doc.qt.io/qt-6/qtwidgets-module.html) para QT.
- [Book of Gehn](https://book-of-gehn.github.io/articles/2019/10/23/Constant-Rate-Loop.html) para el constant rate loop.

## 3. Problemas

### 3.1 Puntos problemáticos

Para empezar, la mayoría de problemas en el desarrollo se presentaron en el cliente, principalmente debido a SDL y QT, y la curva de aprendizaje que implicaron al inicio del proyecto. Además, si bien podría considerarse un punto más “tedioso” que “problemático”, otorgarle al juego la estética final requirió una gran dedicación en aspectos que no estaban estrictamente vinculados al código, como la edición de imágenes y la selección y recorte de assets. Esta última tarea en particular fue especialmente lenta debido a la mala organización del repositorio original de imágenes.

Por otro lado, desde el servidor lo más complejo fue definir la lógica de *Interact,* ya que se vinculaba a múltiples comportamientos distintos. La buena planificación inicial evitó tener grandes problemas en este *scope*.

### 3.2 Retrospectiva

Finalizado el proyecto, consideramos que pudimos incluir todas las *features* pedidas en la consigna. Por supuesto hubo múltiples *bugs* a lo largo del desarrollo, pero hasta donde hemos detectado todos fueron arreglados antes de la entrega final.

Analizando el estado del proyecto, hay algunos puntos que nos gustaría haber mejorado. Sin embargo, nos deja tranquilos saber que refactorizamos hasta último momento, trabajando en TODOs que habíamos dejado en el código a lo largo del desarrollo. Algunos de los aspectos que retocaríamos si volviéramos a encarar el proyecto son:

- Haber investigado más el Logger (mensajes de error a syslog) desde un inicio o haberlo eliminado más temprano. Durante parte del desarrollo, y sin que entendamos por qué, los errores del cliente fueron muy poco descriptivos, por lo que optamos por el uso de asserts para facilitar el proceso de debbuging. En la última semana descubrimos que se debía al Logger, y si bien lo corregimos, nos hubiera ahorrado tiempo saberlo antes.
- Dado el formato de TOML utilizado para cargar las texturas al cliente, la carpeta  */resources* escaló en tamaño más de lo que esperábamos. Desde un inicio debería haber sido separada en **/assets** para las carpetas de imágenes, y **/config** para los archivos TOML.
- Algunos nombres de clases y archivos resultaron redundantes. Por ejemplo, el prefijo “cmd_” para las clases vinculadas al patrón command en el servidor.
- Ciertas partes de la lógica podrían haber sido reemplazadas y simplificadas con el uso de excepciones.
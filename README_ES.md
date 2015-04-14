 #CSpec 
Mini framework para hacer [BDD](http://en.wikipedia.org/wiki/Behavior-driven_development) en C

##Un pequeño ejemplo

```C
#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>

context (ejemplo) {

    describe("Describe de ejemplo") {

        it("true debería ser igual a true") {
            should_bool(true) be equal to(true);
        } end

        it("true no debería ser igual a false") {
            should_bool(true) not be equal to(false);
        } end

        it("este test va a fallar porque 10 no es igual a 11") {
            should_int(10) be equal to(11);
        } end

        skip("este test deberia fallar porque \"Hola\" no es \"Chau\"") {
            should_string("Hola") be equal to("Chau");
        } end

    } end

}
```

* Compilar: `gcc cspecEjemplo.c -o cspecEjemplo -lcspecs`
* Ejecutar: `./cspecEjemplo`

```

  Describe de ejemplo
    ✔ true debería ser igual a true
    ✔ true no debería ser igual a false
    1) Describe de ejemplo - este test va a fallar porque 10 no es igual a 11
    • este test deberia fallar porque "Hola" no es "Chau"


  Summary

    1) Describe de ejemplo - este test va a fallar porque 10 no es igual a 11
      - Expected <11> but was <10> [./cspecEjemplo.c:18]

  2 success
  1 failure
  1 pending

```

##Empecemos!
###¿Cómo lo instalo?
1. `git clone https://github.com/pepita-remembrance/cspec.git`
1. `cd cspec`
1. `make`
1. `sudo make install`

###Ahora, ¿qué hago?
* Escribí el código C que quieras
* Probalo usando este framework, para eso no te olvides de compilarlo con `-lcspecs`. Por ejemplo: `gcc cspecEjemplo.c -o cspecEjemplo -lcspec`
* Correlo en la consola: `./cspecEjemplo`

####¿Estás codificando en eclipse y te muestra errores por todos lados?
Tenés que agregar cspec a las dependencias del proyecto.
Para eso...

1. Click derecho en el proyecto => `Properties`
1. En el panel de la derecha ir a `C/C++ Build` => `Settings` => `Tool Settings` => `GCC C++ Linker` => `Libraries`
1. En el panel de la derecha, que está arriba (`Libraries (-l)`) hacé click en `add` y luego escribí `cspecs`
1. Aplicá los cambios
1. Recompilar el proyecto

###¿Qué me ofrece CSpecs?
Me ofrece un conjunto de operaciones - basadas en [RSpec](http://rspec.info/) (Ruby), [Mocha](http://visionmedia.github.io/mocha/), [Jasmine](http://jasmine.github.io/) y [Karma](http://karma-runner.github.io/0.12/index.html) (JavaScript) - que me permiten realizar pruebas (unitarias y de integración del código) [orientadas al comportamiento](http://en.wikipedia.org/wiki/Behavior-driven_development)

##¿Cómo lo uso?

###context
Cada comportamiento a testear debe estar dentro de un contexto. La forma de definir un contexto en CSpecs es de esta forma:

```C
context(<identificador>) {
    /* Acá va a el contexto */
}
```

Cómo todo lo que ocurre en C, y dado que el framework trabaja de forma sincrónica, el código escrito dentro de un contexto se ejecuta de forma secuencial.

Dentro de un **contexto** puedo escribir funciones y llamarlas desde mis tests, puedo incluir archivos (.h), definir macros y describir escenarios - **describes**.

###describe
Cada escenario se escribe dentro de un **describe**, de esta forma:

```C
describe("Breve descripción del escenario") {
    /* Acá va a el código */
} end
```

Nuevamente, dentro de un **describe** puedo escribir funciones y llamarlas desde mis test, puedo incluir archivos (.h), definir macros y escribir los test - **its**.

###it

Cada **it** es un test, propiamente dicho.

```C
it("Breve descripción del test") {
    /* Acá va a el código, con sus respectivas aserciones */
} end
```

Dentro de mismo, debo realizar las aserciones sobre el comportamiento que quiero probar. Para ello, CSpecs, cuenta con operaciones básicas para realizarlas - **shoulds**

###should
Cada **should** es una aserción, la cual espera 2 valores. El primero es el que quiero testear contra el segundo, el esperado.

```C
should_bool(<booleano_actual>) be equal to(<booleano_esperado>);
should_bool(<booleano_actual>) not be equal to(<booleano_no_esperado>);

should_char(<caracter_actual>) be equal to(<caracter_esperado>);
should_char(<caracter_actual>) not be equal to(<caracter_no_esperado>);

should_short(<numero_actual>) be equal to(<numero_esperado>);
should_short(<numero_actual>) not be equal to(<numero_no_esperado>);

should_int(<numero_actual>) be equal to(<numero_esperado>);
should_int(<numero_actual>) not be equal to(<numero_no_esperado>);

should_long(<numero_actual>) be equal to(<numero_esperado>);
should_long(<numero_actual>) not be equal to(<numero_no_esperado>);

should_float(<flotante_actual>) be equal to(<flotante_esperado>);
should_float(<flotante_actual>) not be equal to(<flotante_no_esperado>);

should_double(<decimal_actual>) be equal to(<decimal_esperado>);
should_double(<decimal_actual>) not be equal to(<decimal_no_esperado>);

should_ptr(<puntero_actual>) be equal to(<puntero_esperado>);
should_ptr(<puntero_actual>) not be equal to(<puntero_no_esperado>);

should_string(<palabra_actual>) be equal to(<palabra_esperada>);
should_string(<palabra_actual>) not be equal to(<palabra_no_esperada>);
```

A su vez, CSpecs ofrece [azúcares sintácticos](http://en.wikipedia.org/wiki/Syntactic_sugar) para ciertos tipos de aserciones, como las siguietes.

```C
should_bool(<booleano_actual>) be truthy;
should_bool(<booleano_actual>) not be truthy;

should_bool(<booleano_actual>) be falsey;
should_bool(<booleano_actual>) not be falsey;

should_ptr(<puntero_actual>) be null;
should_ptr(<puntero_actual>) not be null;
```

###Hooks - before y after

Hay veces que entre los test se repiten escenarios o configuraciones iniciales y/o se libera la memoria de las mismas variables. Para manejar eso, dentro de cada **describe**, puede agregarse una porcion de código para ejecutar **antes** y **después** de cada **it**

####before
```C
before {
    /* codigo para ejecutar antes de cada it */
} end
```

####after
```C
after {
    /* codigo para ejecutar después de cada it */
} end
```

**Nota:** Tener en cuenta que, como se ha mencionado anteriormente, el describe se ejecuta de forma secuencial, por eso es __muy importante__ recordar que el **before** y **after** deben estar al principio del **describe** (antes del primer **it**)

##Veamos ahora un ejemplo completo, con su respectiva ejecución

```C
#include <stdio.h>
#include <stdlib.h>
#include <cspecs/cspec.h>

context (ejemplo_completo) {

    describe("Describe 1") {

        int *a = NULL,
             b;

        before {
            puts("before 1");
            a = malloc(sizeof(int));
            *a = 10;
            b = 20;
        } end

        after {
            puts("after 1");
            free(a);
            a = NULL;
        } end

        it("*a debería ser 10 y b debería ser 20") {
            should_int(*a) be equal to(10);
            should_int(b) be equal to(20);
        } end

        describe("Describe 2") {

            before {
                puts("before 2");
                *a = 30;
                b = 15;
            } end

            after {
                puts("after 2");
                free(a);
                a = NULL;
            } end

            it("*a debería ser 30 y b debería ser 15") {
                should_int(*a) be equal to(30);
                should_int(b) be equal to(15);
            } end

            describe("Describe 3") {

                before {
                    puts("before 3");
                    b = 150;
                } end

                after {
                    puts("after 3");
                    free(a);
                    a = NULL;
                } end

                it("*a deberia ser 30 y b debería ser 150") {
                    should_int(*a) be equal to(30);
                    should_int(b) be equal to(150);
                } end

            } end

        } end

        describe("Describe 4") {

            it("*a debería ser 10 y b debería ser 20") {
                should_int(*a) be equal to(10);
                should_int(b) be equal to(20);
            } end

        } end

    } end

}
```

Una vez compilado el código y ejecutado, nos daría un reporte como el siguiente:

```

  Describe 1
before 1
    ✔ *a debería ser 10 y b debería ser 20
after 1
    Describe 2
before 1
before 2
      ✔ *a debería ser 30 y b debería ser 15
after 2
after 1
      Describe 3
before 1
before 2
before 3
        ✔ *a deberia ser 30 y b debería ser 150
after 3
after 2
after 1
    Describe 4
before 1
      ✔ *a debería ser 10 y b debería ser 20
after 1


  Summary

  4 success

```

###¿Cómo lo desinstalo?
1. Ir a la carpeta clonada del repositorio.
1. `make clean`
1. `sudo make uninstall`

######Gracias!

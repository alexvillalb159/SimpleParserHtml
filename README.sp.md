# SimpleParserHtml ([see in English](README.md)) 
=================================================

------------------------------------------------------------------
Un simple parseador de línea de comandos para documentos html. 

Tiene dos modos de operación, uno en el cual muestra el contenido 
que esta en una <b>ruta de tags o etiquetas</b>, que es un 
conjunto de tags contenidas una dentro de la otra de forma 
consecutiva. 

En otro modo de operación se le da de entrada cualquier contenido
a buscar en el documento html (que puede ser visible o no) y 
muestra cuales son las etiquestas con ese contenido.



Por ejemplo sea el documento test.html:

```
<html>
<head>
</head>
	<body>
	<table id=111>
		<tr>Hello World 1</tr>
	</table>
	<table class='hello' id=200>
		<tr id=29>Hello World 2</tr>
	</table>


	<table class='nothing' id=300>
		<tr>hello world 3</tr>
	</table>
	
	</body>
</html>

```

En el primer modo de operación para ver el contenido que esta 
entre todas las etiquetas table se ejecuta el comando:

./simpleparserhtml test.html table

El resultado seria:
```
1: 
		<tr>Hello World 1</tr>
	
2: 
		<tr id=29>Hello World 2</tr>
	
3: 
		<tr>hello world 3</tr>
```

Las apariciones son numeradas de uno en uno siguiendo el mismo 
orden en el cual se encuentra en el archivo, la numeración se 
coloca a la derecha del contenido encontrado en color rojo. En 
el caso de arriba el número '1' indica la primera aparación del 
contenido buscado, luego de los dos punto ':', aparece el 
contenido original del archivo y así para el 2 y el 3.

Si se quiere hacer una busqueda más específica se puede 
especificar las etiquetas consecutivas en la que esta el contenido
o los atributos de las etiquetas. Este comando mezcla las dos 
cosas:

./simpleparserhtml test.html table class=\'hello\' tr id=29

Resultado:

```
1: Hello World 2
```



En el 2do Modo se operación se específica el contenido a buscar 
en el  archivo html y se obtiene como salida las tags en las que 
se encuentra ese contenido. Es obligatorio específicar el grado de
profundidad, luego del parámetro -d, que indica cuantos de los 
tags, que encierran el contenido, se mostrarán. Los tags en los 
que esta el contenido se muestran junto con sus atributos. Para 
el mismo documento del caso anterior este seria un ejemplo:

./simpleparserhtml -d 3 tests/simpleparserhtml/test.html "hello"

La salidad seria:

```
96: <html><body><table class='hello' id=200>
192: <body><table class='nothing' id=300><tr>hello world 3
```

Donde los números de la derecha (96, 192) son los offsets en el 
cual esta el contenido buscado ("hello"), en color rojo. Luego 
de los dos puntos del offset se imprimen la ruta de etiquetas y 
el contenido. Notese que en el caso del offset 96 el contenido 
esta dentro de la misma definición de la etiqueta y no en el 
contenido o el bloque de esta. 

------------------------------------------------------------------

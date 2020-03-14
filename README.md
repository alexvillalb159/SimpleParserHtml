SimpleParserHtml ([ver en Español](README.sp.md)) 
=================================================

------------------------------------------------------------------


A simple command line parser for html documents.

It has two modes of operation, one of which shows the content
which is in a <b> tag or tag path </b>, which is a
set of tags contained one inside the other so
consecutive.

In another mode of operation, it is given input to any content to 
search in the html document (which may or may not be visible) 
and shows what the tags are with that content.

For example given the document test.html:
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


In the first mode of operation to see the content that is between 
all the 'table' labels, this command is executed:

./simpleparserhtml test.html table

The result would be:


```
1: 
		<tr>Hello World 1</tr>
	
2: 
		<tr id=29>Hello World 2</tr>
	
3: 
		<tr>hello world 3</tr>
```


The results are numbered one by one following the same order in 
which it is found in the file, the numbering is placed to the 
right of the content found in red. In the case above, the number 
'1' indicates the first appearance of the content sought, after 
the two points ':', the original content of the file appears and 
thus for 2 and 3.

If you want to do a more specific search, you can specify the 
consecutive labels in which the content or attributes of the 
labels are. This command mixes the two things:

./simpleparserhtml test.html table class=\'hello\' tr id=29

Outcome:

```
1: Hello World 2
```


In the 2nd Mode, the content to search in the html file is 
specified and the tags in which that content is found are obtained
as output. It is mandatory to specify the degree of depth, after 
the -d parameter, which indicates how many of the Tags, which 
enclose the content, will be displayed. Tags in that this content 
is displayed along with its attributes. For the same document 
from the previous case this would be an example:

./simpleparserhtml -d 3 tests/simpleparserhtml/test.html "hello"

The output would be:

```
96: <html><body><table class='hello' id=200>
192: <body><table class='nothing' id=300><tr>hello world 3
```

Where the numbers on the right (96, 192) are the offsets in which 
the searched content is ("hello"), in red. After the ':' 
character, following the offset, the label path is printed and
the content. Note that in the case of offset 96 the content
It is within the same definition of the label and not in the
content or block of this.

------------------------------------------------------------------

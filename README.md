## Introduction

Qt Office Open Xml is a library that can be used to read and write opc packages,
such as .xlsx/.docx/.pptx files. It doesn't require
Microsoft Office and can be used in any platform that Qt5 supported.

* [Done]Classes in the namespace ```QtOfficeOpenXml::Opc``` is designed to read or write general OPC package.
* [Wip]Classes in the namespace ```QtOfficeOpenXml::Sml``` is designed to read or write Spreadsheet docuements.
* [Todo]Classes in the namespace ```QtOfficeOpenXml::Wml``` is designed to read or write Wordprocessing docuements.
* [Todo]Classes in the namespace ```QtOfficeOpenXml::Pml``` is designed to read or write Presention docuements.
* [Wip]Classes in the namespace ```QtOfficeOpenXml::Dml``` provide Drawing support for Sml, Wml and Pml.
* [Wip]Classes in the namespace ```QtOfficeOpenXml::Ooxml``` are used by Dml, Sml, Wml and Pml.
* [Done]Classes in the namespace ```QtOfficeOpenXml::Mce``` provide Markup Compatibility and
Extensibility supports for all the library.

## Getting Started

### Building the module

> **Note**: Perl is needed in this step.

* Download the source code.

* Put the source code in any directory you like

* Open the qtofficeopenxml.pro file using Qt Creator

 * Build the project.

* Go to the build directory of the project in a terminal and run

```
   make install
```

The library, the header files, and others will be installed to your system.

> **Note**: If you don't want to use Qt Creator, you can run following command
 at the toplevel directory of the project

> ```
    qmake
    make
    make install
```

### Using the module

* Add following line to your qmake's project file:

```
    QT += officeopenxml
```

* Then, using Qt Office Open XML in your code

## Source code directories

Maybe useful for users and contributors.

### ```global\```

Globals files requried by all over the library.

### ```mce\```

Depends on ```global``` only.

MCE (Markup Compatibility and Extensibility)

More information for mce can be found in ISO/IEC 29500:3.

key features of MCE?

* Alternative Content Blocks [Done]
* Ignorable and "must understand" namespaces [Done]
* ProcessContents [Done]
* Application-defined extension elements [Todo, wait for next version of ISO/IEC 29500?]
* Namespace subsumption [Won't support, seems they will be dropped by next ISO/IEC 29500?]
* ...

### ```opc\```

Depends on ```global``` and ```mce```.

OPC (Open Packaging Conventions)

More information for opc can be found in ISO/IEC 29500:2.

### ```ooxml\```

Depends on ```global```, ```mce``` and ```opc```.

Shared files for Office Open XML Parts: ```dml```, ```wml```,```sml``` and ```pml```.

More information can be found in ISO/IEC 29500:1 and ISO/IEC 29500:4.

### ```dml\```

Depends on ```global```, ```mce```, ```opc``` and ```ooxml```.

DML (DrawingML)

### ```sml\```

Depends on ```global```, ```mce```, ```opc```, ```ooxml``` and ```dml```.

SML (SpreadsheetML)

### ```wml\```

Depends on ```global```, ```mce```, ```opc```, ```ooxml``` and ```dml```.

WML (WordprocessingML)

### ```pml\```

Depends on ```global```, ```mce```, ```opc```, ```ooxml``` and ```dml```.

PML (PresentionML)

## References

### General

* https://www.assembla.com/spaces/IS29500/wiki/Home

* http://referencesource.microsoft.com/#WindowsBase/Base/System/IO/Packaging/Package.cs
* http://msdn.microsoft.com/en-us/library/System.IO.Packaging%28v=vs.110%29.aspx
* https://github.com/mono/mono

* http://msdn.microsoft.com/en-us/library/cc313071.aspx
* http://www.lyquidity.com/devblog/?p=35

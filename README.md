Qt Office Open Xml is a library that can read and write simple .xlsx/.docx/.pptx files. It doesn't require Microsoft Office and can be used in any platform that Qt5 supported.
 
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


## References

### General

* http://referencesource.microsoft.com/#WindowsBase/Base/System/IO/Packaging/Package.cs
* http://msdn.microsoft.com/en-us/library/System.IO.Packaging%28v=vs.110%29.aspx
* https://github.com/mono/mono

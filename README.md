# ComicBookReader #
A reader to support the different formats used for comics, just like some current software

To be submitted:

 - test examples
 - UML
 - program executive and code
 - readme for program (3d parts illustration included)

finally, there exists a presentation of 5 mins (explication, illustration(run the program))



## LOG ##

- Use QML to draw the GUI, especially using the Qt Quick Control, having these components

  - ToolBar
  - Menu(popup)
  - Slider

- ~~Use `QQuickPaintedItem` to transfer and show the image in Qt.~~
  Use `QQuickImageProvider` to transfer and show the image in Qt, much more simple.
  - do not need the cache in the c++ part, Qt chaches the image automatically.(But we cannot control the cache size etc, it's better to turn off this feature, if we have time).
  - can easily to read picture in an asynchronous way.

   reference is here https://stackoverflow.com/questions/20691414/qt-qml-send-qimage-from-c-to-qml-and-display-the-qimage-on-gui

- Add free drag and free scaling(ctrl + wheel) feature. The center of scaling is always the mouse position.

## TO DO (in priori order)## ​
- make the picture load asynchronouslly.
- add pre-load feature.
- change scaling bar to 3 buttons: zoom in, zoom out, auto-fit.
- link the process part to qt, finish the button T/I/R (can use the popup view: Text, Image, Raw).
- improve the image process part, including two parts: picture, and test.
- finish the button "Two".
- link the uncompress part.
- finish the button "Open".(actually no need to use the popup view).

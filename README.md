# ComicBookReader
A reader to support the different formats used for comics, just like some current software

To be submitted:

 - test examples
 - UML
 - program executive and code
 - readme for program (3d parts illustration included)

finally, there exists a presentation of 5 mins (explication, illustration(run the program))



## LOG

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
- Add multi-processing preload feature, a worker in background will always preload images, and a thread-safe cache is used to caching the images.

- Use a special slight method to transform cv::Mat to QImage, the ref is here
  https://github.com/dbzhang800/QtOpenCV
  Attention: the transfomed QImage uses the memory shared with the orignal cv::Mat, so make sure the orignal cv::Mat cannot be deleted.(Both QImage and cv::Mat inside use a pointer to save the data in a new bloc of memory, which is shared in this quick transformation.)
- After having finished the UI part, the project is re-managed by Visual Studio, which is much more fast and easy to debug. The new VS project is in the Qt folder.
- Use original cv::Mat format to cache the images instead of QImage, this can solve the problem of management of memory. (If QImage is cached, the cv::Mat cannot be freed to keep the data of QImage, but when QImage is automatically freed by cache, cv::Mat cannot be freed automatically).
- Realize the button T/I/R (mode switch)

## TO DO (in priori order)
- ~~make the picture load asynchronouslly.~~ Done
- ~~add pre-load feature.~~ Done
  - ~~change cache to the thread-safe implementation, use QCahe and QReadWriteLock~~ Done
  - ~~make preload_worker always run in background until filling all the cache~~ Done
- ~~change scaling bar to 3 buttons: zoom in, zoom out, auto-fit.~~ Done
- ~~link the process part to qt, finish the button T/I/R (can use the popup view: Text, Image, Raw).~~ Done
- ~~improve the image process part, including two parts: picture, and test.~~ Done
- add adjust bar for image process
- ~~finish the button "Two".~~ Done
- link the uncompress part.
- add parallel feature to uncompress part. One thread to read the current page, one thread to read the neighbour pages into buffer
- finish the button "Open".(actually no need to use the popup view).

# How to properly tag a new release

-   Update `include/monet.h`
-   Update `CMakeLists.txt`
-   Re-run `make -C docs -B` so that SVG files include the correct version number in comments
-   Run the following commands:

    ```
    git tag vX.Y.Z
    git push origin vX.Y.Z
    ```
-   Create a release from the tag

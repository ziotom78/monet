# How to properly tag a new release

-   Update `include/monet.h`
-   Update `CMakeLists.txt`
-   Re-run `./rebuild-examples.sh` so that SVG files include the correct version number in comments
-   Run the following commands:

    ```
    git tag vX.Y.Z
    git push origin vX.Y.Z
    ```
-   Create a release from the tag

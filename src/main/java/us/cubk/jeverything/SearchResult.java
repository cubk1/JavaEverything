package us.cubk.jeverything;

import java.io.File;
import java.nio.file.Path;
import java.nio.file.Paths;

public class SearchResult {
    private final String fileName;
    private final String filePath;

    SearchResult(String fileName, String filePath) {
        this.fileName = fileName;
        this.filePath = filePath;
    }

    public Path getPath() {
        return Paths.get(filePath, fileName);
    }

    public File getFile() {
        return getPath().toFile();
    }

    @Override
    public String toString() {
        return getPath().toString();
    }
}
    
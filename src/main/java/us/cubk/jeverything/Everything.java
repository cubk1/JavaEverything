package us.cubk.jeverything;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class Everything {
    static {
        try {
            String[] libraries = {"EverythingLoader.dll", "Everything64.dll"};
            for (String lib : libraries) {
                releaseResource(lib);
            }
            System.loadLibrary("EverythingLoader");
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private static void releaseResource(String resourceName) throws IOException {
        InputStream resourceStream = Everything.class.getResourceAsStream("/" + resourceName);
        if (resourceStream == null) {
            throw new IOException(resourceName);
        }

        try (InputStream input = resourceStream;
             FileOutputStream output = new FileOutputStream(resourceName)) {
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = input.read(buffer)) != -1) {
                output.write(buffer, 0, bytesRead);
            }
        }
    }

    public native void setSearch(String searchString);
    public native void setMatchPath(boolean enable);
    public native void setMatchCase(boolean enable);
    public native void setMatchWholeWord(boolean enable);
    public native void setRegex(boolean enable);
    public native boolean query();
    public native int getNumResults();
    public native String getResultFileName(int index);
    public native String getResultPath(int index);
    public native void reset();
    public native void cleanup();
}

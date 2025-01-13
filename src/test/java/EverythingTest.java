import us.cubk.jeverything.Search;
import us.cubk.jeverything.SearchResult;

import java.util.Collection;
import java.util.List;

public class EverythingTest {
    public static void main(String[] args) {
        Collection<SearchResult> results = Search.searchFiles(".java");

        for (SearchResult result : results) {
            System.out.println(result.getFile().getAbsolutePath());
        }
    }

}
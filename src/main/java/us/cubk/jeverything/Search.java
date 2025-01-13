package us.cubk.jeverything;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Search implements AutoCloseable {
    private final Everything everything;


    public Search() {
        everything = new Everything();
    }
    
    public static SearchBuilder newSearch() {
        return new SearchBuilder(new Search());
    }

    public static Collection<SearchResult> searchFiles(String query) {
        return newSearch()
            .query(query)
            .search();
    }

    public static Collection<SearchResult> searchFolders(String query) {
        return newSearch()
            .query(query)
            .matchPath()
            .matchWholeWord()
            .search();
    }

    public static Collection<SearchResult> searchWithRegex(String regex) {
        return newSearch()
            .query(regex)
            .useRegex()
            .search();
    }

    Collection<SearchResult> search(SearchBuilder builder) {
        everything.reset();
        everything.setSearch(builder.query);
        everything.setMatchPath(builder.matchPath);
        everything.setMatchCase(builder.matchCase);
        everything.setMatchWholeWord(builder.matchWholeWord);
        everything.setRegex(builder.regex);
        
        if (!everything.query()) {
            return new ArrayList<>();
        }
        
        return IntStream.range(0, everything.getNumResults())
            .mapToObj(i -> new SearchResult(
                everything.getResultFileName(i),
                everything.getResultPath(i)))
            .collect(Collectors.toList());
    }
    
    @Override
    public void close() {
        everything.cleanup();
    }
}
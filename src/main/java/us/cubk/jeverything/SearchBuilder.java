package us.cubk.jeverything;

import java.util.Collection;
import java.util.List;

public class SearchBuilder {
    private final Search search;
    String query;
    boolean matchPath = false;
    boolean matchCase = false;
    boolean matchWholeWord = false;
    boolean regex = false;

    SearchBuilder(Search search) {
        this.search = search;
    }

    public SearchBuilder query(String query) {
        this.query = query;
        return this;
    }

    public SearchBuilder matchPath() {
        this.matchPath = true;
        return this;
    }

    public SearchBuilder matchCase() {
        this.matchCase = true;
        return this;
    }

    public SearchBuilder matchWholeWord() {
        this.matchWholeWord = true;
        return this;
    }

    public SearchBuilder useRegex() {
        this.regex = true;
        return this;
    }

    public Collection<SearchResult> search() {
        return search.search(this);
    }
}
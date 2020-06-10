const simpleSearch = (keywords, options) => {
       
    options = options || [];
    let searchOps = [];
    
    if (options.toggleRegex.isChecked()) {
        searchOps.push("regular_expression");
    }
    if (options.toggleCase.isChecked()) {
        searchOps.push("case_sensitive");
    }
    if (options.toggleWord.isChecked()) {
        searchOps.push("whole_word");
    }
    
    // console.log(JSON.stringify(searchOps));
    // console.log("search for " + keywords);
    app.editor().find(keywords, searchOps.join(','));
}
    
const fileSearch = (keywords, replace, where, options) => {
       
    options = options || [];
    let searchOps = [];
    
    if (options.toggleRegex.isChecked()) {
        searchOps.push("regular_expression");
    }
    if (options.toggleCase.isChecked()) {
        searchOps.push("case_sensitive");
    }
    if (options.toggleWord.isChecked()) {
        searchOps.push("whole_word");
    }
    console.log("advance search for " + keywords);
    console.log("not yet working");
    // app.editor().find(keywords, searchOps.join(','));
}

export {
    simpleSearch,
    fileSearch
}
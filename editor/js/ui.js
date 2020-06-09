const panels = {};

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
    
const advanceSearch = (keywords, replace, where, options) => {
       
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

export const ui = {
        
    search: (keywords, options) => {
        options = options || {}
        
        let panelName = "search";
        if (options["advance"]) {
            panelName = "advance_search";
        }
        
        // console.log(JSON.stringify(panels));
        
        try {
            let panel = app.createPanel(panelName);
            if (panels[panelName]) {
                panel.show();
                let input = panels[panelName].input;
                if (keywords) {
                    input.setText(keywords);
                }
                input.setFocus();
                return;
            }
            
            panels[panelName] = panel;
            panels[panelName].options = options;
            
            let box = panel.hbox();
            let togglesv = panel.vbox(box);
            let labels = panel.vbox(box);
            let inputs = panel.vbox(box);
            let buttons = panel.vbox(box);
            
            let toggles = panel.hbox(togglesv);
            let toggleRegex = panel.toggleButton(".*", toggles);
            let toggleCase = panel.toggleButton("Aa", toggles);
            let toggleWord = panel.toggleButton("❝❞", toggles); //
                      
            let input = panel.inputText(keywords, inputs);           
            panels[panelName].input = input;
            
            let btnFind = panel.button("Find", buttons);
            input.onChange = (v) => {
                // console.log(v);
            }
            input.onSubmit = (action) => {
               simpleSearch(input.getText(), {
                   toggleRegex,
                   toggleCase,
                   toggleWord
               });              
            }
            btnFind.onClick = () => {
                input.onSubmit();
            }
            input.setFocus();
            
            panel.setMinimumSize(0,70);
            panel.resize(0,70);
            
            // if advance
            if (options["advance"] === true) {
                panel.label("Find", labels).setMinimumSize(70, 0);
                panel.label("Replace", labels).setMinimumSize(70, 0);
                panel.label("", togglesv); // spacer
                let replace = panel.inputText("", inputs);
                panels[panelName].replace = replace;
                let btnReplace = panel.button("Replace", buttons);
                btnReplace = () => {
                    input.onSubmit('replace');
                }
                panel.setMinimumSize(0,100);
                panel.resize(0,100);
                
                input.onSubmit = (action) => {
                   advanceSearch(
                       input.getText(),
                       replace.getText(),
                       "",
                   {
                       toggleRegex,
                       toggleCase,
                       toggleWord
                   });              
                }
            }
            
        } catch (err) {
            console.log(err);
        }
    }
}

console.log("js::ui");
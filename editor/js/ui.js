const panels = {};

export const ui = {

    search: (keywords) => {
        try {
            let panel = app.createPanel("search");
            if (panels["search"]) {
                panel.show();
                let input = panels["search"].input;
                if (keywords) {
                    input.setText(keywords);
                }
                input.setFocus();
                return;
            }
            
            let box = panel.hbox();
            let toggleRegex = panel.toggleButton(".*", box);
            let toggleCase = panel.toggleButton("Aa", box);
            let toggleWord = panel.toggleButton("❝❞", box);
            let input = panel.inputText(keywords, box);
            let btn = panel.button("search", box);
            input.onChange = (v) => {
                // console.log(v);
            }
            input.onSubmit = () => {
                let options = [];
                if (toggleRegex.isChecked()) {
                    options.push("regular_expression");
                }
                if (toggleCase.isChecked()) {
                    options.push("case_sensitive");
                }
                if (toggleWord.isChecked()) {
                    options.push("whole_word");
                }
                // console.log("search for " + input.getText() + "...");
                app.editor().find(input.getText(), options.join(','));
            }
            btn.onClick = () => {
                input.onSubmit();
            }
            input.setFocus();

            panel.setMinimumSize(0,60);
            panels["search"] = panel;
            panels["search"].input = input;
        } catch (err) {
            console.log(err);
        }
    }
}

console.log("js::ui");
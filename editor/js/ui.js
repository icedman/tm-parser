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
            let input = panel.inputText(keywords, box);
            let btn = panel.button("search", box);
            input.onChange = (v) => {
                // console.log(v);
            }
            input.onSubmit = () => {
                console.log("search for " + input.value + "...");
                app.editor().find(input.value);
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
import { keybinding } from './keybinding.js';

export {
    keybinding
};

try {
    let ui = app.createPanel("search");
    let box = ui.hbox();
    let vbox = ui.vbox(box);
    let btn = ui.button("heya!", box);
    btn.onClick = () => {
        console.log("hello");
    }
    btn = ui.button("xx", box);
    btn.onClick = () => {
        console.log("yy");
    }
    btn = ui.button("xx", vbox);
    btn.onClick = () => {
        console.log("yy");
    }
    btn = ui.button("xx", vbox);
    btn.onClick = () => {
        console.log("yy");
    }
} catch (err) {
    console.log(err);
}

console.log("js engine initialize");
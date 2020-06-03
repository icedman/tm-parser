const key_mapping = {};

export const keybinding = {
    processKeys: (k) => {
        let kb = key_mapping[k];
        if (kb) {
            // console.log(app.editor());
            // console.log(JSON.stringify(kb));
            eval(kb.command);
        }
    },
    loadMap: (m) => {
        let jm = JSON.parse(m);
        jm.forEach(kb => {
            key_mapping[kb["keys"]] = kb;
        });
        // console.log(JSON.stringify(key_mapping));
    }
}

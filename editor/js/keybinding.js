const key_mapping = {};
const last_key = {
    time: 0,
    keys: ""
};

// const ui = _ui;
const ui = {};

const keybinding = {
    processKeys: (k) => {
        // console.log(k);
        let kb = key_mapping[k];
        if (!kb) {
            return false;
        }
        // prevent double call (both mainwindow & editor are trapping keyevents)
        let t = new Date().getTime();
        if (last_key.keys == k && t - last_key.time < 150) {
            return false;
        }
        last_key.time = t;
        last_key.keys = k;
        try {
            kb.func.call();
        } catch(err) {
            console.log(err);
        }
        return true;
    },
    
    loadMap: (m) => {
        try {
        let jm =m; //  JSON.parse(m);
        jm.forEach(kb => {
            key_mapping[kb["keys"]] = kb;
            eval(`kb.func = ()=>{ ${kb.command} };`);
        });
        
        // engine.log(JSON.stringify(key_mapping));
        
        } catch(err) {
        engine.log(err);
        }
    }
}

window.keybinding = keybinding;
engine.log("js::keybinding");
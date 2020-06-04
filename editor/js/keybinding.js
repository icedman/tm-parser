import { ui as _ui } from './ui.js';

const key_mapping = {};
const last_key = {
    time: 0,
    keys: ""
};

const ui = _ui;

export const keybinding = {
    processKeys: (k) => {
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
        let jm = JSON.parse(m);
        jm.forEach(kb => {
            key_mapping[kb["keys"]] = kb;
            eval(`kb.func = ()=>{ ${kb.command} };`);
        });
    }
}

console.log("js::keybinding");
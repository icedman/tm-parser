const listeners = {
}

export const events = {
    emit: (event, payload) => {
        if (!listeners[event]) {
            return;
        }
        listeners[event].forEach((func) =>  {
            func.call(event, payload);
        })
    },

    on: (event, func) => {
        listeners[event] = listeners[event] || [];
        listeners[event].push(func);
    },

    off: (event, func) => {
        listeners[event] = listeners[event] || [];
        for(let i=0; i<listeners[event].length;i++) {
            let fn = listeners[event][i];
            if (func === fn) {
                listeners[event].splice(i, 1);
                return;
            }
        }
    }
}
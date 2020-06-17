const extensions = {
    loadMap: (m) => {
        try {
        
        } catch(err) {
            console.log(err);
        }
    }
}

window.extensions = extensions;
console.log("js::extensions");

export default extensions;
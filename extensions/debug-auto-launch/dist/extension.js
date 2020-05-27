!function(e,t){for(var n in t)e[n]=t[n]}(exports,function(e){var t={};function n(o){if(t[o])return t[o].exports;var r=t[o]={i:o,l:!1,exports:{}};return e[o].call(r.exports,r,r.exports,n),r.l=!0,r.exports}return n.m=e,n.c=t,n.d=function(e,t,o){n.o(e,t)||Object.defineProperty(e,t,{enumerable:!0,get:o})},n.r=function(e){"undefined"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(e,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(e,"__esModule",{value:!0})},n.t=function(e,t){if(1&t&&(e=n(e)),8&t)return e;if(4&t&&"object"==typeof e&&e&&e.__esModule)return e;var o=Object.create(null);if(n.r(o),Object.defineProperty(o,"default",{enumerable:!0,value:e}),2&t&&"string"!=typeof e)for(var r in e)n.d(o,r,function(t){return e[t]}.bind(null,r));return o},n.n=function(e){var t=e&&e.__esModule?function(){return e.default}:function(){return e};return n.d(t,"a",t),t},n.o=function(e,t){return Object.prototype.hasOwnProperty.call(e,t)},n.p="",n(n.s=1)}([function(e,t){e.exports=require("path")},function(e,t,n){"use strict";Object.defineProperty(t,"__esModule",{value:!0}),t.deactivate=t.activate=void 0;const o=n(2),r=n(3),a=n(5),i=r.loadMessageBundle(n(0).join(__dirname,"extension.ts")),s=i(0,null),c=i(1,null),u="extension.node-debug.toggleAutoAttach",l="debug.javascript",d="usePreview",f="jsDebugIpcState",g="debug.node",p="useV3",v="autoAttach";let m,b=Promise.resolve({state:0,transitionData:null});function h(){const e=o.workspace.getConfiguration(g);if(e){let t=e.get(v);t="on"===t?"off":"on";const n=e.inspect(v);let r=o.ConfigurationTarget.Global;n&&(n.workspaceFolderValue?r=o.ConfigurationTarget.WorkspaceFolder:n.workspaceValue?r=o.ConfigurationTarget.Workspace:n.globalValue?r=o.ConfigurationTarget.Global:n.defaultValue&&o.workspace.workspaceFolders&&(r=o.ConfigurationTarget.Workspace)),e.update(v,t,r)}}function y(e){return m?m.show():((m=o.window.createStatusBarItem(o.StatusBarAlignment.Left)).command=u,m.tooltip=i(2,null),m.show(),e.subscriptions.push(m)),m}t.activate=function(e){e.subscriptions.push(o.commands.registerCommand(u,h));const t=[`${g}.${v}`,`${g}.${p}`,`${l}.${d}`];e.subscriptions.push(o.workspace.onDidChangeConfiguration(n=>{t.some(e=>n.affectsConfiguration(e))&&x(e)})),x(e)},t.deactivate=async function(){var e,t;const{state:n,transitionData:o}=await b;await(null===(t=(e=S[n]).exit)||void 0===t?void 0:t.call(e,o))};const S={0:{async enter(e){null===m||void 0===m||m.hide(),e.workspaceState.get(f)&&(await e.workspaceState.update(f,void 0),await o.commands.executeCommand("extension.js-debug.clearAutoAttachVariables"))}},1:{enter(e){y(e).text=c}},3:{async enter(e){const t=y(e),n=process.env.VSCODE_PID,r=n?parseInt(n):0;await o.commands.executeCommand("extension.node-debug.startAutoAttach",r),t.text=s},async exit(){await o.commands.executeCommand("extension.node-debug.stopAutoAttach")}},2:{async enter(e){const t=await async function(e){var t,n;const r=e.workspaceState.get(f),a=(null===(t=o.extensions.getExtension("ms-vscode.js-debug-nightly"))||void 0===t?void 0:t.extensionPath)||(null===(n=o.extensions.getExtension("ms-vscode.js-debug"))||void 0===n?void 0:n.extensionPath);if(r&&r.jsDebugPath===a)return r.ipcAddress;const i=(await o.commands.executeCommand("extension.js-debug.setAutoAttachVariables")).ipcAddress;return await e.workspaceState.update(f,{ipcAddress:i,jsDebugPath:a}),i}(e),n=await new Promise((e,n)=>{const r=a.createServer(e=>{let t=[];e.on("data",e=>t.push(e)),e.on("end",()=>o.commands.executeCommand("extension.js-debug.autoAttachToProcess",JSON.parse(Buffer.concat(t).toString())))}).on("error",n).listen(t,()=>e(r))});return y(e).text=s,n},async exit(e){await new Promise(t=>e.close(t))}}};function x(e){const t=function(){const e=o.workspace.getConfiguration(g);switch(e.get(v)){case"off":return 1;case"on":const t=o.workspace.getConfiguration(l);return e.get(p)||t.get(d)?2:3;case"disabled":default:return 0}}();b=b.then(async({state:n,transitionData:o})=>{var r,a,i,s;if(t===n)return{state:n,transitionData:o};await(null===(a=(r=S[n]).exit)||void 0===a?void 0:a.call(r,o));const c=await(null===(s=(i=S[t]).enter)||void 0===s?void 0:s.call(i,e));return{state:t,transitionData:c}})}},function(e,t){e.exports=require("vscode")},function(e,t,n){"use strict";Object.defineProperty(t,"__esModule",{value:!0});var o,r,a,i,s,c=n(0),u=n(4),l=Object.prototype.toString;function d(e){return void 0!==e}function f(e){return"[object String]"===l.call(e)}function g(e){return JSON.parse(u.readFileSync(e,"utf8"))}function p(e,t){return s&&(e="［"+e.replace(/[aouei]/g,"$&$&")+"］"),0===t.length?e:e.replace(/\{(\d+)\}/g,function(e,n){var o=n[0],r=t[o],a=e;return"string"==typeof r?a=r:"number"!=typeof r&&"boolean"!=typeof r&&void 0!==r&&null!==r||(a=String(r)),a})}function v(e){return function(t,n){for(var o=[],r=2;r<arguments.length;r++)o[r-2]=arguments[r];return function(e){return"[object Number]"===l.call(e)}(t)?t>=e.length?void console.error("Broken localize call found. Index out of bounds. Stacktrace is\n: "+new Error("").stack):p(e[t],o):f(n)?(console.warn("Message "+n+" didn't get externalized correctly."),p(n,o)):void console.error("Broken localize call found. Stacktrace is\n: "+new Error("").stack)}}function m(e,t){for(var n=[],o=2;o<arguments.length;o++)n[o-2]=arguments[o];return p(t,n)}function b(e,t){return a[e]=t,t}function h(e,t){var n,o=c.join(i.cacheRoot,e.id+"-"+e.hash+".json"),r=!1,a=!1;try{return n=JSON.parse(u.readFileSync(o,{encoding:"utf8",flag:"r"})),function(e){var t=new Date;u.utimes(e,t,t,function(){})}(o),n}catch(e){if("ENOENT"===e.code)a=!0;else{if(!(e instanceof SyntaxError))throw e;console.log("Syntax error parsing message bundle: "+e.message+"."),u.unlink(o,function(e){e&&console.error("Deleting corrupted bundle "+o+" failed.")}),r=!0}}if(!(n=function(e,t){var n=i.translationsConfig[e.id];if(n){var o=g(n).contents,r=g(c.join(t,"nls.metadata.json")),a=Object.create(null);for(var s in r){var u=r[s],l=o[e.outDir+"/"+s];if(l){for(var d=[],p=0;p<u.keys.length;p++){var v=u.keys[p],m=l[f(v)?v:v.key];void 0===m&&(m=u.messages[p]),d.push(m)}a[s]=d}else a[s]=u.messages}return a}}(e,t))||r)return n;if(a)try{u.writeFileSync(o,JSON.stringify(n),{encoding:"utf8",flag:"wx"})}catch(e){if("EEXIST"===e.code)return n;throw e}return n}function y(e){try{return function(e){var t=g(c.join(e,"nls.metadata.json")),n=Object.create(null);for(var o in t){var r=t[o];n[o]=r.messages}return n}(e)}catch(e){return void console.log("Generating default bundle from meta data failed.",e)}}function S(e,t){var n;if(!0===i.languagePackSupport&&void 0!==i.cacheRoot&&void 0!==i.languagePackId&&void 0!==i.translationsConfigFile&&void 0!==i.translationsConfig)try{n=h(e,t)}catch(e){console.log("Load or create bundle failed ",e)}if(!n){if(i.languagePackSupport)return y(t);var o=function(e){for(var t=i.locale;t;){var n=c.join(e,"nls.bundle."+t+".json");if(u.existsSync(n))return n;var o=t.lastIndexOf("-");t=o>0?t.substring(0,o):void 0}if(void 0===t&&(n=c.join(e,"nls.bundle.json"),u.existsSync(n)))return n}(t);if(o)try{return g(o)}catch(e){console.log("Loading in the box message bundle failed.",e)}n=y(t)}return n}function x(e){if(!e)return m;var t=c.extname(e);if(t&&(e=e.substr(0,e.length-t.length)),i.messageFormat===o.both||i.messageFormat===o.bundle){var n=function(e){for(var t,n=c.dirname(e);t=c.join(n,"nls.metadata.header.json"),!u.existsSync(t);){var o=c.dirname(n);if(o===n){t=void 0;break}n=o}return t}(e);if(n){var r=c.dirname(n),l=a[r];if(void 0===l)try{var f=JSON.parse(u.readFileSync(n,"utf8"));try{var p=S(f,r);l=b(r,p?{header:f,nlsBundle:p}:null)}catch(e){console.error("Failed to load nls bundle",e),l=b(r,null)}}catch(e){console.error("Failed to read header file",e),l=b(r,null)}if(l){var h=e.substr(r.length+1).replace(/\\/g,"/"),y=l.nlsBundle[h];return void 0===y?(console.error("Messages for file "+e+" not found. See console for details."),function(){return"Messages not found."}):v(y)}}}if(i.messageFormat===o.both||i.messageFormat===o.file)try{var x=g(function(e){var t;if(i.cacheLanguageResolution&&t)t=t;else{if(s||!i.locale)t=".nls.json";else for(var n=i.locale;n;){var o=".nls."+n+".json";if(u.existsSync(e+o)){t=o;break}var r=n.lastIndexOf("-");r>0?n=n.substring(0,r):(t=".nls.json",n=null)}i.cacheLanguageResolution&&(t=t)}return e+t}(e));return Array.isArray(x)?v(x):d(x.messages)&&d(x.keys)?v(x.messages):(console.error("String bundle '"+e+"' uses an unsupported format."),function(){return"File bundle has unsupported format. See console for details"})}catch(e){"ENOENT"!==e.code&&console.error("Failed to load single file bundle",e)}return console.error("Failed to load message bundle for file "+e),function(){return"Failed to load message bundle. See console for details."}}!function(e){e.file="file",e.bundle="bundle",e.both="both"}(o=t.MessageFormat||(t.MessageFormat={})),function(e){e.is=function(e){var t=e;return t&&d(t.key)&&d(t.comment)}}(r||(r={})),function(){if(i={locale:void 0,languagePackSupport:!1,cacheLanguageResolution:!0,messageFormat:o.bundle},f(process.env.VSCODE_NLS_CONFIG))try{var e=JSON.parse(process.env.VSCODE_NLS_CONFIG);if(f(e.locale)&&(i.locale=e.locale.toLowerCase()),function(e){return!0===e||!1===e}(e._languagePackSupport)&&(i.languagePackSupport=e._languagePackSupport),f(e._cacheRoot)&&(i.cacheRoot=e._cacheRoot),f(e._languagePackId)&&(i.languagePackId=e._languagePackId),f(e._translationsConfigFile)){i.translationsConfigFile=e._translationsConfigFile;try{i.translationsConfig=g(i.translationsConfigFile)}catch(t){e._corruptedFile&&u.writeFile(e._corruptedFile,"corrupted","utf8",function(e){console.error(e)})}}}catch(e){}s="pseudo"===i.locale,void 0,a=Object.create(null)}(),t.loadMessageBundle=x,t.config=function(e){return e&&(f(e.locale)&&(i.locale=e.locale.toLowerCase(),void 0,a=Object.create(null)),void 0!==e.messageFormat&&(i.messageFormat=e.messageFormat)),s="pseudo"===i.locale,x}},function(e,t){e.exports=require("fs")},function(e,t){e.exports=require("net")}]));
//# sourceMappingURL=https://ticino.blob.core.windows.net/sourcemaps/d69a79b73808559a91206d73d7717ff5f798f23c/extensions/debug-auto-launch/dist/extension.js.map
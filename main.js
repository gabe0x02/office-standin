const _ = require('lodash');

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
let addr = "/dev/cu.usbmodem14601";
let baud = 115200;
const port = new SerialPort(addr, { baudRate: baud });
const parser = new Readline();
const { spawn } = require('child_process');
const excuseMap = require('./excuses.json');
port.pipe(parser);

let prefix = 'saw something, mood: ';
function handleSerialData (data) {
    console.log(`> ${data}`);
    let startIndex = data.indexOf("start");
    if (startIndex !== -1) {
        value = Number(data.substring(startIndex + 5));
    }

    if(data.startsWith(prefix)) {
        let mood = data.substring(prefix.length).replace(/\r$/, '');
        const excuse = makeExcuse(mood);
        console.log(excuse);
        say(excuse, mood);
    }
}

parser.on('data', handleSerialData);

function makeExcuse(mood) {
    return _.sample(excuseMap.excuses[mood]).replace(/\{(.*?)\}/g, (match, type) => (
        _.sample(excuseMap.substitutions[type])
    ));
}

function say(something, mood)  {
    const voice = {
        lazy: 'Alex',
        frustrated: 'Veena',
        resentful: 'Samantha',
        depressed: 'Daniel',
    }[mood] || 'Alex';
    spawn('say', ['-v', voice, something]);
}

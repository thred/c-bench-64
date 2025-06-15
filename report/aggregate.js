// Node.js script to aggregate benchmark results for each target
// Outputs one JSON file per target with total time and .prg size for each benchmark

const fs = require("fs");
const path = require("path");

const benchmarks = [
    "aes256",
    "crc8",
    "crc16",
    "crc32",
    "dhrystone",
    "fact",
    "pi",
    "pow",
    "puff2",
    "sieve",
    "sieve_bit",
];

const compilers = {
    cc65: ["cc65p", "cc65s"],
    "llvm-mos": ["llvmp", "llvms"],
    oscar64: ["o64p", "o64s"],
    vbcc: ["vbccp", "vbccs"],
};

const baseDir = path.resolve(__dirname, "../benchmarks");

function getLogTime(logPath) {
    if (!fs.existsSync(logPath)) return null;

    const content = fs.readFileSync(logPath, "utf8");
    const match = content.match(/Total time:\s*([\d.]+)/);

    return match ? parseFloat(match[1]) : null;
}

function getPrgSize(prgPath) {
    if (!fs.existsSync(prgPath)) return null;

    return fs.statSync(prgPath).size;
}

function aggregateTarget(target, dir, ext) {
    const result = {};

    for (const bench of benchmarks) {
        const log = path.join(baseDir, dir, "bin", `${bench}-${target}.log`);
        const prg = path.join(baseDir, dir, "bin", `${bench}-${target}.prg`);

        result[bench] = {
            total_time: getLogTime(log),
            prg_size: getPrgSize(prg),
        };
    }

    return result;
}

function main() {
    for (const [dir, targets] of Object.entries(compilers)) {
        for (const target of targets) {
            const data = aggregateTarget(target, dir, "prg");
            const outPath = path.join(__dirname, `public/${target}.json`);

            fs.writeFileSync(outPath, JSON.stringify(data, null, 2));

            console.log(`Wrote ${outPath}`);
        }
    }
}

main();

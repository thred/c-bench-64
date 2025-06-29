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
    cc65: "cc65.json",
    kickc: "kickc.json",
    "llvm-mos": "llvm.json",
    oscar64: "o64.json",
    sdcc: "sdcc.json",
    vbcc: "vbcc.json",
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

function getBuildTime(buildTimePath) {
    if (!fs.existsSync(buildTimePath)) return null;

    return fs.readFileSync(buildTimePath, "utf8").trim();
}

function getBuildVersion(buildVersionPath) {
    if (!fs.existsSync(buildVersionPath)) return null;

    return fs.readFileSync(buildVersionPath, "utf8").trim();
}

function aggregateConfiugration(configuration, dir, ext) {
    const result = {};

    for (const bench of benchmarks) {
        const log = path.join(baseDir, dir, "bin", `${bench}-${configuration}.log`);
        const prg = path.join(baseDir, dir, "bin", `${bench}-${configuration}.prg`);
        const buildTime = path.join(baseDir, dir, "bin", `${bench}-${configuration}.buildtime`);
        const buildVersion = path.join(baseDir, dir, "bin", `${bench}-${configuration}.buildversion`);

        result[bench] = {
            prgSize: getPrgSize(prg),
            totalTime: getLogTime(log),
            buildTime: getBuildTime(buildTime),
            buildVersion: getBuildVersion(buildVersion),
        };
    }

    return result;
}

function main() {
    for (const [dir, compilerKey] of Object.entries(compilers)) {
        const compilerPath = path.join(baseDir, dir, compilerKey);

        if (!fs.existsSync(compilerPath)) {
            console.warn(`Compiler config not found: ${compilerPath}`);
            continue;
        }

        const destJson = path.join(__dirname, "public", compilerKey);

        // Read compiler config
        const compiler = JSON.parse(fs.readFileSync(compilerPath, "utf8"));
        const configurations = Object.keys(compiler.configurations);

        // Aggregate results for each configuration
        for (const configuration of configurations) {
            const data = aggregateConfiugration(configuration, dir, "prg");

            compiler.results = compiler.results || {};
            compiler.results[configuration] = data;
        }

        fs.writeFileSync(destJson, JSON.stringify(compiler, null, 2));
        console.log(`Wrote aggregated results to ${destJson}`);
    }
}

main();

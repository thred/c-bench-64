// Node.js script to aggregate benchmark results for each target
// Outputs one JSON file per target with total time and .prg size for each benchmark

const { output } = require("@angular/core");
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

const compilersKeys = {
    cc65: "cc65",
    kickc: "kickc",
    "llvm-mos": "llvm",
    oscar64: "o64",
    sdcc: "sdcc",
    vbcc: "vbcc",
};

const baseDir = path.resolve(__dirname, "../benchmarks");

function getLogTime(log) {
    const match = log.match(/Total time:\s*([\d.]+)/);

    return match ? parseFloat(match[1]) : null;
}

function getStatus(log) {
    if (log.includes("[FAIL]")) return "fail";
    if (log.includes("[OK]")) return "pass";
    return "unknown";
}

function filterLog(log) {
    const headline = "Displaying 40x25 screen at $0400:\n";
    const displayIdx = log.indexOf(headline);
    
    log = displayIdx !== -1 ? log.slice(displayIdx + headline.length) : "";

    // return log
    //     .split("\n")
    //     .map(line => line.replace(/^[0-9a-fA-F]{4} /, ""))
    //     .join("\n")
    //     .trim();

    return log;
}

function aggregateConfiugration(configuration, dir, ext) {
    const result = {};

    for (const bench of benchmarks) {
        const prgPath = path.join(baseDir, dir, "bin", `${bench}-${configuration}.prg`);

        if (!fs.existsSync(prgPath)) {
            console.error(`  PRG file not found: ${prgPath}`);
            continue;
        }

        const size = fs.statSync(prgPath).size;

        const logPath = path.join(baseDir, dir, "bin", `${bench}-${configuration}.log`);

        if (!fs.existsSync(logPath)) {
            console.error(`  Log file not found: ${logPath}`);
            continue;
        }

        const log = fs.readFileSync(logPath, "utf8");

        result[bench] = {
            size: size,
            time: getLogTime(log),
            status: getStatus(log),
            output: filterLog(log),
        };
    }

    return result;
}

function main() {
    for (const [dir, compilerKey] of Object.entries(compilersKeys)) {
        console.log(`\nProcessing compiler: ${compilerKey} in directory: ${dir}`);

        const compilerPath = path.join(baseDir, dir, compilerKey + ".json");

        if (!fs.existsSync(compilerPath)) {
            console.warn(`Compiler config not found: ${compilerPath}`);
            continue;
        }

        const versionPath = path.join(baseDir, dir, compilerKey + ".version");
        let version = null;
        if (fs.existsSync(versionPath)) {
            version = fs.readFileSync(versionPath, "utf8").trim();
        }

        const datePath = path.join(baseDir, dir, compilerKey + ".date");
        let date = null;
        if (fs.existsSync(datePath)) {
            date = fs.readFileSync(datePath, "utf8").trim();
        }
        
        console.log(`  Version: ${version || "unknown"}`);

        const destJson = path.join(__dirname, "public", compilerKey + ".json");

        // Read compiler config
        const compiler = JSON.parse(fs.readFileSync(compilerPath, "utf8"));
        const configurations = Object.keys(compiler.configurations);

        // Aggregate results for each configuration
        for (const configuration of configurations) {
            const data = aggregateConfiugration(configuration, dir, "prg");

            compiler.version = version || "unknown";
            compiler.date = date || "unknown";
            compiler.results = compiler.results || {};
            compiler.results[configuration] = data;
        }

        fs.writeFileSync(destJson, JSON.stringify(compiler, null, 2));
        console.log(`  Wrote aggregated results to: ${destJson}`);
    }
}

main();

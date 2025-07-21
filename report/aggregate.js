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
    "test_math",
    "test_stdio_file",
    "test_stdio_print",
    "test_stdio_scan",
    "test_stdlib",
    "test_string",
];

const testDefs = {
    "test_math": {
        "name": "math.h",
        "tests": ["floor", "ceil", "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "exp", "log", "log10", "pow", "sqrt"]
    },
    "test_stdio_file": {
        "name": "stdio.h",
        "tests": ["fputc", "fgetc", "fputs", "fgets", "fwrite", "fread"]
    },
    "test_stdio_print": {
        "name": "stdio.h",
        "tests": ["printf-c", "printf-s", "printf-d", "printf-ld", "printf-i", "printf-li", "printf-x", "printf-lx", "printf-X", "printf-lX", "printf-f", "printf-e"]
    },
    "test_stdio_scan": {
        "name": "stdio.h",   
        "tests": ["scanf-c", "scanf-s", "scanf-d",  "scanf-i", "scanf-x", "scanf-X", "scanf-f", "scanf-e"]     
    },
    "test_stdlib": {
        "name": "stdlib.h",
        "tests": ["rand", "srand", "atof", "atoi", "atol", "malloc", "calloc", "realloc", "qsort", "abs", "labs", "div", "ldiv"]
    },
    "test_string": {
        "name": "string.h",
        "tests": ["strlen", "strchr", "strrchr", "strcmp", "strncmp", "strcat", "strncat", "strstr", "memchr", "memset", "memcpy", "memmove"]
    }
};

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
    if (!log) return "unknown";
    if (log.includes("[FAIL]")) return "fail";
    if (log.includes("[MISS]")) return "unsupported";
    if (log.includes("[OK]")) return "pass";
    return "unknown";
}

function filterLog(log) {
    const headline = "Displaying 40x25 screen at $0400:\n";
    const displayIdx = log.indexOf(headline);
    
    log = displayIdx !== -1 ? log.slice(displayIdx + headline.length) : "";

    return log;
}

function aggregateResults(configKey, dir) {
    const results = {};

    for (const bench of benchmarks) {
        const prgName = `${bench}-${configKey}.prg`;
        const prgPath = path.join(baseDir, dir, "bin", prgName);

        if (!fs.existsSync(prgPath)) {
            console.error(`  PRG file not found: ${prgPath}`);
            continue;
        }

        const size = fs.statSync(prgPath).size;

        const logPath = path.join(baseDir, dir, "bin", `${bench}-${configKey}.log`);

        if (!fs.existsSync(logPath)) {
            console.error(`  Log file not found: ${logPath}`);
            continue;
        }

        const log = fs.readFileSync(logPath, "utf8");

        results[bench] = {
            prgName: prgName,
            size: size,
            time: getLogTime(log),
            status: getStatus(log),
            output: filterLog(log),
        };

        const screenshotName = `${bench}-${configKey}.png`;
        const screenshotPath = path.join(baseDir, dir, "bin", screenshotName);

        if (fs.existsSync(screenshotPath)) {
            results[bench].screenshot = screenshotName;
            const destScreenshotPath = path.join(__dirname, "public", screenshotName);
            fs.copyFileSync(screenshotPath, destScreenshotPath);
        }
    }

    return results;
}

function aggregateTests(configKey, dir) {
    const tests = {};

    for (const testKey in testDefs) {
        const testDef = testDefs[testKey];
        const testName = testDef.name;

        tests[testName] = tests[testName] || {};

        const prgName = `${testKey}-${configKey}.prg`;
        const prgPath = path.join(baseDir, dir, "bin", prgName);

        if (!fs.existsSync(prgPath)) {
            console.error(`  PRG file not found: ${prgPath}`);
            continue;
        }

        const size = fs.statSync(prgPath).size;

        const logPath = path.join(baseDir, dir, "bin", `${testKey}-${configKey}.log`);

        if (!fs.existsSync(logPath)) {
            console.error(`  Log file not found for test ${testName}: ${logPath}`);
            continue;
        }

        const log = fs.readFileSync(logPath, "utf8");

        for (const test of testDef.tests) {
            const regex = new RegExp(`#${test}\\b.*`, "m");
            const match = log.match(regex);
            
            tests[testName][test] = {
                status: getStatus(match ? match[0] : null),
                prgName: prgName
            };
        }
    }

    return tests;
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

        // Aggregate results for each configuration
        for (const config of compiler.configs) {

            if (!config.compilerKey) {
                config.compilerKey = compilerKey;
            }

            const results = aggregateResults(config.key, dir);
            const tests = aggregateTests(config.key, dir);

            compiler.version = version || "unknown";
            compiler.date = date || "unknown";
            compiler.results = compiler.results || {};
            compiler.results[config.key] = results;
            compiler.tests = compiler.tests || {};
            compiler.tests[config.key] = tests;
        }

        fs.writeFileSync(destJson, JSON.stringify(compiler, null, 2));
        console.log(`  Wrote aggregated results to: ${destJson}`);
    }
}

main();

import { HttpClient } from "@angular/common/http";
import { computed, inject, Injectable, resource, signal } from "@angular/core";
import { ChartOptions } from "chart.js";
import { firstValueFrom } from "rxjs";
import {
    activeCompilerKeys,
    BenchmarkKey,
    benchmarkKeys,
    BenchmarkResults,
    benchmarks,
    BenchmarkWithResultsMap,
    Compiler,
    CompilerKey,
    compilerKeys,
    CompilersByKey,
    Config,
    ConfigResult,
    ConfigsByKey,
    MethodTestResultsMap,
    Optimization,
    TestKey,
    testKeys,
    TestResult,
} from "./benchmarks";
import { persistentSignal } from "./persistent.signal";

@Injectable({
    providedIn: "root",
})
export class AppService {
    private readonly http = inject(HttpClient);

    private readonly compilersResource = resource({
        loader: () => this.loadCompilers(),
    });

    readonly loading = computed(() => this.compilersResource.isLoading());

    readonly theme = persistentSignal<string>("c-bench-64.theme", () => "c64");

    readonly compilers = computed<CompilersByKey>(() => this.compilersResource.value() ?? {});

    readonly compilersByKey = computed<CompilersByKey>(() => (this.compilersResource.value() as CompilersByKey) ?? {});

    readonly configsByKey = computed<ConfigsByKey>(() => {
        const configsByKey: ConfigsByKey = {};

        for (const compiler of Object.values(this.compilersByKey())) {
            for (const config of compiler.configs) {
                configsByKey[config.key] = config;
            }
        }

        return configsByKey;
    });

    readonly includePerfOpts = signal<boolean>(true);

    readonly includeSizeOpts = signal<boolean>(true);

    readonly selectedCompilerKeys = signal<CompilerKey[]>([...activeCompilerKeys]);

    readonly selectedBenchmarkKeys = signal<BenchmarkKey[]>([
        ...benchmarkKeys.filter((key) => !benchmarks[key].initiallyDisabled),
    ]);

    readonly selectedTestKeys = signal<TestKey[]>([...testKeys]);

    readonly selectedConfigKeys = computed(() => {
        const selectedCompilerKeys = this.selectedCompilerKeys();
        const configKeys: string[] = [];

        for (const compilerKey of selectedCompilerKeys) {
            const compiler = this.compilersByKey()[compilerKey];

            if (!compiler) {
                continue;
            }

            for (const config of compiler.configs) {
                if (this.isConfigSelected(config.key)) {
                    configKeys.push(config.key);
                }
            }
        }

        return configKeys;
    });

    readonly includePerfOptOnly = signal<boolean>(false);

    readonly includeSizeOptOnly = signal<boolean>(false);

    readonly benchmarkWithResultsMap = computed<BenchmarkWithResultsMap>(() => {
        const compilers = this.compilersByKey();
        const map: Partial<BenchmarkWithResultsMap> = {};

        for (const benchmarkKey of this.selectedBenchmarkKeys()) {
            const results = this.collectBenchmarkResults(compilers, benchmarkKey);

            map[benchmarkKey] = {
                key: benchmarkKey,
                name: benchmarks[benchmarkKey].name,
                shortName: benchmarks[benchmarkKey].shortName,
                description: benchmarks[benchmarkKey].description,
                note: benchmarks[benchmarkKey].note,
                author: benchmarks[benchmarkKey].author,
                url: benchmarks[benchmarkKey].url,
                footnotes: benchmarks[benchmarkKey].footnotes,
                results,
            };
        }

        return map as BenchmarkWithResultsMap;
    });

    isCompilerSelected(compilerKey: CompilerKey): boolean {
        return this.selectedCompilerKeys().includes(compilerKey);
    }

    setCompilerSelected(compilerKey: CompilerKey, value: boolean): void {
        const selectedCompilers = [...this.selectedCompilerKeys()];

        if (value) {
            if (!selectedCompilers.includes(compilerKey)) {
                selectedCompilers.push(compilerKey);
            }
        } else {
            const index = selectedCompilers.indexOf(compilerKey);

            if (index !== -1) {
                selectedCompilers.splice(index, 1);
            }
        }

        this.selectedCompilerKeys.set(selectedCompilers);
    }

    toggleCompilerSelection(compilerKey: CompilerKey): void {
        const selectedCompilers = [...this.selectedCompilerKeys()];
        const index = selectedCompilers.indexOf(compilerKey);

        if (index === -1) {
            selectedCompilers.push(compilerKey);
        } else {
            selectedCompilers.splice(index, 1);
        }

        this.selectedCompilerKeys.set(selectedCompilers);
    }

    isBenchmarkSelected(benchmarkKey: BenchmarkKey): boolean {
        return this.selectedBenchmarkKeys().includes(benchmarkKey);
    }

    setBenchmarkSelected(benchmarkKey: BenchmarkKey, value: boolean): void {
        const selectedBenchmarks = [...this.selectedBenchmarkKeys()];

        if (value) {
            if (!selectedBenchmarks.includes(benchmarkKey)) {
                selectedBenchmarks.push(benchmarkKey);
            }
        } else {
            const index = selectedBenchmarks.indexOf(benchmarkKey);

            if (index !== -1) {
                selectedBenchmarks.splice(index, 1);
            }
        }

        this.selectedBenchmarkKeys.set(selectedBenchmarks);
    }

    toggleBenchmarkSelection(benchmarkKey: BenchmarkKey): void {
        const selectedBenchmarks = [...this.selectedBenchmarkKeys()];
        const index = selectedBenchmarks.indexOf(benchmarkKey);

        if (index === -1) {
            selectedBenchmarks.push(benchmarkKey);
        } else {
            selectedBenchmarks.splice(index, 1);
        }

        this.selectedBenchmarkKeys.set(selectedBenchmarks);
    }

    isTestSelected(benchmarkKey: TestKey): boolean {
        return this.selectedTestKeys().includes(benchmarkKey);
    }

    setTestSelected(benchmarkKey: TestKey, value: boolean): void {
        const selectedTests = [...this.selectedTestKeys()];

        if (value) {
            if (!selectedTests.includes(benchmarkKey)) {
                selectedTests.push(benchmarkKey);
            }
        } else {
            const index = selectedTests.indexOf(benchmarkKey);

            if (index !== -1) {
                selectedTests.splice(index, 1);
            }
        }

        this.selectedTestKeys.set(selectedTests);
    }

    toggleTestSelection(benchmarkKey: TestKey): void {
        const selectedTests = [...this.selectedTestKeys()];
        const index = selectedTests.indexOf(benchmarkKey);

        if (index === -1) {
            selectedTests.push(benchmarkKey);
        } else {
            selectedTests.splice(index, 1);
        }

        this.selectedTestKeys.set(selectedTests);
    }

    isConfigSelected(configKey: string): boolean {
        const config = this.findConfigByKey(configKey);

        if (!config) {
            return false;
        }

        if (!this.isOptimizationSelected(config.optimization)) {
            return false;
        }

        return this.isCompilerSelected(config.compilerKey);
    }

    isOptimizationSelected(optimization: Optimization): boolean {
        if (optimization === "performance") {
            return this.includePerfOpts();
        } else if (optimization === "size") {
            return this.includeSizeOpts();
        }

        return true;
    }

    private collectBenchmarkResults(compilers: CompilersByKey, benchmarkKey: BenchmarkKey): BenchmarkResults {
        const benchmarkResults: Partial<BenchmarkResults> = {};

        for (const compilerKey of this.selectedCompilerKeys()) {
            const compiler = compilers[compilerKey];

            if (!compiler) {
                continue;
            }

            for (const config of compiler.configs) {
                const results = compiler.results[config.key];

                if (
                    results &&
                    results[benchmarkKey] &&
                    results[benchmarkKey].time !== null &&
                    results[benchmarkKey].size !== null
                ) {
                    benchmarkResults[config.key] = Object.assign(
                        {
                            time: results[benchmarkKey].time,
                            size: results[benchmarkKey].size,
                            status: results[benchmarkKey].status ?? "unknown",
                            output: results[benchmarkKey].output ?? undefined,
                            screenshot: results[benchmarkKey].screenshot ?? undefined,
                        },
                        config,
                    );
                }
            }
        }

        return benchmarkResults as BenchmarkResults;
    }

    findConfigByKey(configKey: string): Config | undefined {
        return this.configsByKey()[configKey];
    }

    findConfigNameByKey(configKey: string): string {
        return this.findConfigByKey(configKey)?.name || configKey;
    }

    findCompilerByConfigKey(configKey: string): Compiler | undefined {
        const config = this.findConfigByKey(configKey);

        if (!config) {
            return undefined;
        }

        return this.compilersByKey()[config.compilerKey];
    }

    findMethodTestResultsMapByTestAndConfigKey(testKey: TestKey, configKey: string): MethodTestResultsMap | undefined {
        const configurationTest = this.findCompilerByConfigKey(configKey)?.tests[configKey];

        if (!configurationTest) {
            return undefined;
        }

        return configurationTest[testKey];
    }

    findTestResultByTestAndConfigKeyAndMethod(
        testKey: TestKey,
        configKey: string,
        method: string,
    ): TestResult | undefined {
        const results = this.findMethodTestResultsMapByTestAndConfigKey(testKey, configKey);

        if (!results) {
            return undefined;
        }

        return results[method];
    }

    findConfigResultByPrgName(prgName: string): ConfigResult | undefined {
        const compilers = this.compilers();

        for (const compiler of Object.values(compilers)) {
            if (!compiler.results) {
                continue;
            }

            for (const result of Object.values(compiler.results)) {
                for (const configResult of Object.values(result)) {
                    if (configResult.prgName === prgName) {
                        return configResult;
                    }
                }
            }
        }

        return undefined;
    }

    private async loadCompilers(): Promise<CompilersByKey> {
        const compilers: CompilersByKey = {};

        // Load each target's JSON file
        for (const compilerKey of compilerKeys) {
            try {
                const compiler = await firstValueFrom(this.http.get<Compiler>(`${compilerKey}.json`));

                if (!compiler) {
                    console.warn(`No data received for ${compilerKey}.json`);
                    continue;
                }

                compilers[compilerKey] = compiler;
            } catch (error) {
                console.warn(`Failed to load ${compilerKey}.json:`, error);
            }
        }

        return compilers;
    }

    createChartOptions(indexAxis: "x" | "y", xAxisText: string, fractionDigits: number, unit: string): ChartOptions {
        const gridColor = this.theme() === "cbm" ? "#888888" : "#000000";

        return {
            indexAxis,
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                x: {
                    beginAtZero: true,
                    grid: {
                        color: gridColor,
                        lineWidth: 2,
                    },
                    title: {
                        display: true,
                        text: xAxisText,
                        color: "#706deb",
                        font: {
                            family: "Oxanium",
                            size: 12,
                            weight: "bold",
                        },
                    },
                    ticks: {
                        color: "#706deb",
                        font: {
                            family: "Oxanium",
                            size: 12,
                            weight: "bold",
                        },
                    },
                    border: {
                        color: gridColor,
                    },
                },
                y: {
                    grid: {
                        color: gridColor,
                        lineWidth: 2,
                    },
                    ticks: {
                        color: "#706deb",
                        font: {
                            family: "Oxanium",
                            size: 12,
                            weight: "bold",
                        },
                    },
                    border: {
                        color: gridColor,
                    },
                },
            },
            plugins: {
                legend: {
                    display: false,
                    labels: {
                        font: {
                            family: "Oxanium",
                            size: 12,
                            weight: "bold",
                        },
                    },
                },
                tooltip: {
                    callbacks: {
                        label: (context: any) => {
                            const value: number = context.parsed[context.chart.options.indexAxis === "y" ? "x" : "y"];
                            return `${context.dataset.label ?? ""}: ${value.toFixed(fractionDigits)} ${unit}`;
                        },
                    },
                },
            },
            layout: {
                padding: {
                    left: 10,
                    right: 10,
                },
            },
        };
    }
}

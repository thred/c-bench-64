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
    Compilers,
    Configuration,
    Configurations,
} from "./benchmarks";

@Injectable({
    providedIn: "root",
})
export class AppService {
    private readonly http = inject(HttpClient);

    private readonly compilersResource = resource({
        loader: () => this.loadCompilers(),
    });

    readonly loading = computed(() => this.compilersResource.isLoading());

    readonly compilers = computed<Compilers>(() => (this.compilersResource.value() as Compilers) ?? {});

    readonly confiugrations = computed<Configurations>(() => {
        const configurations: Configurations = {};

        for (const compiler of Object.values(this.compilers())) {
            for (const [key, configuration] of Object.entries(compiler.configurations)) {
                configurations[key] = configuration;
            }
        }

        return configurations;
    });

    readonly selectedCompilerKeys = signal<CompilerKey[]>([...activeCompilerKeys]);

    readonly selectedBenchmarkKeys = signal<BenchmarkKey[]>([...benchmarkKeys]);

    readonly includePerfOptOnly = signal<boolean>(false);

    readonly includeSizeOptOnly = signal<boolean>(false);

    readonly benchmarkWithResultsMap = computed<BenchmarkWithResultsMap>(() => {
        const compilers = this.compilers();
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

    getConfiguration(configurationKey: string): Configuration | undefined {
        return this.confiugrations()[configurationKey];
    }

    getConfigurationName(configurationKey: string): string {
        return this.getConfiguration(configurationKey)?.name || configurationKey;
    }

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

    private collectBenchmarkResults(compilers: Compilers, benchmarkKey: BenchmarkKey): BenchmarkResults {
        const benchmarkResults: Partial<BenchmarkResults> = {};

        for (const compilerKey of this.selectedCompilerKeys()) {
            const compiler = compilers[compilerKey];

            if (!compiler) {
                continue;
            }

            for (const configurationKey of Object.keys(compiler.configurations)) {
                const configuration = compiler.configurations[configurationKey];
                const results = compiler.results[configurationKey];

                if (
                    results &&
                    results[benchmarkKey] &&
                    results[benchmarkKey].time !== null &&
                    results[benchmarkKey].size !== null
                ) {
                    benchmarkResults[configurationKey] = Object.assign(
                        {
                            time: results[benchmarkKey].time,
                            size: results[benchmarkKey].size,
                            status: results[benchmarkKey].status ?? "unknown",
                            output: results[benchmarkKey].output ?? undefined,
                        },
                        configuration,
                    );
                }
            }
        }

        return benchmarkResults as BenchmarkResults;
    }

    private async loadCompilers(): Promise<Compilers> {
        const compilers: Compilers = {};

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
        return {
            indexAxis,
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                x: {
                    beginAtZero: true,
                    grid: {
                        color: "#000000",
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
                        color: "#000000",
                    },
                },
                y: {
                    grid: {
                        color: "#000000",
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
                        color: "#000000",
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

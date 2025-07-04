import { HttpClient } from "@angular/common/http";
import { computed, inject, Injectable, resource } from "@angular/core";
import { ChartOptions } from "chart.js";
import { firstValueFrom } from "rxjs";
import {
    BenchmarkKey,
    benchmarkKeys,
    BenchmarkResults,
    benchmarks,
    BenchmarkWithResultsMap,
    Compiler,
    CompilerKey,
    compilerKeys,
    Compilers,
} from "./benchmarks";

@Injectable({
    providedIn: "root",
})
export class AppService {
    private readonly http = inject(HttpClient);

    private readonly compilersResource = resource({
        loader: () => this.loadCompilers(),
    });

    readonly compilers = computed<Compilers>(() => (this.compilersResource.value() as Compilers) ?? {});

    readonly benchmarkWithResultsMap = computed<BenchmarkWithResultsMap>(() => {
        const compilers = this.compilers();
        const map: Partial<BenchmarkWithResultsMap> = {};

        for (const benchmarkKey of benchmarkKeys) {
            const results = this.collectBenchmarkResults(compilers, benchmarkKey);

            map[benchmarkKey] = {
                key: benchmarkKey,
                name: benchmarks[benchmarkKey].name,
                description: benchmarks[benchmarkKey].description,
                results,
            };
        }

        return map as BenchmarkWithResultsMap;
    });

    private collectBenchmarkResults(compilers: Compilers, benchmarkKey: BenchmarkKey): BenchmarkResults {
        const benchmarkResults: Partial<BenchmarkResults> = {};

        for (const compilerKey of Object.keys(compilers) as CompilerKey[]) {
            const compiler = compilers[compilerKey]!;

            for (const configurationKey of Object.keys(compiler.configurations)) {
                const configuration = compiler.configurations[configurationKey];
                const results = compiler.results[configurationKey]!;

                if (
                    results[benchmarkKey] &&
                    results[benchmarkKey].totalTime !== null &&
                    results[benchmarkKey].prgSize !== null
                ) {
                    benchmarkResults[configurationKey] = Object.assign(
                        {
                            time: results[benchmarkKey].totalTime,
                            size: results[benchmarkKey].prgSize,
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

    createChartOptions(xAxisText: string): ChartOptions {
        return {
            indexAxis: "y" as const,
            aspectRatio: 3 / 1,
            scales: {
                x: {
                    beginAtZero: true,
                    grid: {
                        color: "#000000",
                    },
                    title: {
                        display: true,
                        text: xAxisText,
                        color: "#706deb",
                        font: {
                            family: "Oxanium",
                            size: 14,
                        },
                    },
                    ticks: {
                        color: "#706deb",
                        font: {
                            family: "Oxanium",
                            size: 14,
                        },
                    },
                    border: {
                        color: "#000000",
                    },
                },
                y: {
                    grid: {
                        color: "#000000",
                    },
                    ticks: {
                        color: "#706deb",
                        font: {
                            family: "Oxanium",
                            size: 14,
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
                            size: 14,
                        },
                    },
                },
            },
        };
    }
}

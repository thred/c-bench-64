import { Component, computed, inject, input, signal } from "@angular/core";
import { FormsModule } from "@angular/forms";
import { AppService } from "../app.service";
import { BenchmarkKey, BenchmarkResults, Optimization } from "../benchmarks";
import { OutputComponent } from "../output/output.component";
import { SizeBenchmarkComponent } from "../size-benchmark/size-benchmark.component";
import { TimeBenchmarkComponent } from "../time-benchmark/time-benchmark.component";

@Component({
    selector: "app-benchmark",
    imports: [OutputComponent, TimeBenchmarkComponent, SizeBenchmarkComponent, FormsModule],
    templateUrl: "./benchmark.component.html",
    styleUrl: "./benchmark.component.scss",
})
export class BenchmarkComponent {
    private readonly service = inject(AppService);

    readonly key = input.required<BenchmarkKey>();

    readonly benchmark = computed(() => this.service.benchmarkWithResultsMap()[this.key()]);

    readonly timeBenchmark = computed(() =>
        BenchmarkComponent.filterConfigurations(
            this.benchmark()?.results,
            ["none", "performance"],
            this.includePerfOptOnly(),
        ),
    );

    readonly sizeBenchmark = computed(() =>
        BenchmarkComponent.filterConfigurations(this.benchmark()?.results, ["none", "size"], this.includeSizeOptOnly()),
    );

    readonly configurationKeys = computed(() => Object.keys(this.benchmark()?.results || {}));

    readonly failedBenchmarks = computed(() =>
        Object.keys(this.benchmark()?.results)
            .filter((key) => this.benchmark()?.results[key].status !== "pass")
            .map((key) => `"${this.benchmark()?.results[key].name}"`)
            .join(", "),
    );

    readonly selectedOutputConfigurationKey = signal<string | undefined>(undefined);

    readonly selectedOutput = computed(() =>
        this.selectedOutputConfigurationKey()
            ? this.benchmark()?.results[this.selectedOutputConfigurationKey()!]?.output
            : undefined,
    );

    get includePerfOptOnly() {
        return this.service.includePerfOptOnly;
    }

    get includeSizeOptOnly() {
        return this.service.includeSizeOptOnly;
    }

    toggleOutputConfigurationKey(configurationKey: string): void {
        if (this.selectedOutputConfigurationKey() === configurationKey) {
            this.selectedOutputConfigurationKey.set(undefined);
        } else {
            this.selectedOutputConfigurationKey.set(configurationKey);
        }
    }

    getConfigurationName(configurationKey: string): string {
        return this.benchmark()?.results[configurationKey]?.name || configurationKey;
    }

    static filterConfigurations(
        configurations: BenchmarkResults,
        optimizations: Optimization[],
        enabled: boolean,
    ): BenchmarkResults {
        if (!enabled || !optimizations.length) {
            return configurations;
        }

        const filtered: BenchmarkResults = {};

        for (const [key, result] of Object.entries(configurations)) {
            if (result && optimizations.includes(result.optimization)) {
                filtered[key] = result;
            }
        }

        return filtered;
    }
}

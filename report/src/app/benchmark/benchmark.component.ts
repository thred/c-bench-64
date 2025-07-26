import { Component, computed, inject, input, signal } from "@angular/core";
import { FormsModule } from "@angular/forms";
import { AppService } from "../app.service";
import { BenchmarkKey, BenchmarkResults, benchmarks, Optimization } from "../benchmarks";
import { OutputComponent } from "../output/output.component";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { SizeBenchmarkComponent } from "../size-benchmark/size-benchmark.component";
import { TimeBenchmarkComponent } from "../time-benchmark/time-benchmark.component";

@Component({
    selector: "app-benchmark",
    imports: [
        OutputComponent,
        TimeBenchmarkComponent,
        SizeBenchmarkComponent,
        FormsModule,
        SectionComponent,
        SectionCollapserDirective,
    ],
    templateUrl: "./benchmark.component.html",
    styleUrl: "./benchmark.component.scss",
})
export class BenchmarkComponent {
    private readonly service = inject(AppService);

    readonly key = input.required<BenchmarkKey>();

    readonly chartWidth = input<number>(1024);

    readonly widthStyle = computed(() => `${this.chartWidth() - 24}px`);

    readonly heightStyle = computed(() => `${Math.max(376, this.chartWidth() / 3)}px`);

    readonly benchmark = computed(() => {
        const key = this.key();
        const results: BenchmarkResults = {};
        const configKeys = this.service.selectedConfigKeys();

        for (const configKey of configKeys) {
            const result = this.service.findConfigResultByBenchmarkAndConfigKey(this.key(), configKey);
            const config = this.service.findConfigByKey(configKey);

            if (result && result.time !== null && result.size !== null) {
                results[configKey] = Object.assign(
                    {
                        time: result.time,
                        size: result.size,
                        status: result.status ?? "unknown",
                        output: result.output ?? undefined,
                        screenshot: result.screenshot ?? undefined,
                    },
                    config,
                );
            }
        }

        return {
            key,
            name: benchmarks[key].name,
            shortName: benchmarks[key].shortName,
            description: benchmarks[key].description,
            note: benchmarks[key].note,
            author: benchmarks[key].author,
            url: benchmarks[key].url,
            footnotes: benchmarks[key].footnotes,
            results,
        };
    });

    readonly configKeys = computed(() => this.service.selectedConfigKeys());

    readonly failedBenchmarks = computed(() =>
        Object.keys(this.benchmark()?.results)
            .filter((key) => this.benchmark()?.results[key].status !== "pass")
            .map((key) => `"${this.benchmark()?.results[key].name}"`)
            .join(", "),
    );

    readonly selectedOutputConfigKey = signal<string | undefined>(undefined);

    readonly selectedOutput = computed(() =>
        this.selectedOutputConfigKey() ? this.benchmark()?.results[this.selectedOutputConfigKey()!]?.output : undefined,
    );

    toggleOutputConfigKey(configKey: string): void {
        if (this.selectedOutputConfigKey() === configKey) {
            this.selectedOutputConfigKey.set(undefined);
        } else {
            this.selectedOutputConfigKey.set(configKey);
        }
    }

    getConfigName(configKey: string): string {
        return this.benchmark()?.results[configKey]?.name || configKey;
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

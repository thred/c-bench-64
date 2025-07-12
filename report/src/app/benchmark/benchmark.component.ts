import { Component, computed, inject, input, signal } from "@angular/core";
import { AppService } from "../app.service";
import { BenchmarkKey } from "../benchmarks";
import { OutputComponent } from "../output/output.component";
import { SizeBenchmarkComponent } from "../size-benchmark/size-benchmark.component";
import { TimeBenchmarkComponent } from "../time-benchmark/time-benchmark.component";

@Component({
    selector: "app-benchmark",
    imports: [OutputComponent, TimeBenchmarkComponent, SizeBenchmarkComponent],
    templateUrl: "./benchmark.component.html",
    styleUrl: "./benchmark.component.scss",
})
export class BenchmarkComponent {
    private readonly service = inject(AppService);

    readonly key = input.required<BenchmarkKey>();

    readonly benchmark = computed(() => this.service.benchmarkWithResultsMap()[this.key()]);

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
}

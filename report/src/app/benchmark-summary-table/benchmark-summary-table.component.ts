import { Component, computed, inject, input } from "@angular/core";
import { AppService } from "../app.service";
import { BenchmarkKey, benchmarks, BenchmarkSummary } from "../benchmarks";

@Component({
    selector: "app-benchmark-summary-table",
    imports: [],
    templateUrl: "./benchmark-summary-table.component.html",
    styleUrl: "./benchmark-summary-table.component.scss",
})
export class BenchmarkSummaryTableComponent {
    private readonly service = inject(AppService);

    readonly summary = input.required<BenchmarkSummary>();

    readonly decimalDigits = input<number>(3);

    readonly unit = input.required<string>();

    readonly benchmarkKeys = computed(() => Object.keys(this.summary()) as BenchmarkKey[]);

    readonly benchmarks = computed(() =>
        this.benchmarkKeys().map((key) => this.service.benchmarkWithResultsMap()[key]),
    );

    readonly configurationKeys = computed(() => {
        const keys: string[] = [];

        for (const benchmarkKey of this.benchmarkKeys()) {
            const benchmarkSummary = this.summary()[benchmarkKey];

            if (benchmarkSummary) {
                keys.push(...Object.keys(benchmarkSummary));
            }
        }

        return Array.from(new Set(keys));
    });

    getBenchmarkShortName(benchmarkKey: BenchmarkKey): string {
        return benchmarks[benchmarkKey].shortName;
    }

    getConfigurationName(configurationKey: string): string {
        return this.service.getConfigurationName(configurationKey);
    }

    getValue(benchmarkKey: BenchmarkKey, configurationKey: string): string {
        const item = this.summary()[benchmarkKey]?.[configurationKey];

        if (!item || isNaN(item.value)) {
            return "";
        }

        return `${item.value.toFixed(this.decimalDigits())} ${this.unit()}`;
    }

    getClassName(benchmarkKey: BenchmarkKey, configurationKey: string): string {
        const item = this.summary()[benchmarkKey]?.[configurationKey];

        if (!item) {
            return "unknown";
        }

        if (item.status !== "pass") {
            return item.status;
        }

        if (item.lowestValue) {
            return "lowest-value";
        }

        if (item.hightestValue) {
            return "highest-value";
        }

        return item.status;
    }
}

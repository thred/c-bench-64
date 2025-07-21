import { CommonModule } from "@angular/common";
import { Component, computed, inject } from "@angular/core";
import { FormsModule } from "@angular/forms";
import { AppService } from "../app.service";
import { BenchmarkSummaryTableComponent } from "../benchmark-summary-table/benchmark-summary-table.component";
import { BenchmarkResult, BenchmarkSummary, BenchmarkSummaryItem } from "../benchmarks";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";

@Component({
    selector: "app-benchmark-summary",
    imports: [CommonModule, BenchmarkSummaryTableComponent, FormsModule, SectionComponent, SectionCollapserDirective],
    templateUrl: "./benchmark-summary.component.html",
    styleUrl: "./benchmark-summary.component.scss",
})
export class BenchmarkSummaryComponent {
    private readonly service = inject(AppService);

    get includePerfOptOnly() {
        return this.service.includePerfOptOnly;
    }

    get includeSizeOptOnly() {
        return this.service.includeSizeOptOnly;
    }

    readonly timeSummary = computed<BenchmarkSummary>(() => {
        const allResults: Partial<BenchmarkSummary> = {};

        for (const benchmarkKey of this.service.selectedBenchmarkKeys()) {
            const results = (allResults[benchmarkKey] = allResults[benchmarkKey] ?? {});
            const benuchmarkResults = this.service.benchmarkWithResultsMap()[benchmarkKey];

            for (const configKey of Object.keys(benuchmarkResults.results)) {
                const configuration = this.service.findConfigByKey(configKey);

                if (!configuration || (this.includePerfOptOnly() && configuration.optimization === "size")) {
                    continue;
                }

                const currentResult: BenchmarkResult | undefined = benuchmarkResults.results[configKey];

                if (!currentResult || currentResult.time === undefined) {
                    results[configKey] = {
                        value: NaN,
                        status: "unknown",
                        lowestValue: false,
                        hightestValue: false,
                    };
                } else {
                    results[configKey] = {
                        value: currentResult.time,
                        status: currentResult.status,
                        lowestValue: false,
                        hightestValue: false,
                    };
                }
            }

            BenchmarkSummaryComponent.updateLowestAndHighestValues(results);
        }

        return allResults as BenchmarkSummary;
    });

    readonly sizeSummary = computed<BenchmarkSummary>(() => {
        const allResults: Partial<BenchmarkSummary> = {};

        for (const benchmarkKey of this.service.selectedBenchmarkKeys()) {
            const results = (allResults[benchmarkKey] = allResults[benchmarkKey] ?? {});
            const benuchmarkResults = this.service.benchmarkWithResultsMap()[benchmarkKey];

            for (const configKey of Object.keys(benuchmarkResults.results)) {
                const configuration = this.service.findConfigByKey(configKey);

                if (!configuration || (this.includeSizeOptOnly() && configuration.optimization === "performance")) {
                    continue;
                }

                const currentResult: BenchmarkResult | undefined = benuchmarkResults.results[configKey];

                if (!currentResult || currentResult.size === undefined) {
                    results[configKey] = {
                        value: NaN,
                        status: "unknown",
                        lowestValue: false,
                        hightestValue: false,
                    };
                } else {
                    results[configKey] = {
                        value: currentResult.size / 1024,
                        status: currentResult.status,
                        lowestValue: false,
                        hightestValue: false,
                    };
                }
            }

            BenchmarkSummaryComponent.updateLowestAndHighestValues(results);
        }

        return allResults as BenchmarkSummary;
    });

    private static updateLowestAndHighestValues(benchmarkSummary: { [configKey: string]: BenchmarkSummaryItem }): void {
        const values = Object.values(benchmarkSummary)
            .filter((item) => !isNaN(item.value) && item.status === "pass")
            .map((item) => item.value);

        if (values.length === 0) {
            return;
        }

        const lowestValue = Math.min(...values);
        const highestValue = Math.max(...values);

        for (const [configKey, item] of Object.entries(benchmarkSummary)) {
            if (isNaN(item.value) || item.status !== "pass") {
                continue;
            }

            item.lowestValue = item.value === lowestValue;
            item.hightestValue = item.value === highestValue;
        }
    }
}

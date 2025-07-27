import { Component, computed, inject, input } from "@angular/core";
import { AppService } from "../app.service";
import { BenchmarkKey, benchmarks, BenchmarkSummaryItem } from "../benchmarks";
import { ScreenshotService } from "../screenshot/screenshot.service";

@Component({
    selector: "[appBenchmarkSummaryRow]",
    imports: [],
    templateUrl: "./benchmark-summary-row.component.html",
    styleUrl: "./benchmark-summary-row.component.scss",
})
export class BenchmarkSummaryRowComponent {
    private readonly service = inject(AppService);
    private readonly screenshotService = inject(ScreenshotService);

    readonly benchmarkKey = input.required<BenchmarkKey>();

    readonly benchmarkShortName = computed(() => benchmarks[this.benchmarkKey()].shortName);

    readonly type = input.required<"time" | "size">();

    readonly configKeys = input.required<string[]>();

    readonly benchmarkResults = computed(() => {
        const testResults: BenchmarkSummaryItem[] = [];

        for (const configKey of this.configKeys()) {
            const result = this.service.findConfigResultByBenchmarkAndConfigKey(this.benchmarkKey(), configKey);
            let value = this.type() === "time" ? result?.time : result?.size ? result.size / 1024 : undefined;
            const status = result?.status ?? "unknown";

            testResults.push({
                configKey,
                value: value ?? NaN,
                status: status,
                lowestValue: false,
                hightestValue: false,
                output: result?.output ?? undefined,
                screenshot: result?.screenshot,
            });
        }

        // Find lowest and highest values among valid results
        const validValues = testResults.map((r) => r.value).filter((v) => !isNaN(v));

        const min = Math.min(...validValues);
        const max = Math.max(...validValues);

        for (const result of testResults) {
            if (!isNaN(result.value)) {
                result.lowestValue = result.value === min;
                result.hightestValue = result.value === max;
            }
        }

        return testResults;
    });

    getClassName(testResult: BenchmarkSummaryItem): string {
        if (!testResult) {
            return "unknown";
        }

        if (testResult.status !== "pass") {
            return testResult.status;
        }

        if (testResult.lowestValue) {
            return "lowest-value";
        }

        if (testResult.hightestValue) {
            return "highest-value";
        }

        return testResult.status;
    }

    getValue(testResult: BenchmarkSummaryItem): string {
        if (!testResult || isNaN(testResult.value)) {
            return "";
        }

        return `${testResult.value.toFixed(this.type() === "time" ? 1 : 1)} ${this.type() === "time" ? "s" : "KB"}`;
    }

    gotoBenchmark(): void {
        const href = `#benchmark-${this.benchmarkKey().replace(/\./g, "\\.")}`;
        console.log("Navigating to benchmark:", href);
        const element = document.querySelector(href!);

        if (element) {
            element.scrollIntoView({ behavior: "smooth" });
        }
    }

    gotoScreenshot(testResult: BenchmarkSummaryItem): void {
        if (testResult.screenshot) {
            this.screenshotService.openScreenshot(testResult.screenshot);
        } else {
            console.warn("No screenshot available for benchmark result:", testResult);
        }
    }
}

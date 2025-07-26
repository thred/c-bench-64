import { Component, computed, inject, input } from "@angular/core";
import { AppService } from "../app.service";
import { TestKey, TestResult } from "../benchmarks";
import { ScreenshotService } from "../screenshot/screenshot.service";
import { StatusComponent } from "../status/status.component";

@Component({
    selector: "[appTestResultRow]",
    imports: [StatusComponent],
    templateUrl: "./test-result-row.component.html",
    styleUrl: "./test-result-row.component.scss",
})
export class TestResultRowComponent {
    private readonly service = inject(AppService);
    private readonly screenshotService = inject(ScreenshotService);

    readonly testKey = input.required<TestKey>();

    readonly method = input.required<string>();

    readonly configKeys = input.required<string[]>();

    readonly testResults = computed(() => {
        const testResults: TestResult[] = [];

        for (const configKey of this.configKeys()) {
            const results = this.service.findTestResultByTestAndConfigKeyAndMethod(
                this.testKey(),
                configKey,
                this.method(),
            );

            if (results) {
                testResults.push(results);
            } else {
                testResults.push({ status: "unknown" });
            }
        }

        return testResults;
    });

    hasPrgName(result: TestResult): boolean {
        return result.prgName !== undefined && result.prgName.length > 0;
    }

    getTitleAttr(result: TestResult): string {
        let title;

        switch (result.status) {
            case "pass":
                title = "Test passed.";
                break;

            case "fail":
                title = "Test failed.";
                break;

            case "unsupported":
                title = "Features is not supported.";
                break;

            case "disabled":
                title = "Feature is disabled by compiler options.";
                break;

            case "unknown":
                title = "Test execution produced no result (may indicate a crash or compile error).";
                break;

            default:
                return "";
        }

        if (result.prgName) {
            title += " Click to see test output!";
        } else {
            title += " There is no output available.";
        }

        return title;
    }

    openScreenshot(result: TestResult): void {
        if (result.prgName) {
            const configResult = this.service.findConfigResultByPrgName(result.prgName);

            if (configResult) {
                this.screenshotService.openScreenshot(configResult.screenshot);
            } else {
                console.warn("No config result found for program name:", result.prgName);
            }
        }
    }
}

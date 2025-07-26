import { Component, computed, inject, input } from "@angular/core";
import { AppService } from "../app.service";
import { BenchmarkSummaryRowComponent } from "../benchmark-summary-row/benchmark-summary-row.component";
import { BenchmarkKey } from "../benchmarks";

@Component({
    selector: "app-benchmark-summary-table",
    imports: [BenchmarkSummaryRowComponent],
    templateUrl: "./benchmark-summary-table.component.html",
    styleUrl: "./benchmark-summary-table.component.scss",
})
export class BenchmarkSummaryTableComponent {
    private readonly service = inject(AppService);

    readonly benchmarkKeys = input.required<BenchmarkKey[]>();

    readonly type = input.required<"time" | "size">();

    readonly configKeys = input.required<string[]>();

    readonly configNames = computed(() => this.configKeys().map((key) => this.service.findConfigNameByKey(key)));
}

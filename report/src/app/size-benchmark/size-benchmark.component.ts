import { Component, computed, ElementRef, inject, input, viewChild } from "@angular/core";
import { BaseChartDirective } from "ng2-charts";
import { AppService } from "../app.service";
import { BenchmarkResults } from "../benchmarks";

@Component({
    selector: "app-size-benchmark",
    imports: [BaseChartDirective],
    templateUrl: "./size-benchmark.component.html",
    styleUrl: "./size-benchmark.component.scss",
})
export class SizeBenchmarkComponent {
    protected readonly service = inject(AppService);

    protected readonly canvas = viewChild.required<ElementRef<HTMLCanvasElement>>("canvas");

    readonly results = input.required<BenchmarkResults>();

    readonly options = computed(() => this.service.createChartOptions("x", "Size (KB)", 3, "KB"));

    readonly data = computed(() => {
        const results = this.results();
        const configurationKeys = Object.keys(results);

        const labels = configurationKeys.map((key) => results[key].name);
        const sizes = configurationKeys.map((key) => results[key].size / 1024); // Convert bytes to kilobytes
        const backgroundColors = configurationKeys.map((key) =>
            results[key].status === "pass" ? results[key].color : "#4a4a4a",
        );

        return {
            labels,
            datasets: [
                {
                    data: sizes,
                    backgroundColor: backgroundColors,
                    borderWidth: 0,
                },
            ],
        };
    });
}

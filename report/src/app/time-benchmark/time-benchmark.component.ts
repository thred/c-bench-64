import { Component, computed, ElementRef, inject, input, viewChild } from "@angular/core";
import { BaseChartDirective } from "ng2-charts";
import { AppService } from "../app.service";
import { BenchmarkResults } from "../benchmarks";

@Component({
    selector: "app-time-benchmark",
    imports: [BaseChartDirective],
    templateUrl: "./time-benchmark.component.html",
    styleUrl: "./time-benchmark.component.scss",
})
export class TimeBenchmarkComponent {
    protected readonly service = inject(AppService);

    protected readonly canvas = viewChild.required<ElementRef<HTMLCanvasElement>>("canvas");

    readonly results = input.required<BenchmarkResults>();

    readonly options = computed(() => this.service.createChartOptions("y", "Time (s)", 1, "s"));

    readonly data = computed(() => {
        const results = this.results();
        const configKeys = Object.keys(results);

        const labels = configKeys.map((key) => results[key].name);
        const times = configKeys.map((key) => results[key].time);
        const backgroundColors = configKeys.map((key) =>
            results[key].status === "pass" ? results[key].color : "#4a4a4a",
        );

        return {
            labels,
            datasets: [
                {
                    data: times,
                    backgroundColor: backgroundColors,
                    borderWidth: 0,
                },
            ],
        };
    });
}

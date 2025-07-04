import { Component, computed, inject, input } from "@angular/core";
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

    readonly results = input.required<BenchmarkResults>();

    readonly options = computed(() => this.service.createChartOptions("y", "Time (s)", 1, "s"));

    readonly data = computed(() => {
        const results = this.results();
        const configurationKeys = Object.keys(results);

        const labels = configurationKeys.map((key) => results[key].name);
        const times = configurationKeys.map((key) => results[key].time);
        const backgroundColors = configurationKeys.map((key) => results[key].color);

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

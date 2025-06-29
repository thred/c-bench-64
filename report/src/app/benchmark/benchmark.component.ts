import { ChangeDetectionStrategy, Component, computed, inject, input } from "@angular/core";
import { MatCardModule } from "@angular/material/card";
import { AppService } from "../app.service";
import { BenchmarkKey } from "../benchmarks";
import { SizeBenchmarkComponent } from "../size-benchmark/size-benchmark.component";
import { TimeBenchmarkComponent } from "../time-benchmark/time-benchmark.component";

@Component({
    selector: "app-benchmark",
    imports: [MatCardModule, TimeBenchmarkComponent, SizeBenchmarkComponent],
    templateUrl: "./benchmark.component.html",
    styleUrl: "./benchmark.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class BenchmarkComponent {
    private readonly service = inject(AppService);

    readonly key = input.required<BenchmarkKey>();

    readonly benchmark = computed(() => this.service.benchmarkWithResultsMap()[this.key()]);
}

import { ChangeDetectionStrategy, Component, inject } from "@angular/core";
import { AppService } from "../app.service";
import { BenchmarkSummaryComponent } from "../benchmark-summary/benchmark-summary.component";
import { BenchmarkComponent } from "../benchmark/benchmark.component";
import { benchmarkKeys, benchmarks, testKeys } from "../benchmarks";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { lazyComputed } from "../signals/lazy.signal";
import { sizeOfSignal } from "../signals/size-of.signal";
import { TestComponent } from "../test/test.component";

@Component({
    selector: "app-benchmarks-page",
    imports: [
        BenchmarkComponent,
        BenchmarkSummaryComponent,
        SectionComponent,
        SectionCollapserDirective,
        TestComponent,
    ],
    templateUrl: "./benchmarks-page.component.html",
    styleUrl: "./benchmarks-page.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class BenchmarksPageComponent {
    readonly service = inject(AppService);

    readonly benchmarkKeys = benchmarkKeys;
    readonly testKeys = testKeys;

    readonly benchmarks = benchmarks;

    private readonly sizeOf = sizeOfSignal();

    readonly chartWidth = lazyComputed(() => Math.min(this.sizeOf().width, 1024), 0.1);
}

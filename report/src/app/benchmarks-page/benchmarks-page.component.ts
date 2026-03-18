import { ChangeDetectionStrategy, Component, effect, inject } from "@angular/core";
import { RouterLink } from "@angular/router";
import { AppService } from "../app.service";
import { BenchmarkSummaryComponent } from "../benchmark-summary/benchmark-summary.component";
import { BenchmarkComponent } from "../benchmark/benchmark.component";
import { benchmarkKeys, benchmarks } from "../benchmarks";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { lazyComputed } from "../signals/lazy.signal";
import { routeParamSignal } from "../signals/route.signal";
import { sizeOfSignal } from "../signals/size-of.signal";

@Component({
    selector: "app-benchmarks-page",
    imports: [BenchmarkComponent, BenchmarkSummaryComponent, SectionComponent, SectionCollapserDirective, RouterLink],
    templateUrl: "./benchmarks-page.component.html",
    styleUrl: "./benchmarks-page.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class BenchmarksPageComponent {
    readonly service = inject(AppService);

    readonly benchmarkKeys = benchmarkKeys;
    readonly nonTestBenchmarkKeys = benchmarkKeys.filter((b) => !b.startsWith("test_"));

    readonly benchmarks = benchmarks;

    readonly benchmarkKey = routeParamSignal("benchmarkKey");

    private readonly sizeOf = sizeOfSignal();

    readonly chartWidth = lazyComputed(() => Math.min(this.sizeOf().width, 1024), 0.1);

    protected readonly scrollToCompilerEffect = effect(() => {
        const benchmarkKey = this.benchmarkKey();

        if (benchmarkKey) {
            window.setTimeout(() => {
                document.getElementById(benchmarkKey)?.scrollIntoView({ behavior: "smooth" });
            }, 100);
        }
    });
}

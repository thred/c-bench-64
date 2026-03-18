import { ChangeDetectionStrategy, Component, effect, inject } from "@angular/core";
import { RouterLink } from "@angular/router";
import { AppService } from "../app.service";
import { BenchmarkComponent } from "../benchmark/benchmark.component";
import { benchmarkKeys, benchmarks, testKeys } from "../benchmarks";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { lazyComputed } from "../signals/lazy.signal";
import { routeParamSignal } from "../signals/route.signal";
import { sizeOfSignal } from "../signals/size-of.signal";
import { TestComponent } from "../test/test.component";

@Component({
    selector: "app-tests-page",
    imports: [TestComponent, SectionComponent, BenchmarkComponent, SectionCollapserDirective, RouterLink],
    templateUrl: "./tests-page.component.html",
    styleUrl: "./tests-page.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class TestsPageComponent {
    readonly service = inject(AppService);

    readonly benchmarks = benchmarks;

    readonly testKeys = testKeys;

    readonly testBenchmarkKeys = benchmarkKeys.filter((b) => b.startsWith("test_"));

    readonly testKey = routeParamSignal("testKey");

    private readonly sizeOf = sizeOfSignal();

    readonly chartWidth = lazyComputed(() => Math.min(this.sizeOf().width, 1024), 0.1);

    protected readonly scrollToCompilerEffect = effect(() => {
        const testKey = this.testKey();

        if (testKey) {
            window.setTimeout(() => {
                document.getElementById(testKey)?.scrollIntoView({ behavior: "smooth" });
            }, 100);
        }
    });
}

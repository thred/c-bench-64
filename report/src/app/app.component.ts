import { CommonModule } from "@angular/common";
import { Component, computed, effect, inject } from "@angular/core";
import { AppService } from "./app.service";
import { BenchmarkSummaryComponent } from "./benchmark-summary/benchmark-summary.component";
import { BenchmarkComponent } from "./benchmark/benchmark.component";
import { benchmarkKeys, benchmarks, BenchmarkWithResultsMap, compilerKeys, testKeys } from "./benchmarks";
import { CompilerComponent } from "./compiler/compiler.component";
import { lazyComputed } from "./lazy.signal";
import { ScreenshotComponent } from "./screenshot/screenshot.component";
import { SectionCollapserDirective } from "./section-collapser.directive";
import { SectionComponent } from "./section/section.component";
import { sizeOfSignal } from "./signals/size-of.signal";
import { TestComponent } from "./test/test.component";
import { themeNames, themes } from "./themes";
import { TOCItemComponent } from "./toc-item/toc-item.component";
import { TOCSectionComponent } from "./toc-section/toc-section.component";
import { TOCSeparatorComponent } from "./toc-separator/toc-separator.component";
import { TOCComponent } from "./toc/toc.component";

@Component({
    selector: "app-root",
    imports: [
        CommonModule,
        BenchmarkComponent,
        BenchmarkSummaryComponent,
        CompilerComponent,
        ScreenshotComponent,
        SectionComponent,
        SectionCollapserDirective,
        TestComponent,
        TOCComponent,
        TOCSectionComponent,
        TOCItemComponent,
        TOCSeparatorComponent,
    ],
    templateUrl: "./app.component.html",
    styleUrl: "./app.component.scss",
})
export class App {
    readonly service = inject(AppService);

    get theme() {
        return this.service.theme;
    }

    readonly benchmarkKeys = benchmarkKeys;
    readonly nonTestBenchmarkKeys = benchmarkKeys.filter((b) => !b.startsWith("test_"));
    readonly testBenchmarkKeys = benchmarkKeys.filter((b) => b.startsWith("test_"));
    readonly compilerKeys = compilerKeys;
    readonly testKeys = testKeys;
    readonly themes = themes;
    readonly themeNames = themeNames;

    readonly benchmarkWithResultsMap = computed<BenchmarkWithResultsMap>(() => this.service.benchmarkWithResultsMap());
    readonly compilers = computed(() => this.service.compilersByKey());
    readonly benchmarks = benchmarks;

    private readonly sizeOf = sizeOfSignal();

    readonly chartWidth = lazyComputed(() => Math.min(this.sizeOf().width, 1024), 0.1);

    constructor() {
        effect(() => {
            document.documentElement.classList.remove(...themes);
            document.documentElement.classList.add(this.theme());
        });
    }
}

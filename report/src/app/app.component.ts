import { CommonModule } from "@angular/common";
import { Component, computed, effect, inject } from "@angular/core";
import { AppService } from "./app.service";
import { BenchmarkSummaryComponent } from "./benchmark-summary/benchmark-summary.component";
import { BenchmarkComponent } from "./benchmark/benchmark.component";
import { benchmarkKeys, benchmarks, BenchmarkWithResultsMap, compilerKeys } from "./benchmarks";
import { CompilerComponent } from "./compiler/compiler.component";
import { lazyComputed } from "./lazy.signal";
import { persistentSignal } from "./persistent.signal";
import { SectionCollapserDirective } from "./section-collapser.directive";
import { SectionComponent } from "./section/section.component";
import { sizeOfSignal } from "./signals/size-of.signal";
import { TOCItemComponent } from "./toc-item/toc-item.component";
import { TOCSectionComponent } from "./toc-section/toc-section.component";
import { TOCComponent } from "./toc/toc.component";

@Component({
    selector: "app-root",
    imports: [
        CommonModule,
        BenchmarkComponent,
        BenchmarkSummaryComponent,
        CompilerComponent,
        SectionComponent,
        SectionCollapserDirective,
        TOCComponent,
        TOCSectionComponent,
        TOCItemComponent,
    ],
    templateUrl: "./app.component.html",
    styleUrl: "./app.component.scss",
})
export class App {
    readonly service = inject(AppService);

    readonly benchmarkKeys = benchmarkKeys;
    readonly compilerKeys = compilerKeys;

    readonly benchmarkWithResultsMap = computed<BenchmarkWithResultsMap>(() => this.service.benchmarkWithResultsMap());
    readonly compilers = computed(() => this.service.compilers());
    readonly benchmarks = benchmarks;

    readonly theme = persistentSignal<string>("c-bench-64.theme", () => "c64");

    private readonly sizeOf = sizeOfSignal();

    readonly chartWidth = lazyComputed(() => Math.min(this.sizeOf().width, 1024), 0.1);

    constructor() {
        effect(() => {
            document.documentElement.classList.remove("dark", "light");

            switch (this.theme()) {
                case "dark":
                    document.documentElement.classList.add("dark");
                    break;
                case "light":
                    document.documentElement.classList.add("light");
                    break;
            }
        });
    }
}

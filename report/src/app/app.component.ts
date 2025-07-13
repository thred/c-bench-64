import { CommonModule } from "@angular/common";
import { Component, computed, inject } from "@angular/core";
import { AppService } from "./app.service";
import { BenchmarkSummaryComponent } from "./benchmark-summary/benchmark-summary.component";
import { BenchmarkComponent } from "./benchmark/benchmark.component";
import { benchmarkKeys, benchmarks, BenchmarkWithResultsMap, compilerKeys } from "./benchmarks";
import { CompilerComponent } from "./compiler/compiler.component";

@Component({
    selector: "app-root",
    imports: [CommonModule, BenchmarkComponent, BenchmarkSummaryComponent, CompilerComponent],
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
}

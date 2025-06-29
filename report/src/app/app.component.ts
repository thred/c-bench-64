import { CommonModule } from "@angular/common";
import { Component, computed, inject } from "@angular/core";
import { AppService } from "./app.service";
import { BenchmarkComponent } from "./benchmark/benchmark.component";
import { benchmarkKeys, BenchmarkWithResultsMap, compilerKeys } from "./benchmarks";
import { CompilerComponent } from "./compiler/compiler.component";

@Component({
    selector: "app-root",
    imports: [CommonModule, BenchmarkComponent, CompilerComponent],
    templateUrl: "./app.component.html",
    styleUrl: "./app.component.scss",
})
export class App {
    private readonly service = inject(AppService);

    readonly benchmarkKeys = benchmarkKeys;
    readonly compilerKeys = compilerKeys;

    readonly benchmarkWithResultsMap = computed<BenchmarkWithResultsMap>(() => this.service.benchmarkWithResultsMap());
    readonly compilers = computed(() => this.service.compilers());
}

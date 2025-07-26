import { CommonModule } from "@angular/common";
import { Component, computed, inject } from "@angular/core";
import { FormsModule } from "@angular/forms";
import { AppService } from "../app.service";
import { BenchmarkSummaryTableComponent } from "../benchmark-summary-table/benchmark-summary-table.component";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";

@Component({
    selector: "app-benchmark-summary",
    imports: [CommonModule, BenchmarkSummaryTableComponent, FormsModule, SectionComponent, SectionCollapserDirective],
    templateUrl: "./benchmark-summary.component.html",
    styleUrl: "./benchmark-summary.component.scss",
})
export class BenchmarkSummaryComponent {
    private readonly service = inject(AppService);

    readonly benchmarkKeys = computed(() => this.service.selectedBenchmarkKeys());

    readonly configKeys = computed(() => this.service.selectedConfigKeys());
}

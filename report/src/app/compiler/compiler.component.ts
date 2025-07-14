import { DatePipe } from "@angular/common";
import { ChangeDetectionStrategy, Component, input } from "@angular/core";
import { Compiler } from "../benchmarks";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { SupportedComponent } from "../supported/supported.component";

@Component({
    selector: "app-compiler",
    templateUrl: "./compiler.component.html",
    styleUrl: "./compiler.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
    imports: [SupportedComponent, DatePipe, SectionComponent, SectionCollapserDirective],
})
export class CompilerComponent {
    readonly compiler = input.required<Compiler>();
}

import { DatePipe } from "@angular/common";
import { ChangeDetectionStrategy, Component, inject, input } from "@angular/core";
import { FormsModule } from "@angular/forms";
import { AppService } from "../app.service";
import { Compiler } from "../benchmarks";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { SupportedComponent } from "../supported/supported.component";

@Component({
    selector: "app-compiler",
    templateUrl: "./compiler.component.html",
    styleUrl: "./compiler.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
    imports: [SupportedComponent, DatePipe, SectionComponent, SectionCollapserDirective, FormsModule],
})
export class CompilerComponent {
    readonly service = inject(AppService);

    readonly compiler = input.required<Compiler>();
}

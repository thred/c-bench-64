import { ChangeDetectionStrategy, Component, input } from "@angular/core";
import { Compiler } from "../benchmarks";
import { SupportedComponent } from "../supported/supported.component";

@Component({
    selector: "app-compiler",
    templateUrl: "./compiler.component.html",
    styleUrl: "./compiler.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
    imports: [SupportedComponent],
})
export class CompilerComponent {
    readonly compiler = input.required<Compiler>();
}

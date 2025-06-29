import { ChangeDetectionStrategy, Component, input } from "@angular/core";
import { Compiler } from "../benchmarks";

@Component({
    selector: "app-compiler",
    templateUrl: "./compiler.component.html",
    styleUrl: "./compiler.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class CompilerComponent {
    readonly compiler = input.required<Compiler>();
}

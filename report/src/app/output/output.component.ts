import { Component, input } from "@angular/core";

@Component({
    selector: "app-output",
    imports: [],
    templateUrl: "./output.component.html",
    styleUrl: "./output.component.scss",
})
export class OutputComponent {
    readonly output = input.required<string>();
}

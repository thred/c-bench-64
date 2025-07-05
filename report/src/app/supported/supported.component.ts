import { Component, input } from "@angular/core";
import { Supported } from "../benchmarks";

@Component({
    selector: "app-supported",
    imports: [],
    templateUrl: "./supported.component.html",
    styleUrl: "./supported.component.scss",
})
export class SupportedComponent {
    readonly supported = input.required<Supported>();
}

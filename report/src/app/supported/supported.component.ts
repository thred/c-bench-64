import { Component, computed, input } from "@angular/core";
import { Supported } from "../benchmarks";

@Component({
    selector: "app-supported",
    imports: [],
    templateUrl: "./supported.component.html",
    styleUrl: "./supported.component.scss",
})
export class SupportedComponent {
    readonly supported = input.required<Supported>();

    readonly url = input<string | undefined>(undefined);

    readonly className = computed(() => {
        switch (this.supported()) {
            case "yes":
                return "yes";
            case "no":
                return "no";
            case "partial":
                return "partial";
            default:
                return "";
        }
    });

    readonly icon = computed(() => {
        switch (this.supported()) {
            case "yes":
                return "check";
            case "no":
                return "times";
            case "partial":
                return undefined;
            default:
                return undefined;
        }
    });

    readonly text = computed(() => {
        switch (this.supported()) {
            case "yes":
                return "Yes";
            case "no":
                return "No";
            case "partial":
                return "Partially";
            default:
                return this.supported();
        }
    });
}

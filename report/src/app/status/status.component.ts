import { Component, computed, input } from "@angular/core";
import { Status } from "../benchmarks";

@Component({
    selector: "app-status",
    imports: [],
    templateUrl: "./status.component.html",
    styleUrl: "./status.component.scss",
    host: {
        "[class]": "className()",
    },
})
export class StatusComponent {
    readonly status = input.required<Status>();

    readonly className = computed(() => this.status());

    readonly icon = computed(() => {
        switch (this.status()) {
            case "pass":
                return "check";
            case "fail":
                return "times";
            case "unsupported":
                return "minus";
            case "unknown":
                return "question";
            default:
                return undefined;
        }
    });
}

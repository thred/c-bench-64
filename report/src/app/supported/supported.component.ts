import { Component, computed, input } from "@angular/core";
import { Supported } from "../benchmarks";

@Component({
    selector: "app-supported",
    imports: [],
    templateUrl: "./supported.component.html",
    styleUrl: "./supported.component.scss",
})
export class SupportedComponent {
    readonly supported = input.required<Supported, Supported | null>({
        transform: (value) => value ?? "unknown",
    });

    readonly url = input<string | undefined>(undefined);

    readonly className = computed(() => {
        let supported = this.supported();

        if (typeof supported === "object") {
            supported = supported.supported;
        }

        switch (supported) {
            case "yes":
                return "yes";
            case "no":
                return "no";
            case "partial":
                return "partial";
            case "unknown":
                return "unknown";
            default:
                return "";
        }
    });

    readonly icon = computed(() => {
        let supported = this.supported();

        if (typeof supported === "object") {
            supported = supported.supported;
        }

        switch (supported) {
            case "yes":
                return "check";
            case "no":
                return "times";
            case "partial":
                return undefined;
            case "unknown":
                return "question";
            default:
                return undefined;
        }
    });

    readonly text = computed(() => {
        let supported = this.supported();

        if (typeof supported === "object") {
            supported = supported.supported;
        }

        switch (supported) {
            case "yes":
                return "Yes";
            case "no":
                return "No";
            case "partial":
                return "Partially";
            case "unknown":
                return "Unknown";
            default:
                return this.supported();
        }
    });

    readonly note = computed(() => {
        let supported = this.supported();

        if (typeof supported === "object") {
            return supported.note;
        }

        return undefined;
    });
}

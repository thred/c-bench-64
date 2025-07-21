import { ChangeDetectionStrategy, Component, computed, HostListener, inject } from "@angular/core";
import { ScreenshotService } from "./screenshot.service";

@Component({
    selector: "app-screenshot",
    imports: [],
    templateUrl: "./screenshot.component.html",
    styleUrl: "./screenshot.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class ScreenshotComponent {
    private readonly screenshotService = inject(ScreenshotService);

    readonly screenshotUrl = computed(() => this.screenshotService.screenshotUrl());

    close(): void {
        this.screenshotService.closeScreenshot();
    }

    @HostListener("document:keydown", ["$event"])
    handleKeyDown(event: KeyboardEvent): void {
        if (event.key === "Escape" && this.screenshotUrl()) {
            this.close();
        }
    }
}

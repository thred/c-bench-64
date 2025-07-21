import { Injectable, signal } from "@angular/core";

@Injectable({
    providedIn: "root",
})
export class ScreenshotService {
    readonly screenshotUrl = signal<string | undefined>(undefined);

    openScreenshot(screenshotUrl: string | undefined): void {
        this.screenshotUrl.set(screenshotUrl);
    }

    closeScreenshot(): void {
        this.screenshotUrl.set(undefined);
    }
}

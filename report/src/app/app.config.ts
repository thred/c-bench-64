import { provideHttpClient } from "@angular/common/http";
import { ApplicationConfig, provideBrowserGlobalErrorListeners, provideZonelessChangeDetection } from "@angular/core";
import { provideCharts, withDefaultRegisterables } from "ng2-charts";

export const appConfig: ApplicationConfig = {
    providers: [
        provideBrowserGlobalErrorListeners(),
        provideZonelessChangeDetection(),
        provideCharts(withDefaultRegisterables()),
        provideHttpClient(),
    ],
};

declare global {
    interface Array<T> {
        distinct(): Array<T>;
    }
}

Array.prototype.distinct = function <T>(this: T[]): T[] {
    return Array.from(new Set(this));
};

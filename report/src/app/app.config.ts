import { provideHttpClient } from "@angular/common/http";
import { ApplicationConfig, provideBrowserGlobalErrorListeners, provideZonelessChangeDetection } from "@angular/core";
import { provideRouter, withViewTransitions } from "@angular/router";
import { provideCharts, withDefaultRegisterables } from "ng2-charts";
import { routes } from "./app.routes";

export const appConfig: ApplicationConfig = {
    providers: [
        provideBrowserGlobalErrorListeners(),
        provideZonelessChangeDetection(),
        provideRouter(routes, withViewTransitions()),
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

import { ChangeDetectionStrategy, Component, computed, effect, inject } from "@angular/core";
import { RouterOutlet } from "@angular/router";
import { AppService } from "./app.service";
import { benchmarkKeys, benchmarks, compilerKeys, testKeys } from "./benchmarks";
import { ScreenshotComponent } from "./screenshot/screenshot.component";
import { themeNames, themes } from "./themes";
import { TOCItemComponent } from "./toc-item/toc-item.component";
import { TOCSectionComponent } from "./toc-section/toc-section.component";
import { TOCSeparatorComponent } from "./toc-separator/toc-separator.component";
import { TOCComponent } from "./toc/toc.component";
import { Utils } from "./utils";

@Component({
    selector: "app-root",
    imports: [
        ScreenshotComponent,
        RouterOutlet,
        TOCComponent,
        TOCSectionComponent,
        TOCItemComponent,
        TOCSeparatorComponent,
    ],
    templateUrl: "./app.component.html",
    styleUrl: "./app.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class App {
    readonly service = inject(AppService);

    get theme() {
        return this.service.theme;
    }

    readonly nonTestBenchmarkKeys = benchmarkKeys.filter((b) => !b.startsWith("test_"));
    readonly testBenchmarkKeys = benchmarkKeys.filter((b) => b.startsWith("test_"));
    readonly compilerKeys = compilerKeys;
    readonly testKeys = testKeys;
    readonly themes = themes;
    readonly themeNames = themeNames;

    readonly compilers = computed(() => this.service.compilersByKey());
    readonly benchmarks = benchmarks;

    readonly includePerfOpts = computed(() => this.service.includePerfOpts());
    readonly includeSizeOpts = computed(() => this.service.includeSizeOpts());

    constructor() {
        effect(() => {
            document.documentElement.classList.remove(...themes);
            document.documentElement.classList.add(this.theme());
        });
    }

    toggleIncludePerfOpts(): void {
        if (this.service.includePerfOpts()) {
            this.service.includeSizeOpts.set(false);
        }

        this.service.includePerfOpts.set(true);
    }

    toggleIncludeSizeOpts(): void {
        if (this.service.includeSizeOpts()) {
            this.service.includePerfOpts.set(false);
        }

        this.service.includeSizeOpts.set(true);
    }

    setIncludePerfOpts(checked: boolean): void {
        this.service.includePerfOpts.set(checked);
        this.reactivateOpts();
    }

    setIncludeSizeOpts(checked: boolean): void {
        this.service.includeSizeOpts.set(checked);
        this.reactivateOpts();
    }

    private async reactivateOpts() {
        if (!this.service.includePerfOpts() && !this.service.includeSizeOpts()) {
            await Utils.wait(0.1); // Allow the UI to update before reactivating

            this.service.includePerfOpts.set(true);
            this.service.includeSizeOpts.set(true);
        }
    }
}

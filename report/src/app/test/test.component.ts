import { Component, computed, inject, input } from "@angular/core";
import { AppService } from "../app.service";
import { Config, TestKey } from "../benchmarks";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { TestResultRowComponent } from "../test-result-row/test-result-row.component";

@Component({
    selector: "app-test",
    imports: [TestResultRowComponent, SectionComponent, SectionCollapserDirective],
    templateUrl: "./test.component.html",
    styleUrl: "./test.component.scss",
})
export class TestComponent {
    private readonly service = inject(AppService);

    readonly testKey = input.required<TestKey>();

    readonly configKeys = input.required<string[]>();

    readonly configs = computed<Config[]>(
        () =>
            this.configKeys()
                .map((configKey) => this.service.findConfigByKey(configKey))
                .filter(Boolean) as Config[],
    );

    readonly existingConfigKeys = computed(() => this.configs().map((config) => config.key));

    readonly methods = computed(() => {
        const methods: string[] = [];

        for (const configKey of this.configKeys()) {
            const map = this.service.findMethodTestResultsMapByTestAndConfigKey(this.testKey(), configKey);

            if (!map) {
                continue;
            }

            methods.push(...Object.keys(map));
        }

        return Array.from(new Set(methods)).sort((a, b) => a.localeCompare(b));
    });
}

import { ChangeDetectionStrategy, Component, effect, inject } from "@angular/core";
import { AppService } from "../app.service";
import { SectionCollapserDirective } from "../section-collapser.directive";
import { SectionComponent } from "../section/section.component";
import { routeParamSignal } from "../signals/route.signal";

@Component({
    selector: "app-notice-page",
    imports: [SectionComponent, SectionCollapserDirective],
    templateUrl: "./notice-page.component.html",
    styleUrl: "./notice-page.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class NoticePage {
    readonly service = inject(AppService);

    readonly noticeKey = routeParamSignal("noticeKey");

    protected readonly scrollToCompilerEffect = effect(() => {
        const noticeKey = this.noticeKey();

        if (noticeKey) {
            window.setTimeout(() => {
                document.getElementById(noticeKey)?.scrollIntoView({ behavior: "smooth" });
            }, 100);
        }
    });
}

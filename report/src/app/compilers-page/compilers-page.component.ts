import { ChangeDetectionStrategy, Component, computed, effect, inject } from "@angular/core";
import { RouterLink } from "@angular/router";
import { AppService } from "../app.service";
import { compilerKeys } from "../benchmarks";
import { CompilerComponent } from "../compiler/compiler.component";
import { routeParamSignal } from "../signals/route.signal";

@Component({
    selector: "app-compilers-page",
    imports: [CompilerComponent, RouterLink],
    templateUrl: "./compilers-page.component.html",
    styleUrl: "./compilers-page.component.scss",
    changeDetection: ChangeDetectionStrategy.OnPush,
})
export class CompilersPageComponent {
    readonly service = inject(AppService);

    readonly compilerKeys = compilerKeys;

    readonly compilerKey = routeParamSignal("compilerKey");

    readonly compilers = computed(() => this.service.compilersByKey());

    protected readonly scrollToCompilerEffect = effect(() => {
        const compilerKey = this.compilerKey();
        const compilers = this.compilers();

        if (compilerKey && compilers[compilerKey as keyof typeof compilers]) {
            window.setTimeout(() => {
                document.getElementById("compiler-" + compilerKey)?.scrollIntoView({ behavior: "smooth" });
            }, 100);
        }
    });
}

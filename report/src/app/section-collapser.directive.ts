import { DestroyRef, Directive, effect, ElementRef, HostListener, inject } from "@angular/core";
import { SectionComponent } from "./section/section.component";

@Directive({
    selector: "[appSectionCollapser]",
})
export class SectionCollapserDirective {
    private readonly section = inject(SectionComponent);
    private readonly elementRef = inject(ElementRef);

    constructor() {
        this.section.collapsible.set(true);
        this.elementRef.nativeElement.classList.add("section-collapser");

        inject(DestroyRef).onDestroy(() => {
            this.section.collapsible.set(false);
            this.elementRef.nativeElement.classList.remove("section-collapser");
        });

        effect(() => {
            if (this.section.collapsed()) {
                this.elementRef.nativeElement.classList.add("collapsed");
            } else {
                this.elementRef.nativeElement.classList.remove("collapsed");
            }

            this.elementRef.nativeElement.setAttribute("aria-expanded", !this.section.collapsed());
        });
    }

    @HostListener("click")
    toggleCollapse() {
        if (this.section.collapsible()) {
            this.section.collapsed.set(!this.section.collapsed());
        }
    }
}

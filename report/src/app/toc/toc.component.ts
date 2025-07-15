import { Component, contentChildren, signal } from "@angular/core";
import { lazyComputed } from "../lazy.signal";
import { sizeOfSignal } from "../signals/size-of.signal";
import { TOCSectionComponent } from "../toc-section/toc-section.component";

@Component({
    selector: "app-toc",
    imports: [],
    templateUrl: "./toc.component.html",
    styleUrl: "./toc.component.scss",
    host: {
        class: "toc",
    },
})
export class TOCComponent {
    readonly sections = contentChildren(TOCSectionComponent);

    readonly expanded = signal<string | undefined>(undefined);

    readonly sizeOf = sizeOfSignal();

    readonly height = lazyComputed(() => `${this.sizeOf().height}px`);
}

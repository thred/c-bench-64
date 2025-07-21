import { NgTemplateOutlet } from "@angular/common";
import { Component, computed, contentChildren, inject, input, model } from "@angular/core";
import { AbstractTocItemComponent } from "../toc-item/abstract-toc-item.component";
import { TOCSeparatorComponent } from "../toc-separator/toc-separator.component";
import { TOCComponent } from "../toc/toc.component";

@Component({
    selector: "app-toc-section",
    imports: [NgTemplateOutlet],
    templateUrl: "./toc-section.component.html",
    styleUrl: "./toc-section.component.scss",
    host: {
        class: "toc-section",
    },
})
export class TOCSectionComponent {
    readonly toc = inject(TOCComponent);

    readonly id = crypto.randomUUID();

    readonly items = contentChildren(AbstractTocItemComponent);

    readonly separators = contentChildren(TOCSeparatorComponent);

    readonly href = input<string | undefined>(undefined);

    readonly label = input.required<string>();

    readonly collapsed = model<boolean>(false);

    readonly expanded = computed(() => this.toc.expanded() === this.id);

    expand(event: Event) {
        event.preventDefault();

        if (this.expanded()) {
            this.toc.expanded.set(undefined);
        } else {
            this.toc.expanded.set(this.id);
        }
    }
}

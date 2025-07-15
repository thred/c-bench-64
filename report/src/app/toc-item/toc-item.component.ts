import { CommonModule } from "@angular/common";
import { Component, inject, input, model, output, TemplateRef, viewChild } from "@angular/core";
import { FormsModule } from "@angular/forms";
import { TOCComponent } from "../toc/toc.component";

@Component({
    selector: "app-toc-item",
    imports: [CommonModule, FormsModule],
    templateUrl: "./toc-item.component.html",
    styleUrl: "./toc-item.component.scss",
})
export class TOCItemComponent {
    readonly toc = inject(TOCComponent);

    readonly template = viewChild.required(TemplateRef);

    readonly href = input<string | undefined>(undefined);

    readonly label = input.required<string>();

    readonly click = output<Event>();

    readonly checkable = input<boolean>(false);

    readonly checked = model<boolean>(true);

    performSelect(event: Event) {
        event.preventDefault();

        this.click.emit(event);

        if (this.href()) {
            const element = document.querySelector(this.href()!);

            if (element) {
                element.scrollIntoView({ behavior: "smooth" });
            }
        }

        this.toc.expanded.set(undefined);
    }
}

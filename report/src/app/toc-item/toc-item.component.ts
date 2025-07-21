import { CommonModule } from "@angular/common";
import { Component, inject, input, model, output } from "@angular/core";
import { FormsModule } from "@angular/forms";
import { TOCComponent } from "../toc/toc.component";
import { Utils } from "../utils";
import { AbstractTocItemComponent } from "./abstract-toc-item.component";

@Component({
    selector: "app-toc-item",
    imports: [CommonModule, FormsModule],
    templateUrl: "./toc-item.component.html",
    styleUrl: "./toc-item.component.scss",
    providers: [{ provide: AbstractTocItemComponent, useExisting: TOCItemComponent }],
})
export class TOCItemComponent extends AbstractTocItemComponent {
    readonly toc = inject(TOCComponent);

    readonly href = input<string | undefined>(undefined);

    readonly label = input.required<string>();

    readonly click = output<Event>();

    override get clickable(): boolean {
        return true;
    }

    readonly checkable = input<boolean>(false);

    readonly checked = model<boolean>(true);

    async performSelect(event: Event) {
        event.preventDefault();

        if (this.checkable() && !this.checked()) {
            this.checked.set(true);

            // Allow the UI to change state
            await Utils.wait(0.1);
        }

        this.click.emit(event);

        let href = this.href();

        if (href) {
            href = href.replace(/\./g, "\\.");

            const element = document.querySelector(href!);

            if (element) {
                element.scrollIntoView({ behavior: "smooth" });
            }
        }

        this.toc.expanded.set(undefined);
    }
}

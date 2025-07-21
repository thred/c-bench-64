import { Component, inject, input } from "@angular/core";
import { AbstractTocItemComponent } from "../toc-item/abstract-toc-item.component";
import { TOCComponent } from "../toc/toc.component";

@Component({
    selector: "app-toc-separator",
    templateUrl: "./toc-separator.component.html",
    styleUrl: "./toc-separator.component.scss",
    providers: [{ provide: AbstractTocItemComponent, useExisting: TOCSeparatorComponent }],
})
export class TOCSeparatorComponent extends AbstractTocItemComponent {
    readonly toc = inject(TOCComponent);

    readonly label = input<string | undefined>(undefined);
}

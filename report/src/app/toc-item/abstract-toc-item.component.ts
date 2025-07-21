import { Directive, TemplateRef, viewChild } from "@angular/core";

@Directive()
export abstract class AbstractTocItemComponent {
    readonly template = viewChild.required(TemplateRef);

    get clickable(): boolean {
        return false;
    }
}

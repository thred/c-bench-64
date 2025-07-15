import { Component, model, signal } from "@angular/core";

@Component({
    selector: "app-section",
    imports: [],
    templateUrl: "./section.component.html",
    styleUrl: "./section.component.scss",
    host: {
        class: "section",
        "[class.do-not-print]": "collapsed()",
    },
})
export class SectionComponent {
    readonly collapsed = model<boolean>(false);

    readonly collapsible = signal<boolean>(true);
}

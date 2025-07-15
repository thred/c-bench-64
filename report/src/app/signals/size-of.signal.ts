import { assertInInjectionContext, DestroyRef, effect, ElementRef, inject, Signal, signal } from "@angular/core";

/**
 * Creates a signal that emits the size of the observed element. Must be called within an injection context.
 *
 * @param elementRef The optional element reference (native or as signal) to be observed. Uses HTML elements as default.
 * @returns A signal that emits the size of the observed element.
 */
export function sizeOfSignal(
    elementRef?: Signal<ElementRef<HTMLElement> | null | undefined> | ElementRef<HTMLElement>,
): Signal<{
    width: number;
    height: number;
}> {
    assertInInjectionContext(sizeOfSignal);

    let elementRefSignal: Signal<ElementRef<HTMLElement> | null | undefined>;

    if (elementRef === undefined) {
        elementRefSignal = signal(inject(ElementRef<HTMLElement>));
    } else if (elementRef instanceof ElementRef) {
        elementRefSignal = signal(elementRef);
    } else {
        elementRefSignal = elementRef;
    }

    const nativeElement = elementRefSignal()?.nativeElement;

    const sgnl = signal<{
        width: number;
        height: number;
    }>(nativeElement ? nativeElement.getBoundingClientRect() : { width: 0, height: 0 });

    const resizeObserver = new ResizeObserver((entries) => sgnl.set(entries[0].contentRect));

    inject(DestroyRef).onDestroy(() => resizeObserver.disconnect());

    let observedElementRef: ElementRef<HTMLElement> | null | undefined = null;

    effect(() => {
        if (elementRefSignal() === observedElementRef) {
            return;
        }

        observedElementRef = elementRefSignal();

        resizeObserver.disconnect();

        if (observedElementRef !== null && observedElementRef !== undefined) {
            resizeObserver.observe(observedElementRef.nativeElement);
            sgnl.set(observedElementRef.nativeElement.getBoundingClientRect());
        } else {
            sgnl.set({ width: 0, height: 0 });
        }
    });

    return sgnl;
}

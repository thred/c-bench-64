import { effect, Signal, signal, WritableSignal } from "@angular/core";

export interface LazySignal<T> extends WritableSignal<T> {
    set(value: T, inSeconds?: number): void;
    update(updateFn: (value: T) => T, inSeconds?: number): void;
}

/**
 * A writable signal, where the set operation can be postponed. Any new call to set or update will cancel the pending
 * operation.
 *
 * @param initialValue the initial value
 * @returns a lazy signal
 */
export function lazySignal<T>(initialValue: T): LazySignal<T> {
    const sgnl = signal<T>(initialValue);
    const setter = sgnl.set;

    let timeoutId: number | null = null;

    function setValue(value: T, inSeconds?: number): void {
        if (timeoutId) {
            window.clearTimeout(timeoutId);
            timeoutId = null;
        }

        if (inSeconds) {
            timeoutId = setTimeout(() => {
                timeoutId = null;
                setter(value);
            }, inSeconds * 1000);
        } else {
            setter(value);
        }
    }

    return Object.assign(sgnl, {
        set(value: T, inSeconds?: number): void {
            setValue(value, inSeconds);
        },

        update(updateFn: (value: T) => T, inSeconds?: number): void {
            setValue(updateFn(sgnl()), inSeconds);
        },
    });
}

/**
 * A computed signal, that only updates after the source hasn't changed for a given time.
 *
 * @param fn the function to compute the value
 * @param afterSeconds  the time to wait before updating the value
 * @returns a signal that emits the computed value
 */
export function lazyComputed<T>(fn: () => T, afterSeconds?: number): Signal<T> {
    const sgnl = lazySignal(fn());

    effect(() => sgnl.set(fn(), afterSeconds));

    return sgnl;
}

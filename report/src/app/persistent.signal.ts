import { effect, signal, WritableSignal } from "@angular/core";

export interface PersistentSignal<T> extends WritableSignal<T> {
    reset(): void;
    store(): void;
}

/**
 * A signal, which is stored in the local storage.
 *
 * @param key the key in the local storage
 * @param create a function to create the initial state if none is available
 * @returns the persistent signal
 *
 * @author Manfred Hantschel
 */
export function persistentSignal<T>(
    key: string,
    create: () => T,
    opts?: {
        autoStore?: boolean;
        read?: (json: string) => T;
        write?: (obj: T) => string;
    },
): PersistentSignal<T> {
    const read = opts?.read ?? ((json) => JSON.parse(json));
    const write = opts?.write ?? ((obj) => JSON.stringify(obj));
    const autoStore = opts?.autoStore ?? true;

    function restore(): T {
        const json = localStorage.getItem(key);

        if (json !== null && json !== undefined && json !== "null" && json !== "undefined") {
            try {
                return read(json);
            } catch (error) {
                console.error(`Failed to read ${key} from the local storage.`, error);
            }
        }

        return create();
    }

    function store(state: T): void {
        if (state === null || state === undefined) {
            localStorage.removeItem(key);
        } else {
            localStorage.setItem(key, write(state));
        }
    }

    const state = signal<T>(restore());

    if (autoStore) {
        effect(() => store(state()));
    }

    return Object.assign(state, {
        reset(): void {
            state.set(create());
        },
        store(): void {
            store(state());
        },
    });
}

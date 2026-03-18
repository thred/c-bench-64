import { computed, inject, Signal } from "@angular/core";
import { toSignal } from "@angular/core/rxjs-interop";
import { ActivatedRoute, IsActiveMatchOptions, NavigationEnd, ParamMap, Router, UrlTree } from "@angular/router";
import { filter, map } from "rxjs";

export function routeParamsMapSignal(): Signal<ParamMap | undefined> {
    const route = inject(ActivatedRoute);

    return toSignal(route.paramMap);
}

export function routeParamSignal(name: string): Signal<string | null | undefined> {
    const routeParamsMap = routeParamsMapSignal();

    return computed(() => routeParamsMap()?.get(name));
}

export function queryParamsMapSignal(): Signal<ParamMap | undefined> {
    const route = inject(ActivatedRoute);

    return toSignal(route.queryParamMap);
}

export function queryParamSignal<T = string | null | undefined>(
    key: string,
    transformer: (value: string | null | undefined) => T = (v) => v as T,
): Signal<T> {
    const queryParamsMap = queryParamsMapSignal();

    return computed(() => transformer(queryParamsMap()?.get(key)));
}

export function routeActiveSignal(
    url: string | UrlTree,
    matchOptions: IsActiveMatchOptions = {
        matrixParams: "ignored",
        queryParams: "ignored",
        paths: "subset",
        fragment: "ignored",
    },
): Signal<boolean> {
    const router = inject(Router);

    return toSignal(
        router.events.pipe(
            filter((event) => event instanceof NavigationEnd),
            map(() => router.isActive(url, matchOptions)),
        ),
        { initialValue: false },
    );
}

type RouteData = {
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    [key: string | symbol]: any;
};

export function routeDataSignal(): Signal<RouteData> {
    const route = inject(ActivatedRoute);
    const router = inject(Router);

    return toSignal(
        router.events.pipe(
            filter((event) => event instanceof NavigationEnd),
            map(() => Object.assign({}, route.snapshot.data, ...route.snapshot.children.map((child) => child.data))),
        ),
        { initialValue: {} },
    );
}

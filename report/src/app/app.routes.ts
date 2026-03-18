import { Routes } from "@angular/router";
import { BenchmarksPageComponent } from "./benchmarks-page/benchmarks-page.component";

export const routes: Routes = [
    { path: "benchmarks/:benchmarkKey", component: BenchmarksPageComponent },

    {
        path: "tests/:testKey",
        loadComponent: () => import("./tests-page/tests-page.component").then((m) => m.TestsPageComponent),
    },

    {
        path: "compilers/:compilerKey",
        loadComponent: () => import("./compilers-page/compilers-page.component").then((m) => m.CompilersPageComponent),
    },

    {
        path: "notice/:noticeKey",
        loadComponent: () => import("./notice-page/notice-page.component").then((m) => m.NoticePage),
    },

    { path: "**", redirectTo: "benchmarks/" },
];

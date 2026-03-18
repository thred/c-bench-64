export class Utils {
    static async wait(s: number): Promise<void> {
        return new Promise((resolve) => setTimeout(resolve, s * 1000));
    }

    static distinct<T>(array: T[]): T[] {
        return Array.from(new Set(array));
    }
}

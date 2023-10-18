export class RenderManager {
    private _isRunning: boolean = true;
    private readonly _renderQueue: Function[] = [];

    constructor() {
        this.run();
    }

    private run(): void {
        requestAnimationFrame(() => {
            this.flush();
            if (this._isRunning) {
                this.run();
            }
        });
    }

    private flush(): void {
        while (this._renderQueue.length !== 0) {
            const callback = this._renderQueue.shift();
            if (typeof callback !== 'function') {
                console.warn('render callback is not a function, please check your parameter of draw()');
                continue;
            }
            callback();
        }
    }

    stop(): void {
        if (this._isRunning) {
            this._isRunning = false;
        }
    }

    addTask(callback: Function): void {
        this._renderQueue.push(callback);
    }
};

export const renderManager = new RenderManager();

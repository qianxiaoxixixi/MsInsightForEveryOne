import { exec } from 'child_process';
import { promisify } from 'util';
import * as fs from 'fs';
import * as path from 'path';

const execAsync = promisify(exec);

type FolderInfo = {
    cardName: string;
    cardPath: string;
};

const folderSet = new Set<string>();
const script = 'Add-Type -AssemblyName System.Windows.Forms; $dialog = New-Object System.Windows.Forms.FolderBrowserDialog; $result = $dialog.ShowDialog(); if ($result -eq “OK”) { $dialog.SelectedPath }';
async function selectFolders(): Promise<FolderInfo[]> {
    const result: FolderInfo[] = [];
    try {
        const { stdout } = await execAsync(`PowerShell -Command "${script}`);
        const folderPath = stdout.trim();
        if (!folderPath) {
            return result;
        }
        const deviceFolder = folderPath.match(/\b\d+\.\d+\.\d+\.\d+\b/)?.[0];
        if (deviceFolder === undefined) {
            return result;
        }
        if (path.basename(folderPath) === deviceFolder) {
            const subfolders = fs.readdirSync(folderPath, { withFileTypes: true })
                .filter(dirent => dirent.isDirectory())
                .map(dirent => dirent.name);
            for (const subfolder of subfolders) {
                const subfolderPath = path.join(folderPath, subfolder);
                if (folderSet.has(subfolderPath)) {
                    continue;
                }
                folderSet.add(subfolderPath);
                result.push({
                    cardName: subfolder,
                    cardPath: subfolderPath,
                });
            }
        } else if (path.basename(path.dirname(folderPath)) === deviceFolder) {
            if (folderSet.has(folderPath)) {
                return result;
            }
            folderSet.add(folderPath);
            const cardName = path.basename(folderPath);
            result.push({
                cardName,
                cardPath: folderPath,
            });
        }
    } catch (error) {
        console.error(error);
    }
    return result;
}

export const importHandler = async (): Promise<Record<string, unknown>> => {
    const result = await selectFolders();
    return {
        result: [result],
    };
};

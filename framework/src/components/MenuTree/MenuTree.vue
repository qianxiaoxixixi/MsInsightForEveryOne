<script setup lang="ts">
import type { TreeNodeType } from './types';
import LocalIcon from '@/components/icons/loaclImport_icon.vue';
import DeletePopConfirm from '@/components/MenuTree/DeletePopConfirm.vue';
import EditableText from '@/components/MenuTree/EditableText.vue';
import AddLightIcon from '@/components/icons/add_light_icon.vue';
import AddDarkIcon from '@/components/icons/add_dark_icon.vue';
import {onMounted, ref, watch} from 'vue';
import {useDataSources} from '@/stores/dataSource';
import { LOCAL_HOST, PORT, ProjectActionEnum, type DataSource } from '@/centralServer/websocket/defs';
import useWatchTranslation from '@/hooks/useWatchTranslation';
import ResourceDialog from '@/components/ResourceDialog.vue';
import {useSession} from '@/stores/session';
import ContextMenu from '@/components/ContextMenu.vue';
import {t} from '@/i18n';
import { useCompareConfig } from '@/stores/compareConfig';

const { session } = useSession();
const showModal = ref(false);
const projectName = ref('');
const activateNodeId = ref(0);

const selectProjectExplorerInfo = ref({ projectName: '', fileName: '' });
const setBaseLineData = (): void => {
  useCompareConfig().setBaselineData(selectProjectExplorerInfo.value.projectName, selectProjectExplorerInfo.value.fileName);
};

const setCompareData = (): void => {
  useCompareConfig().setCompareData(selectProjectExplorerInfo.value.projectName, selectProjectExplorerInfo.value.fileName);
};

const menuVisible = ref(false);
const menuItems = ref([
  { label: t('Set Baseline Data') as string, action: setBaseLineData },
  { label: t('Set Compare Data') as string, action: setCompareData },
]);
const menuPosition = ref({
  top: '0px',
  left: '0px',
});
const contextMenu = ref();

onMounted(() => {
  window.addEventListener('click', (event: MouseEvent) => {
    // 如果菜单不可见，则不处理
    if (!menuVisible.value) {
      return;
    }
    // 如果dom元素不存在，则直接返回
    const menuRect = document.getElementById('right-click-menu')?.getBoundingClientRect();
    if (!menuRect) {
      return;
    }
    // 判断鼠标点击位置是否在菜单外
    const clickInsideMenu =
        event.clientX >= menuRect.left &&
        event.clientX <= menuRect.right &&
        event.clientY >= menuRect.top &&
        event.clientY <= menuRect.bottom;
    // 如果点击不在菜单内，则隐藏菜单
    if (!clickInsideMenu) {
      menuVisible.value = false;
    }
  });
});


const [DeleteAll, DeleteItem, Cancel, Confirm, ImportData] = useWatchTranslation(['Delete All', 'Delete Item', 'Cancel', 'Confirm', 'Import Data']);
const props = defineProps<{
    dataSource: TreeNodeType[];
}>();

watch(
    () => [useDataSources().lastDataSource, props.dataSource],
    ()=>{
      const idx = props.dataSource.findIndex((item) => item.label === useDataSources().lastDataSource?.projectName);
      if (idx === -1) {
        activateNodeId.value = -1;
        return;
      }

      const project = props.dataSource[idx];
      if (useDataSources().lastDataSource?.dataPath.length === 0) {
        activateNodeId.value = project.id;
        return;
      }

      if (!project.children) {
        activateNodeId.value = -1;
        return;
      }
      const subdirectoryIdx = project.children.findIndex((item) => item.label === useDataSources().lastDataSource.dataPath[0]);
      if (subdirectoryIdx === -1) {
        activateNodeId.value = -1;
        return;
      }
      activateNodeId.value = project.children[subdirectoryIdx].id;
    }
);

const handleNodeClick = (data:any, node: any) => {
  let dataSource = { remote: LOCAL_HOST, port: PORT, projectName: '', dataPath: [] } as DataSource;
  if (node.level === 1) {
    dataSource.projectName = data.label;
  } else {
    dataSource.projectName = data.projectName;
    dataSource.dataPath.push(data.label);
  }
  useDataSources().confirm(dataSource, false, ProjectActionEnum.TRANSFER_PROJECT);
};

function addRemoteUnderProject(node:any, e: MouseEvent) {
  e.stopPropagation();
  projectName.value = node.data.label;
  showModal.value = true;
}

const handleRightClick = (event: MouseEvent, data:any) => {
  selectProjectExplorerInfo.value.projectName = data.projectName;
  selectProjectExplorerInfo.value.fileName = data.label;
  menuPosition.value.top = `${event.clientY}px`;
  menuPosition.value.left = `${event.clientX}px`;
  menuVisible.value = true;
};

const getToolTip = (data:any, node: any): string => {
  if (node.level === 1) {
    return data.label;
  }
  const rankId = useCompareConfig().getRankIdByProjectInfo(data.projectName, data.label);
  if (rankId === '') {
    return data.label;
  }
  return rankId + ' : ' + data.label;
};

</script>
<template>
    <div class="menu-tree">
        <el-tree :data="props.dataSource" node-key="id" :default-expand-all="true" :expand-on-click-node="false" :current-node-key="2" @node-click="handleNodeClick">
            <template #default="{ node, data }">
                <div :class="['content-node', {'activate-node':data.id === activateNodeId}]">
                    <span class="content-body">
                        <LocalIcon v-if="node.level === 1" style="flex: none"/>
                        <el-tooltip :content="getToolTip(data, node)" effect="light" :show-after="200">
                            <EditableText  v-if="node.level === 1" :tree-node="node" :key="data.id + data.label"></EditableText>
                            <span v-else class="content-node-text" @contextmenu.prevent="handleRightClick($event, data)">{{ node.label }} </span>
                        </el-tooltip>
                    </span>

                    <div class="btn-box">
                      <el-tooltip v-if="node.level === 1" :content="ImportData" effect="light">
                          <el-icon class="icon-button" @click.stop="addRemoteUnderProject(node, $event)">
                              <AddDarkIcon  v-if="session.theme=='dark'"/>
                              <AddLightIcon v-else/>
                          </el-icon>
                      </el-tooltip>

                      <el-tooltip :content="node.level === 1 ? DeleteAll : DeleteItem" effect="light">
                        <div class="icon-button">
                          <DeletePopConfirm :data="data" :node="node" :is-delete-all="node.level === 1"/>
                        </div>
                      </el-tooltip>
                    </div>
                </div>
            </template>
        </el-tree>
        <ResourceDialog v-model:showModal=showModal :project-name="projectName"></ResourceDialog>
        <ContextMenu id="right-click-menu" ref="contextMenu" v-model:menuVisible=menuVisible :menu-items="menuItems" :position="menuPosition"/>
    </div>
</template>
<style scoped>
.menu-tree {
    padding: 0.5rem 0.8rem;
}


:deep(.el-tree-node__content:has(.activate-node)) {
  background-color: var(--mi-bg-color-light);
}

.content-node {
    position: relative;
    display: flex;
    user-select: none;
    justify-content: space-between;
    flex: 1;
    line-height: 20px;
    width: calc(100% - 18px);
}

.content-node:hover .btn-box,
.content-node.activate-node .btn-box{
    display: flex;
}

.content-body {
    display: flex;
    align-items: center;
    width: 0;
    flex: 1 1 auto;
}

.content-node-text {
    padding: 0 0 0 2px;
    font-size: 12px;
    color: var(--mi-text-color-tertiary);
    font-weight: 400;
    overflow: hidden;
    white-space: nowrap;
    text-overflow: ellipsis;
}
:deep(.el-tree-node__children .el-tree-node:focus > .el-tree-node__content) {
    background-color: transparent;
}
:deep(.el-tree-node__children .el-tree-node__content:hover) {
    background-color: transparent;
}
.btn-box{
  display: none;
  align-items: center;
  justify-content: end;
  width: 40px;
  margin-left: 10px;
}
.icon-button{
  margin-left: 4px;
}
</style>

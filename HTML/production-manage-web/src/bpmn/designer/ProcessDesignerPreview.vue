<template>
  <div class="my-process-designer">
    <div class="my-process-designer__container">
      <div class="my-process-designer__canvas" ref="bpmn-canvas"></div>
    </div>
  </div>
</template>

<script>
import BpmnModeler from "bpmn-js/lib/Modeler";
// 引入json转换与高亮
import { getFlowXml } from "@/api/process";
import "highlight.js/styles/github-gist.css";
export default {
  name: "ProcessDesignerPreview",
  props: {
    // 流程具体信息， 用于流转状态的颜色区分
    instanceData: {
      type: Object,
      default: () => {
        return {};
      },
    },
  },
  inject: {
    setNextKey: {
      default: () => null,
    },
  },
  data() {
    return {
      bpmnModeler: null,
    };
  },
  mounted() {
    this.initBpmnModeler();
  },
  beforeDestroy() {
    if (this.bpmnModeler) this.bpmnModeler.destroy();
    this.bpmnModeler = null;
  },
  methods: {
    /**
     * 初始化
     * @param {*} val
     * @param {Boolean} fit 是否将内容居中, 默认居中. 某些display:none的组件请关闭该项,否则会渲染失败
     */
    async initDiagram(val, instance = {}, fit = true) {
      if (val) {
        const { deploymentId, resourceName } = val;
        const da = await getFlowXml({ deploymentId, resourceName });
        this.createNewDiagram(da, instance, fit);
      }
    },
    /**
     * 创建新的流程图
     * fit是否将内容居中, 默认居中. 某些display:none的组件请关闭该项,否则会渲染失败
     */
    async createNewDiagram(xml, instance, fit) {
      // 将字符串转换成图显示出来
      try {
        let { warnings } = await this.bpmnModeler.importXML(xml);
        // 根据流转状态 重绘颜色 开始
        let elementRegistry = this.bpmnModeler.get("elementRegistry");
        let modeling = this.bpmnModeler.get("modeling");
        //处理已完成流程
        let highShapeIds = instance.highPoint || [];
        let waitShapeIds = instance.waitingToDo || [];
        let assigneeInfos = instance.assigneeInfos || [];
        highShapeIds.forEach((id) => {
          let shape = elementRegistry.get(id);
          if (shape) {
            modeling.setColor(shape, { stroke: "green" });
          }
        });
        //获取下一节点
        waitShapeIds.forEach((id) => {
          let shape = elementRegistry.get(id);
          if (shape) {
            modeling.setColor(shape, { stroke: "red" });
            if (this.setNextKey) {
              const nextKey = shape.businessObject.outgoing[0].targetRef.id;
              this.setNextKey(nextKey);
            }
          }
        });
        // 节点修改名称
        //办理业务-任务流程
        assigneeInfos.forEach((item) => {
          let shape = elementRegistry.get(item.taskKey);
          if (shape) {
            const label = shape.businessObject.name || "";
            modeling.updateLabel(
              shape,
              `${label}\n${item.assignee || ""}\n${label!=='发起人'?item.assigneeRoles|| "":'' }`
            );
          }
        });
        // 根据流转状态 重绘颜色 结束
        if (warnings && warnings.length) {
          warnings.forEach((warn) => console.warn(warn));
        }
        if (fit) {
          const canvas = this.bpmnModeler.get("canvas");
          canvas.zoom("fit-viewport", "auto");
        }
      } catch (e) {
        console.error(`[Process Designer Warn]: ${e?.message || e}`);
      }
    },
    refit() {
      const canvas = this.bpmnModeler.get("canvas");
      canvas.zoom("fit-viewport", "auto");
    },
    initBpmnModeler() {
      if (this.bpmnModeler) this.bpmnModeler.destroy();
      this.bpmnModeler = new BpmnModeler({
        container: this.$refs["bpmn-canvas"],
        additionalModules: [
          {
            paletteProvider: ["value", ""], //禁用/清空左侧工具栏
            labelEditingProvider: ["value", ""], //禁用节点编辑
            contextPadProvider: ["value", ""], //禁用图形菜单
            bendpoints: ["value", {}], //禁用连线拖动
            zoomScroll: ["value", ""], //禁用滚动
            // moveCanvas: ['value', ''], //禁用拖动整个流程图
            move: ["value", ""], //禁用单个图形拖动
          },
        ],
      });
    },
  },
};
</script>

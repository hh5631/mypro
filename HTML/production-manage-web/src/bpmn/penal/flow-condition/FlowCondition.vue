<template>
  <div class="panel-tab__content">
    <el-form
      :model="flowConditionForm"
      label-width="90px"
      size="mini"
      @submit.native.prevent
    >
      <el-form-item label="流转类型">
        <el-select v-model="flowConditionForm.type" @change="updateFlowType">
          <el-option label="普通流转路径" value="normal" />
          <el-option label="默认流转路径" value="default" />
          <el-option label="条件流转路径" value="condition" />
        </el-select>
      </el-form-item>
      <el-form-item
        label="条件格式"
        v-if="flowConditionForm.type === 'condition'"
        key="condition"
      >
        <el-select
          v-model="flowConditionForm.conditionType"
          @change="updateConditionType"
        >
          <el-option label="判断条件" value="expression" />
          <el-option
            :label="processType == '自定义流程' ? '表单条件' : '流程条件'"
            value="formExpression"
          />

          <!-- <el-option label="脚本" value="script" /> -->
        </el-select>
      </el-form-item>
      <el-form-item
        label="判断条件"
        v-if="
        flowConditionForm.type == 'condition'&&
          flowConditionForm.conditionType &&
          flowConditionForm.conditionType === 'expression'
        "
        key="express"
      >
        <el-select
          v-model="flowConditionForm.body"
          placeholder="请选择"
          @change="updateFlowCondition"
        >
          <el-option label="同意" value="${result==1}"> </el-option>
          <el-option label="拒绝" value="${result==0}"> </el-option>
        </el-select>
      </el-form-item>
      <template
        v-if="
          flowConditionForm.type == 'condition' &&
          flowConditionForm.conditionType &&
          flowConditionForm.conditionType === 'formExpression'
        "
      >
        <h2>表单表达式</h2>
        <div
          class="form-condition"
          v-for="(item, index) in formCondition"
          :key="index"
        >
          <el-select
            v-model="item.formConditionLabel"
            placeholder="字段"
            @change="formConditionChange"
          >
            <el-option
              v-for="item in formAttributes"
              :key="item.dictValue"
              :label="item.dictLabel"
              :value="item.dictValue"
            >
            </el-option>
            <!-- <el-option label="拒绝" value="${result==0}"> </el-option> -->
          </el-select>
          <el-select
            v-model="item.formConditionSymbol"
            placeholder="符号"
            @change="formConditionChange"
            class="form-symbol"
          >
            <el-option
              v-for="(item, index) in symbolList"
              :key="index"
              :label="item"
              :value="item"
            >
            </el-option>
            <!-- <el-option label="拒绝" value="${result==0}"> </el-option> -->
          </el-select>
          <el-input
            style="width: 120px"
            v-model="item.formConditionStr"
            clearable
            placeholder="值"
            @change="formConditionChange"
          />
          <el-button
            size="small"
            type="primary"
            icon="el-icon-plus"
            @click="handleAdd(index)"
            circle
          ></el-button>
          <el-button
            size="small"
            type="danger"
            icon="el-icon-minus"
            @click="handleDel(index)"
            circle
          ></el-button>
        </div>
      </template>
      <template
        v-if="
          flowConditionForm.conditionType &&
          flowConditionForm.conditionType === 'script'
        "
      >
        <el-form-item label="脚本语言" key="language">
          <el-input
            v-model="flowConditionForm.language"
            clearable
            @change="updateFlowCondition"
          />
        </el-form-item>
        <el-form-item label="脚本类型" key="scriptType">
          <el-select v-model="flowConditionForm.scriptType">
            <el-option label="内联脚本" value="inlineScript" />
            <el-option label="外部脚本" value="externalScript" />
          </el-select>
        </el-form-item>
        <el-form-item
          label="脚本"
          v-if="flowConditionForm.scriptType === 'inlineScript'"
          key="body"
        >
          <el-input
            v-model="flowConditionForm.body"
            type="textarea"
            clearable
            @change="updateFlowCondition"
          />
        </el-form-item>
        <el-form-item
          label="资源地址"
          v-if="flowConditionForm.scriptType === 'externalScript'"
          key="resource"
        >
          <el-input
            v-model="flowConditionForm.resource"
            clearable
            @change="updateFlowCondition"
          />
        </el-form-item>
      </template>
    </el-form>
  </div>
</template>

<script>
import { getDicts } from "@/api/system/dict/data";
export default {
  name: "FlowCondition",
  props: {
    businessObject: Object,
    type: String,
    formJson: Object,
    id: String,
    processType: String,
  },
  data() {
    return {
      flowConditionForm: {},
      formAttributes: [],
      formCondition: [
        {
          formConditionLabel: "", //表单条件字段
          formConditionSymbol: "", ///表单条件符号
          formConditionStr: "", //
        },
      ],
      symbolList: [">", "<", "==", "<=", ">="],
    };
  },
  watch: {
    // businessObject: {
    //   immediate: true,
    //   handler() {
    //     this.$nextTick(() => this.resetFlowCondition());
    //   },
    // },
    id: {
      immediate: true,
      handler() {
        this.$nextTick(() => this.resetFlowCondition());
      },
    },
    processType: {
      immediate: true,
      handler(newVal) {
        if (newVal == "业务流程") {
          this.getConditionByDict();
        }
      },
    },
    formJson: {
      immediate: true,
      handler() {
        this.formAttributes = [];
        let widgetList = this.formJson.widgetList;
        if (widgetList && widgetList.length > 0) {
          widgetList.forEach((wItem) => {
            this.getFormAttributes(wItem);
          });
        }
      },
    },
  },
  methods: {
    getConditionByDict() {
      getDicts("process_variables_biz_key").then((da) => {
        this.formAttributes = da.data;
      });
    },
    handleAdd(index) {
      this.formCondition.splice(index + 1, 0, {
        formConditionLabel: "", //表单条件字段
        formConditionSymbol: "", ///表单条件符号
        formConditionStr: "", //
      });
    },
    handleDel(index) {
      this.formCondition.splice(index, 1);
    },
    //回显重组数据
    eorganizationData(data) {
      this.formCondition = [];
      data.forEach((item) => {
        const symbol = item.replace(/[a-zA-Z\d.]/g, "");
        const arr = item.split(symbol);
        this.formCondition.push({
          formConditionLabel: arr[0],
          formConditionSymbol: symbol,
          formConditionStr: arr[1],
        });
      });
    },
    updateConditionType() {
      this.formCondition = [
        {
          formConditionLabel: "", //表单条件字段
          formConditionSymbol: "", ///表单条件符号
          formConditionStr: "", //
        },
      ];
      this.flowConditionForm.body=''
    },
    //表单条件修改
    formConditionChange() {
      let str = "";
      this.formCondition.forEach((item) => {
        if (str) {
          str += `&&${
            item.formConditionLabel +
            item.formConditionSymbol +
            item.formConditionStr
          }`;
        } else {
          str += `${
            item.formConditionLabel +
            item.formConditionSymbol +
            item.formConditionStr
          }`;
        }
      });
      this.flowConditionForm.body = "${" + str + "}";
      this.updateFlowCondition();
    },
    //获取表单属性
    getFormAttributes(wItem) {
      if (wItem.category === "container") {
        if (wItem.type === "grid") {
          if (!!wItem.cols && wItem.cols.length > 0) {
            wItem.cols.forEach((childItem) => {
              this.getFormAttributes(childItem);
            });
          }
        } else if (wItem.type === "table") {
          if (!!wItem.rows && wItem.rows.length > 0) {
            wItem.rows.forEach((rowItem) => {
              if (!!rowItem.cols && rowItem.cols.length > 0) {
                rowItem.cols.forEach((colItem) => {
                  this.getFormAttributes(colItem);
                });
              }
            });
          }
        } else if (wItem.type === "tab") {
          if (!!wItem.tabs && wItem.tabs.length > 0) {
            wItem.tabs.forEach((tabItem) => {
              if (!!tabItem.widgetList && tabItem.widgetList.length > 0) {
                tabItem.widgetList.forEach((childItem) => {
                  this.getFormAttributes(childItem);
                });
              }
            });
          }
        } else if (wItem.type === "sub-form") {
          this.formAttributes.push({
            dictLabel: wItem.options.label,
            dictValue: wItem.options.name,
          });
        } else if (wItem.type === "grid-col" || wItem.type === "table-cell") {
          if (!!wItem.widgetList && wItem.widgetList.length > 0) {
            wItem.widgetList.forEach((childItem) => {
              this.getFormAttributes(childItem);
            });
          }
        } else {
          //自定义容器组件
          if (!!wItem.widgetList && wItem.widgetList.length > 0) {
            wItem.widgetList.forEach((childItem) => {
              this.getFormAttributes(childItem);
            });
          }
        }
      } else if (!!wItem.formItemFlag) {
        this.formAttributes.push({
          dictLabel: wItem.options.label,
          dictValue: wItem.options.name,
        });
      }
    },
    resetFlowCondition() {
      this.bpmnElement = window.bpmnInstances.bpmnElement;
      this.bpmnElementSource = this.bpmnElement.source;
      this.bpmnElementSourceRef = this.bpmnElement.businessObject.sourceRef;
      if (
        this.bpmnElementSourceRef &&
        this.bpmnElementSourceRef.default &&
        this.bpmnElementSourceRef.default.id === this.bpmnElement.id
      ) {
        // 默认
        this.flowConditionForm = { type: "default" };
      } else if (!this.bpmnElement.businessObject.conditionExpression) {
        // 普通
        this.flowConditionForm = { type: "normal" };
      } else {
        // 带条件
        const conditionExpression =
          this.bpmnElement.businessObject.conditionExpression;
        this.flowConditionForm = { ...conditionExpression, type: "condition" };
        // resource 可直接标识 是否是外部资源脚本
        if (this.flowConditionForm.resource) {
          this.$set(this.flowConditionForm, "conditionType", "script");
          this.$set(this.flowConditionForm, "scriptType", "externalScript");
          return;
        }
        if (conditionExpression.language) {
          this.$set(this.flowConditionForm, "conditionType", "script");
          this.$set(this.flowConditionForm, "scriptType", "inlineScript");
          return;
        }
        if (this.flowConditionForm.body) {
          if (
            this.flowConditionForm.body == "${result==1}" ||
            this.flowConditionForm.body == "${result==0}"
          ) {
            this.$set(this.flowConditionForm, "conditionType", "expression");
          } else {
            this.$set(
              this.flowConditionForm,
              "conditionType",
              "formExpression"
            );
            const str = this.flowConditionForm.body.substring(
              2,
              this.flowConditionForm.body.length - 1
            );
            this.eorganizationData(str.split("&&"));
          }
        }
      }
    },
    updateFlowType(flowType) {
      // 正常条件类
      if (flowType === "condition") {
        this.flowConditionRef = window.bpmnInstances.moddle.create(
          "bpmn:FormalExpression"
        );
        window.bpmnInstances.modeling.updateProperties(this.bpmnElement, {
          conditionExpression: this.flowConditionRef,
        });
        return;
      }
      // 默认路径
      if (flowType === "default") {
        window.bpmnInstances.modeling.updateProperties(this.bpmnElement, {
          conditionExpression: null,
        });
        window.bpmnInstances.modeling.updateProperties(this.bpmnElementSource, {
          default: this.bpmnElement,
        });
        return;
      }
      // 正常路径，如果来源节点的默认路径是当前连线时，清除父元素的默认路径配置
      if (
        this.bpmnElementSourceRef.default &&
        this.bpmnElementSourceRef.default.id === this.bpmnElement.id
      ) {
        window.bpmnInstances.modeling.updateProperties(this.bpmnElementSource, {
          default: null,
        });
      }
      window.bpmnInstances.modeling.updateProperties(this.bpmnElement, {
        conditionExpression: null,
      });
    },
    updateFlowCondition() {
      let { conditionType, scriptType, body, resource, language } =
        this.flowConditionForm;
      let condition;
      if (
        conditionType === "expression" ||
        conditionType === "formExpression"
      ) {
        condition = window.bpmnInstances.moddle.create(
          "bpmn:FormalExpression",
          { body }
        );
        this.$forceUpdate()
      } else {
        if (scriptType === "inlineScript") {
          condition = window.bpmnInstances.moddle.create(
            "bpmn:FormalExpression",
            { body, language }
          );
          this.$set(this.flowConditionForm, "resource", "");
        } else {
          this.$set(this.flowConditionForm, "body", "");
          condition = window.bpmnInstances.moddle.create(
            "bpmn:FormalExpression",
            { resource, language }
          );
        }
      }
      window.bpmnInstances.modeling.updateProperties(this.bpmnElement, {
        conditionExpression: condition,
      });
    },
  },
  beforeDestroy() {
    this.bpmnElement = null;
    this.bpmnElementSource = null;
    this.bpmnElementSourceRef = null;
  },
};
</script>
<style lang="scss" scoped>
.form-condition {
  display: flex;
}
.form-symbol {
  ::v-deep .el-input {
    width: 65px !important;
  }
}
</style>

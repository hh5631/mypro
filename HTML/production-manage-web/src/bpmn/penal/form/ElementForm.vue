<template>
  <div class="panel-tab__content">
    <el-form size="mini" label-width="90px">
      <el-form-item label="表单" prop="formId">
        <el-cascader
          v-model="categoryForm"
          :options="formList"
          :props="{ expandTrigger: 'hover' }"
          filterable
          @change="handleChange"
        >
          <template slot-scope="{ data }">
            <el-popover
              v-show="data.formJson"
              placement="left"
              trigger="hover"
              width="720"
            >
              <v-form-render
                :form-json="data.formJson ? JSON.parse(data.formJson) : {}"
              />
              <el-button slot="reference" size="small" type="text" class="popover-btn">{{
                data.label
              }}</el-button>
            </el-popover>
            <span v-show="!data.formJson">{{ data.label }}</span>
          </template>
        </el-cascader>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
import { getFormDetail } from "@/api/process";
import { getFormCascaderData } from "@/utils/process";
export default {
  name: "ElementForm",
  props: {
    id: String,
    type: String,
  },
  inject: {
    prefix: "prefix",
    width: "width",
  },
  data() {
    return {
      defaultForm: {
        formId: "",
      },
      form: {},
      formList: [],
      categoryForm: [],
    };
  },
  watch: {
    id: {
      immediate: true,
      handler(val) {
        this.bpmnElement = window.bpmnInstances.bpmnElement;
        this.$nextTick(() => this.resetFormList());
      },
    },
  },
  mounted() {
    this.getForm();
  },
  methods: {
    //获取表单信息
    getForm() {
      getFormCascaderData().then((da) => {
        this.formList = da;
      });
    },
    resetFormList() {
      for (let key in this.defaultForm) {
        let value =
          this.bpmnElement?.businessObject[key] || this.defaultForm[key];
        this.$set(this.form, key, value);
      }
      if (this.form.formId) {
        getFormDetail(this.form.formId).then((da) => {
          this.categoryForm = [da.data.categoryId, da.data.id];
        });
      } else {
        this.categoryForm = [];
      }
    },
    updateElementForm(key) {
      const formAttr = Object.create(null);
      formAttr[key] = this.form[key] || null;
      window.bpmnInstances.modeling.updateProperties(
        this.bpmnElement,
        formAttr
      );
    },
    handleChange(val) {
      this.form.formId = val[1];
      this.updateElementForm("formId");
    },
  },
};
</script>
<style lang="scss" scoped>
.popover-btn {
  color: #606266;
  width: 100%;
  text-align: left;
}
</style>
<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-07 13:32:13
-->
<template>
  <div class="addCss">
    <el-form :model="assetForm">
      <div>基础信息</div>
      <div class="form_item">
        <div class="item">
          <div class="item-label">资产编码：</div>
          <div class="item-text">{{ assetForm.internalCode }}</div>
        </div>
        <div class="item">
          <div class="item-label">资产类别：</div>
          <div class="item-text">{{ assetForm.type }}</div>
        </div>
        <div class="item">
          <div class="item-label">资产名称：</div>
          <div class="item-text">{{ assetForm.name }}</div>
        </div>
      </div>
      <div class="form_item">
        <div class="item">
          <div class="item-label">品牌：</div>
          <div class="item-text">{{ assetForm.brandName }}</div>
        </div>
        <div class="item">
          <div class="item-label">规格型号：</div>
          <div class="item-text">{{ assetForm.standard }}</div>
        </div>
        <div class="item">
          <div class="item-label">税前单价（元）：</div>
          <div class="item-text">{{ assetForm.price }}</div>
        </div>
      </div>
      <div>
        <div>盘亏说明</div>
        <div class="form_item_lg">
          <el-form-item prop="remark">
            <el-input
              v-model="assetForm.remark"
              type="textarea"
              :rows="2"
              placeholder="请填写盘亏说明"
            />
          </el-form-item>
        </div>
      </div>
      <div class="form-footer">
        <el-button @click="cancle">取消</el-button>
        <el-button type="primary" @click="submit">保存</el-button>
      </div>
    </el-form>
  </div>
</template>

<script>

export default {
  name: 'Add',
  props: {

  },
  data () {
    return {
      assetForm: {}

    }
  },
  created () {
  },
  methods: {
    submit() {
      this.$emit('submitForm', this.assetForm)
    },
    cancle() {
      this.$emit('cancel')
    }
  }
}
</script>

<style lang="scss" scoped>
.addCss{
  .form_item{
    display: flex;
    justify-content: space-between;

    margin: 15px 0;
    .item{
      display: flex;
      align-items: flex-start;
      width: 30%;
    }
    .item-label{
      width:30%
    }
    .item-text{
      width:60%;
    }
  }
  .form_item_lg{
    margin: 15px 0;

    width: 100%;
  }
  .date_box {
    position: relative;
    width: 100%;
    .data_icon {
      position: absolute;
      top: 50%;
      right: 17px;
      z-index: 9;
      color: #c0c4cc;
      font-size: 14px;
      transform: translateY(-50%);
    }
    ::v-deep .el-range__close-icon {
      position: absolute;
      right: 34px;
      top: 54%;
      transform: translateY(-50%);
    }
    ::v-deep .el-date-editor .el-input__inner {
      border: none;
    }
  }
  .form-footer{
    text-align: end;
  }
}
</style>

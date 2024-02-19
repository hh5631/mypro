<template>
  <div>
    <CommonTitle :title-type="titleType" />
    <div class="table-con">
      <table class="table1">
        <tbody>
          <tr>
            <td style="width: 180px;">
              <div class="t1">合同编号：</div>
            </td>
            <td>
              <div class="t1" />
            </td>
            <td style="width: 180px;">
              <div class="t1">发票号：</div>
            </td>
            <td>
              <div class="t1" />
            </td>
          </tr>
          <tr>
            <td>供应商名称：</td>
            <td colspan="3">{{ infoItem.supplierName }}</td>
          </tr>
          <tr>
            <td>合计金额：</td>
            <td colspan="3">{{ money(infoItem.totalPrice) }}</td>
          </tr>
        </tbody>
      </table>
      <!-- 入库确认资产明细表 -->
      <table class="table2">
        <thead>
          <tr>
            <td style="width: 80px;">序号</td>
            <td>名称</td>
            <td>型号</td>
            <td>数量</td>
            <td>单价</td>
            <td>金额</td>
            <td>备注</td>
          </tr>
        </thead>
        <tbody v-if="infoItem.detailVOS && infoItem.detailVOS.length">
          <tr v-for="(item,index) in infoItem.detailVOS" :key="item.id">
            <td>{{ index + 1 }}</td>
            <td>{{ item.materialName }}</td>
            <td>{{ item.materialStandard }}</td>
            <td><span v-if="item.qty && item.unitName">{{ item.qty }} {{ item.unitName }}</span></td>
            <td><span v-if="item.price">{{ item.price.toFixed(2) }}元</span></td>
            <td><span v-if="item.price && item.taxAmount">{{ (item.price + item.taxAmount).toFixed(2) }}元</span></td>
            <td>{{ item.remark }}</td>
          </tr>
          <tr>
            <td />
            <td>合计：<span v-if="infoItem.totalPrice">{{ infoItem.totalPrice.toFixed(2) }}元</span></td>
            <td />
            <td />
            <td />
            <td />
            <td />
          </tr>
        </tbody>
      </table>
    </div>
    <div class="tip">经现场检测人确认，设备、材料或产品合格无误。</div>
    <div class="name-line">
      <span class="name">
        <span>现场检测人：</span>
        <span class="realname" />
      </span>
      <span class="name" style="margin-left:50px;">
        <span>库管员：</span>
        <span class="realname" />
      </span>
    </div>
  </div>
</template>

<script>
import CommonTitle from './commonTitle.vue'
import { money } from '@/utils/index.js'
export default {
  name: 'WarehousingPrintTable',
  components: { CommonTitle },
  props: {
    titleType: {
      type: String,
      default: ''
    },
    infoItem: {
      type: Object,
      default: () => {}
    }
  },
  methods: {
    money
  }
}
</script>

<style lang="scss" scoped>
  .tip {
    text-align: left;
    padding: 10px 0 0px 20px;
  }
</style>

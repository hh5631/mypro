<template>
  <div>
    <CommonTitle :title-type="titleType" />
    <div class="table-con">
      <table class="table1">
        <tbody>
          <tr>
            <td style="width: 150px;">
              <div class="t1">领用部门：{{ infoItem.length>0? infoItem[0].dept:'' }}</div>
            </td>
            <td>
              <div class="t1" />
            </td>
            <td style="width: 150px;">
              <div class="t1">项目名称：</div>
            </td>
            <td>
              <div class="t1" />
            </td>
          </tr>
          <tr>
            <td>合计金额（大写）：</td>
            <td colspan="3">{{ money(infoItem.totalPrice) }}</td>
          </tr>
        </tbody>
      </table>
      <!-- 领用出库资产明细表 -->
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
        <tbody v-if="infoItem && infoItem.length">
          <tr v-for="(item,index) in infoItem" :key="item.id">
            <td>{{ index + 1 }}</td>
            <td>{{ item.name }}</td>
            <td>{{ item.standard }}</td>
            <td><span v-if="item.unit" />1{{ item.unit }}</td>
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
    <div class="name-line">
      <span class="name">
        <span>库管员：</span>
        <span class="realname" />
      </span>
      <span class="name" style="margin-left:50px;">
        <span>领用人：</span>
        <span class="realname" />
      </span>
      <span class="name" style="margin-left:50px;">
        <span>审核人：</span>
        <span class="realname" />
      </span>
    </div>
  </div>
</template>

<script>
import CommonTitle from './commonTitle.vue'
import { money } from '@/utils/index.js'
export default {
  name: 'OutboundOrderPrintTable',
  components: { CommonTitle },
  props: {
    titleType: {
      type: String,
      default: ''
    },
    infoItem: {
      type: Array,
      default: () => []
    }
  },
  mounted() {
  },
  methods: {
    money
  }
}
</script>

<style lang="scss" scoped>

</style>

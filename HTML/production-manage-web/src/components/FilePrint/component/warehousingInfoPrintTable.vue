<template>
  <div>
    <CommonTitle :title-type="titleType" />
    <div class="table-con">
      <table class="table1">
        <tbody>
          <tr>
            <td style="width: 180px;">
              <div class="t1">退库单号：</div>
            </td>
            <td>
              <div class="t1">{{ infoItem.orderNo }}</div>
            </td>
            <td style="width: 180px;">
              <div class="t1">申请时间：</div>
            </td>
            <td>
              <div class="t1">{{ infoItem.createTime }}</div>
            </td>
          </tr>
          <tr>
            <td style="width: 180px;">
              <div class="t1">退库人：</div>
            </td>
            <td>
              <div class="t1" />
            </td>
            <td style="width: 180px;">
              <div class="t1">使用部门：</div>
            </td>
            <td>
              <div class="t1" />
            </td>
          </tr>
          <tr>
            <td>退库说明：</td>
            <td colspan="3">
              <div class="t1" />
            </td>
          </tr>
        </tbody>
      </table>
      <!-- 确认出库资产详情表 -->
      <table class="table2">
        <thead>
          <tr>
            <td style="width: 80px;">序号</td>
            <td>资产编码</td>
            <td>资产名称</td>
            <td>品牌</td>
            <td>规格型号</td>
            <td>计量单位</td>
          </tr>
        </thead>
        <tbody v-if="infoItem.wareBackDetails && infoItem.wareBackDetails.length">
          <tr v-for="(item,index) in infoItem.wareBackDetails" :key="item.id">
            <td>{{ index + 1 }}</td>
            <td>{{ item.internalCode }}</td>
            <td>{{ item.name }}</td>
            <td>{{ item.brandName }}</td>
            <td>{{ item.standard }}</td>
            <td>{{ item.unit }}</td>
          </tr>
        </tbody>
      </table>
    </div>
    <div class="empty" />
    <div class="name-line">
      <span class="name">
        <span>退库人（签名）：</span>
        <span class="realname" />
      </span>
      <span class="name" style="margin-left:50px;">
        <span>资产管理专员（签名）：</span>
        <span class="realname" />
      </span>
    </div>
  </div>
</template>

<script>
import CommonTitle from './commonTitle.vue'
export default {
  name: 'WarehousingInfoPrintTable',
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
  }
}
</script>

<style lang="scss" scoped>

</style>

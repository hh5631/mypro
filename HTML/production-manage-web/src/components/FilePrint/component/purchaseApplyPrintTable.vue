<template>
  <div>
    <CommonTitle :title-type="titleType" />
    <!-- 具体打印表格内容 -->
    <div class="table-con">
      <!-- 采购申请表 -->
      <table class="table4" style="border-collapse:collapse;" cellspacing="0" cellpadding="0">
        <!-- 采购表表头 -->
        <thead>
          <tr>
            <td>名称</td>
            <td style="width: 300px;">规格型号</td>
            <td style="width: 40px;">计量单位</td>
            <td>数量</td>
            <td style="width: 70px;">单价</td>
            <td style="width: 70px;">预计金额</td>
            <td>请购理由</td>
          </tr>
        </thead>
        <!-- 采购表表格内容 -->
        <tbody>
          <div v-if="infoItem.detailVOS && infoItem.detailVOS.length>0">
            <tr v-for="(item) in infoItem.detailVOS" :key="item.id" style="height:40px;">
              <td>{{ item.material.name }}</td>
              <td>{{ item.material.standard }}</td>
              <td><span v-if="item.material.unit" />{{ item.material.unit }}</td>
              <td>{{ item.purchaseQty }}</td>
              <td><span v-if="item.unitPrice">{{ Number(item.unitPrice/100).toFixed(2) }}元</span></td>
              <td><span v-if="item.totalPrice">{{ Number(item.totalPrice/100).toFixed(2) }}元</span></td>
              <td>{{ item.remark }}</td>
            </tr>
          </div>
          <div v-else>
            <tr v-for="(item,index) in 3" :key="index" style="height:40px;">
              <td />
              <td />
              <td />
              <td />
              <td />
              <td />
              <td />
            </tr>
          </div>
          <tr>
            <td colspan="7" style="width:100%;height:40px;">合计：<span v-if="infoItem.totalPrice">{{ Number(infoItem.totalPrice/100).toFixed(2) }}元</span></td>
          </tr>
          <tr>
            <td colspan="7" style="width:100%;height:80px;">拟选择供应商：</td>
          </tr>
          <!-- 申请人 -->
          <tr>
            <td colspan="7" class="t4-td">
              <div class="t4-top">申请人：</div>
              <div class="t4-bottom">
                <div class="t4-bottom-left">签字：</div>
                <div class="t4-bottom-right">
                  <span style="min-width:30px;display:inline-block;" />
                  <span>年</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>月</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>日</span>
                </div>
              </div>
            </td>
          </tr>
          <!-- 部门/团队负责人意见 -->
          <tr>
            <td colspan="7" class="t4-td">
              <div class="t4-top">部门/团队负责人意见：</div>
              <div class="t4-bottom">
                <div class="t4-bottom-left">签字：</div>
                <div class="t4-bottom-right">
                  <span style="min-width:30px;display:inline-block;" />
                  <span>年</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>月</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>日</span>
                </div>
              </div>
            </td>
          </tr>
          <!-- 中心副主任/主任意见 -->
          <tr>
            <td colspan="7" class="t4-td">
              <div class="t4-top">中心副主任/主任意见：</div>
              <div class="t4-bottom">
                <div class="t4-bottom-left">签字：</div>
                <div class="t4-bottom-right">
                  <span style="min-width:30px;display:inline-block;" />
                  <span>年</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>月</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>日</span>
                </div>
              </div>
            </td>
          </tr>
          <!-- 分管副院长/院长意见 -->
          <tr>
            <td colspan="7" class="t4-td">
              <div class="t4-top">分管副院长/院长意见：</div>
              <div class="t4-bottom">
                <div class="t4-bottom-left">签字：</div>
                <div class="t4-bottom-right">
                  <span style="min-width:30px;display:inline-block;" />
                  <span>年</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>月</span>
                  <span style="min-width:30px;display:inline-block;" />
                  <span>日</span>
                </div>
              </div>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
    <div>
      <div style="margin:5px 0">备注：取得增值税专用发票的，“单价”和“金额”请填写不含税金额；</div>
      <div style="margin-left:35px"> 采取线下申请形式的此表需经相应领导签批。</div>
    </div>
  </div>
</template>

<script>
import CommonTitle from './commonTitle.vue'
export default {
  name: 'PurchaseApplyPrintTable',
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
  .table4 {
    border-top: 1px solid #999;
  .t4-td{
    width:100%;height:120px;
  }
  .t4-top{
          margin-bottom: 20px;
        }
   .t4-bottom{
    display: flex;
    justify-content: space-between;
    align-items: center;
   }
   .t4-bottom-left{
         text-align: left;
         text-align: left;
         padding: 10px 0 10px ;
        }
    .t4-bottom-right{
      padding: 10px 50px 10px 0;
        }
  }
</style>

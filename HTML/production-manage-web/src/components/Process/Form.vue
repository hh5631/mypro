<!--
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-07-26 09:40:15
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-16 16:32:22
 * @FilePath: \production-manage-web\src\components\Process\Form.vue
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
-->
<template>
  <component :is="componentName" ref="components" />

</template>
<script>
import PurchaseApplyForm from '@/views/purchase/apply/applyDetail.vue'
const componentType = {
  'type_po': 'PurchaseApplyForm'
}
export default {
  components: {
    PurchaseApplyForm
  },
  props: {
    type: {
      type: String,
      default: ''
    },
    data: {
      type: Object,
      default: () => {
        return {}
      }
    }
  },
  data() {
    return {
      componentName: '',
      id: ''
    }
  },
  watch: {
    type: {
      handler(val) {
        this.changeType(val)
      },
      immediate: true
    }
  },
  methods: {
    changeType(val) {
      this.componentName = componentType[val]
      this.$nextTick(() => {
        switch (val) {
          case 'type_po':
            // this.$refs.components.getDetail(this.data.id)
            break

          default:
            break
        }
      })
    }
  }
}
</script>

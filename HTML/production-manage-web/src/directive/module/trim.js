/*
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-08-15 17:40:52
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-08 08:56:11
 * @FilePath: \production-manage-web\src\directive\module\trim.js
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
 */
/**
 * v-trim
 * 根据规则过滤t-input的值
 */
export default {
  update(el, binding) {
    const value = binding.value + ''
    const input = el.querySelector('.el-input__inner')
    if (!binding.value) {
      input.value = null
      input?.dispatchEvent(new Event('input'))
      return
    }
    if (input == null) throw new Error('该指令只在t-input组件生效!')
    // 过滤中文
    if (binding.modifiers.chinese) {
      input.value = value.replace(/[\u4E00-\u9FA5]/ig, '')
    }
    // 过滤空格
    if (binding.modifiers.space) {
      input.value = value.replace(/[\s]/ig, '')
    }
    // 只留数字
    if (binding.modifiers.exceptNumber) {
      input.value = value.replace(/[^\d^\.]+/g, '').replace('.', '$#$').replace(/\./g, '').replace('$#$', '.')
    }
    // 保留两位小数
    if (binding.modifiers.exceptTwoNumber) {
      input.value = value.replace(/[^\d^\.]+/g, '').replace('.', '$#$').replace(/\./g, '').replace('$#$', '.').replace(/^(\-)*(\d+)\.(\d\d).*$/, '$1$2.$3')
    }
    // 只留整数
    if (binding.modifiers.exceptInt) {
      input.value = value.replace(/[^0-9]/ig, '')
    }
    // 只留正整数
    if (binding.modifiers.positiveInt) {
      input.value = value.replace(/[^0-9]/ig, '').replace(/^[0]+/, '')
    }
    // 过滤特殊符号
    // 过滤所有符号
    input.dispatchEvent(new Event('input'))
  }
}

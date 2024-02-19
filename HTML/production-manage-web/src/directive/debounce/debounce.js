/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-08-04 08:33:40
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-04 08:43:57
 */
export default {
  inserted(el, binding) {
    el.addEventListener('click', () => {
      if (!el.disabled) {
        el.disabled = true
        setTimeout(() => {
          el.disabled = false
        }, binding.value || 3 * 1000)
      }
    })
  }
}


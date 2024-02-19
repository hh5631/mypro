// 利用webpack批量注册组件
const requireComponent = require.context('./', false, /\w+\.vue$/)
const comps = {}
requireComponent.keys().map(fileName => {
  // 排除不是打印的表单
  if (fileName !== './commonTitle.vue') {
    // console.log('fileName', fileName)
    const comp = requireComponent(fileName).default
    comps[comp.name] = comp
  }
})

export default comps

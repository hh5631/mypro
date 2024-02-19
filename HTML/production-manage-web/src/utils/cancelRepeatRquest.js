// 取消重复请求
/*  假如用户重复点击按钮，先后提交了 A 和 B 这两个完全相同（考虑请求路径、方法、参数）的请求，我们可以从以下几种拦截方案中选择其一：
 1. 取消 A 请求，只发出 B 请求（会导致A请求已经发出去,被后端处理了）
 2. 取消 B 请求，只发出 A 请求
 3. 取消 B 请求，只发出 A 请求，把收到的 A 请求的返回结果也作为 B 请求的返回结果
 第3种方案需要做监听处理增加了复杂性，结合我们实际的业务需求，最后采用了第2种方案来实现，即：
 只发第一个请求。在 A 请求还处于 pending 状态时，后发的所有与 A 重复的请求都取消，实际只发出 A 请求，直到 A 请求结束（成功/失败）才停止对这个请求的拦截。
*/
import Axios from 'axios'

// 把当前请求信息添加到pendingRequest对象 中；
const pendingRequest = new Map() // Map对象保存键值对。任何值(对象或者原始值) 都可以作为一个键或一个值。
export function addPendingRequest(config) {
  const requestKey = generateReqKey(config)
  // 判断pendingRequest中是否存在key
  if (pendingRequest.has(requestKey)) {
    config.cancelToken = new Axios.CancelToken(cancel => {
      // cancel 函数的参数会作为 promise 的 error 被捕获
      cancel(`${config.url} 请求已取消`)
    })
  } else {
    config.cancelToken =
        config.cancelToken ||
        new Axios.CancelToken(cancel => {
          pendingRequest.set(requestKey, cancel)
        })
  }
}

// removePendingRequest：检查是否存在重复请求，若存在则取消已发的请求。
export function removePendingRequest(config) {
  const requestKey = generateReqKey(config)
  // 判断是否有这个 key
  if (pendingRequest.has(requestKey)) {
    const cancelToken = pendingRequest.get(requestKey)
    // 取消之前发送的请求
    cancelToken(requestKey)
    // 从请求对象中删除requestKey
    pendingRequest.delete(requestKey)
  }
}

// 用于根据当前请求的信息，将重复的请求生成一个唯一的key
export function generateReqKey(config) {
  // 通过url，method，params，data生成唯一key，用于判断是否重复请求
  const requestObj = {
    url: config.url,
    data: typeof config.data === 'object' ? JSON.stringify(config.data) : config.data,
    method: config.method
  }
  const { method, url, data } = requestObj // 请求方式，参数，请求地址，
  return [method, url, data].join('&') // 拼接
}

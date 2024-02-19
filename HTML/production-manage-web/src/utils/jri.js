
/**
 * 通用js方法封装处理
 */

// 日期格式化
export function parseTime(time, pattern) {
  if (arguments.length === 0 || !time) {
    return null
  }
  const format = pattern || '{y}-{m}-{d} {h}:{i}:{s}'
  let date
  if (typeof time === 'object') {
    date = time
  } else {
    if ((typeof time === 'string') && (/^[0-9]+$/.test(time))) {
      time = parseInt(time)
    } else if (typeof time === 'string') {
      time = time.replace(new RegExp(/-/gm), '/').replace('T', ' ').replace(new RegExp(/\.[\d]{3}/gm), '')
    }
    if ((typeof time === 'number') && (time.toString().length === 10)) {
      time = time * 1000
    }
    date = new Date(time)
  }
  const formatObj = {
    y: date.getFullYear(),
    m: date.getMonth() + 1,
    d: date.getDate(),
    h: date.getHours(),
    i: date.getMinutes(),
    s: date.getSeconds(),
    a: date.getDay()
  }
  const time_str = format.replace(/{(y|m|d|h|i|s|a)+}/g, (result, key) => {
    let value = formatObj[key]
    // Note: getDay() returns 0 on Sunday
    if (key === 'a') { return ['日', '一', '二', '三', '四', '五', '六'][value] }
    if (result.length > 0 && value < 10) {
      value = '0' + value
    }
    return value || 0
  })
  return time_str
}

// 表单重置
export function resetForm(refName) {
  if (this.$refs[refName]) {
    this.$refs[refName].resetFields()
  }
}

// 添加日期范围
export function addDateRange(params, dateRange, propName) {
  const search = params
  search.params = typeof (search.params) === 'object' && search.params !== null && !Array.isArray(search.params) ? search.params : {}
  dateRange = Array.isArray(dateRange) ? dateRange : []
  if (typeof (propName) === 'undefined') {
    search.params['beginTime'] = dateRange[0]
    search.params['endTime'] = dateRange[1]
  } else {
    search.params['begin' + propName] = dateRange[0]
    search.params['end' + propName] = dateRange[1]
  }
  return search
}
// 添加日期范围
export function datesRange(params, dateRange, propName) {
  const search = params
  search.params = typeof (search.params) === 'object' && search.params !== null && !Array.isArray(search.params) ? search.params : {}
  dateRange = Array.isArray(dateRange) ? dateRange : []
  if (typeof (propName) === 'undefined') {
    search.startDate = dateRange[0]
    search.endDate = dateRange[1]
  } else {
    search.params['begin' + propName] = dateRange[0]
    search.params['end' + propName] = dateRange[1]
  }
  return search
}

// 回显数据字典
export function selectDictLabel(datas, value) {
  var actions = []
  Object.keys(datas).some((key) => {
    if (datas[key].value === ('' + value)) {
      actions.push(datas[key].label)
      return true
    }
  })
  return actions.join('')
}

// 回显数据字典（字符串数组）
export function selectDictLabels(datas, value, separator) {
  if (value === undefined) {
    return ''
  }
  var actions = []
  var currentSeparator = undefined === separator ? ',' : separator
  var temp = value.split(currentSeparator)
  Object.keys(value.split(currentSeparator)).some((val) => {
    Object.keys(datas).some((key) => {
      if (datas[key].value === ('' + temp[val])) {
        actions.push(datas[key].label + currentSeparator)
      }
    })
  })
  return actions.join('').substring(0, actions.join('').length - 1)
}

// 字符串格式化(%s )
export function sprintf(str) {
  var args = arguments; var flag = true; var i = 1
  str = str.replace(/%s/g, function() {
    var arg = args[i++]
    if (typeof arg === 'undefined') {
      flag = false
      return ''
    }
    return arg
  })
  return flag ? str : ''
}

// 转换字符串，undefined,null等转化为""
export function parseStrEmpty(str) {
  if (!str || str === 'undefined' || str === 'null') {
    return ''
  }
  return str
}

// 数据合并
export function mergeRecursive(source, target) {
  for (var p in target) {
    try {
      if (target[p].constructor === Object) {
        source[p] = mergeRecursive(source[p], target[p])
      } else {
        source[p] = target[p]
      }
    } catch (e) {
      source[p] = target[p]
    }
  }
  return source
}

/**
 * 构造树型结构数据
 * @param {*} data 数据源
 * @param {*} id id字段 默认 'id'
 * @param {*} parentId 父节点字段 默认 'parentId'
 * @param {*} children 孩子节点字段 默认 'children'
 */
export function handleTree(data, id, parentId, children) {
  const config = {
    id: id || 'id',
    parentId: parentId || 'parentId',
    childrenList: children || 'children'
  }

  var childrenListMap = {}
  var nodeIds = {}
  var tree = []

  for (const d of data) {
    const parentId = d[config.parentId]
    if (childrenListMap[parentId] == null) {
      childrenListMap[parentId] = []
    }
    nodeIds[d[config.id]] = d
    childrenListMap[parentId].push(d)
  }

  for (const d of data) {
    const parentId = d[config.parentId]
    if (nodeIds[parentId] == null) {
      tree.push(d)
    }
  }

  for (const t of tree) {
    adaptToChildrenList(t)
  }

  function adaptToChildrenList(o) {
    if (childrenListMap[o[config.id]] !== null) {
      o[config.childrenList] = childrenListMap[o[config.id]]
    }
    if (o[config.childrenList]) {
      for (const c of o[config.childrenList]) {
        adaptToChildrenList(c)
      }
    }
  }
  return tree
}

/**
* 参数处理
* @param {*} params  参数
*/
export function tansParams(params) {
  let result = ''
  for (const propName of Object.keys(params)) {
    const value = params[propName]
    var part = encodeURIComponent(propName) + '='
    if (value !== null && typeof (value) !== 'undefined') {
      if (typeof value === 'object') {
        for (const key of Object.keys(value)) {
          if (value[key] !== null && typeof (value[key]) !== 'undefined') {
            const params = propName + '[' + key + ']'
            var subPart = encodeURIComponent(params) + '='
            result += subPart + encodeURIComponent(value[key]) + '&'
          }
        }
      } else {
        result += part + encodeURIComponent(value) + '&'
      }
    }
  }
  return result
}

// 验证是否为blob格式
export async function blobValidate(data) {
  try {
    const text = await data.text()
    JSON.parse(text)
    return false
  } catch (error) {
    return true
  }
}
/** 树结构数据扁平化 */
export function flat(data) {
  return data.reduce((pre, cur) => {
    // 此处将对象的children属性和值都解构在空数组中，将对象的其他属性和值都解构在i里面。
    const { children = [], ...i } = cur// 注意 ...i 只能写在解构赋值的末尾，否则报错
    return pre.concat([{ ...i }], flat(children))
  }, [])
}
// 数字过滤
export const numberFilter = {
  // 整数
  toInt: (obj, value) => {
    obj[value] = obj[value].replace(/[^0-9]/g, '').replace(/^[0]+/, '') || ''
  },
  // 小数
  toFloat: (obj, value) => {
    obj[value] = obj[value].replace(/^\./g, '')
      .replace(/[^\d.]/g, '')
      .replace(/\.{2,}/g, '.')
      .replace('.', '$#$').replace(/\./g, '').replace('$#$', '.')
  },
  // 数量区间
  filterQty (record, attributes, compareNum) {
    if (record[attributes] > 0) {
      record[attributes] = record[attributes].replace(/[^0-9]/g, '').replace(/^[0]+/, '')
    } else {
      record[attributes] = 0
    }
    record[attributes] = record[attributes] > compareNum ? compareNum : record[attributes]
  }
}
/**
 *  通用改变当前页码: 解决删除最后一页所有数据后回到前一页无数据问题
 * @param total 总记录数
 * @param delNum 删除记录数据
 * @param pageNum 当前页
 * @param pageSize 每页条数
 * @returns {number} 返回 处理后的当前页
 */
export function changePageNum(total, delNum, pageNum, pageSize) {
  // 计算删除后的实际总页数
  const totalPage = Math.ceil((total - delNum) / pageSize)
  // 判断是否为最后一页
  const currentPage = pageNum > totalPage ? totalPage : pageNum
  // 判断是否为第一页
  return currentPage < 1 ? 1 : currentPage
}
// 字典相关
export const dictionary = {
  // 获取字典标签
  returnDictLabel (arr, val) {
    if (!arr || !val) {
      return ''
    }
    for (var item of arr) {
      if (item.value === val + '') {
        return item.label
      }
    }
  }
}
// 读卡url
export const readUrl = 'http://localhost:45114/rf/read'
// 写卡url
export const writeUrl = 'http://localhost:45114/rf/write'
// 打印标签传参的初始参数格式
export const initialPrintData = {
  Printer:
    {
      'printername': 'ZMIN X1',
      'printerdpi': '203',
      'printSpeed': '4',
      'printDarkness': '10'
    },
  'Operate': 'print',
  // 'lsfFilePath': 'http://10.100.2.151:20001/production/zdyjy.lsf', // 本地开发测试切换此地址
  'lsfFilePath': window.location.origin + '/production/zdyjy.lsf',
  'labels':
    [
      {
        'lsfFileVarList':
          [
            // {
            //   'lsfFileVar':
            //     {
            //       'varname': 'name',
            //       'varvalue': '笔记本电脑'
            //     }
            // },
            // {
            //   'lsfFileVar':
            //     {
            //       'varname': 'code',
            //       'varvalue': 'SZ-102'
            //     }
            // },
            // {
            //   'lsfFileVar':
            //     {
            //       'varname': 'model',
            //       'varvalue': '联想14p'
            //     }
            // },
            // {
            //   'lsfFileVar':
            //     {
            //       'varname': 'type',
            //       'varvalue': '通用设备'
            //     }
            // },
            // {
            //   'lsfFileVar':
            //     {
            //       'varname': 'dept',
            //       'varvalue': '科研二部'
            //     }
            // },
            // {
            //   'lsfFileVar':
            //     {
            //       'varname': 'time',
            //       'varvalue': '2022-03-01'
            //     }
            // },
            // {
            //   'lsfFileVar':
            //     {
            //       'varname': 'qrcode',
            //       'varvalue': '庄佳彬的笔记本电脑'
            //     }
            // }
          ]
      }
    ]
}
// 处理打印数据格式
export function handlePrintData(arr) {
  const printArr = []
  arr.map((e) => {
    const arr = [{
      'lsfFileVar':
        {
          'varname': 'name',
          'varvalue': e.name
        }
    },
    {
      'lsfFileVar':
        {
          'varname': 'code',
          'varvalue': e.internalCode
        }
    }, {
      'lsfFileVar':
        {
          'varname': 'model',
          'varvalue': e.standard
        }
    },
    {
      'lsfFileVar':
        {
          'varname': 'type',
          'varvalue': e.category
        }
    },
    {
      'lsfFileVar':
        {
          'varname': 'dept',
          'varvalue': e.dept
        }
    },
    {
      'lsfFileVar':
        {
          'varname': 'time',
          'varvalue': e.purchaseDate
        }
    },
    {
      'lsfFileVar':
        {
          'varname': 'qrcode',
          // 'varvalue': e.rfidCode
          'varvalue': e.code
        }
    }
    ]
    const obj = {
      lsfFileVarList: arr
    }
    printArr.push(obj)
  })
  console.log(printArr, '打印标签传参')
  return printArr
}
/**
 *判断el-input等元素的内容是否超过文本框宽度
 *
 * @export
 * @param {*} event//鼠标事件获取到的目标对象
 * @return {*}
 */
export function judeInputOver(event) {
  const ev_weight = event.scrollWidth // 文本的实际宽度   scrollWidth：对象的实际内容的宽度，不包边线宽度，会随对象中内容超过可视区后而变大。
  const content_weight = event.clientWidth// 文本的可视宽度 clientWidth：对象内容的可视区的宽度，不包滚动条等边线，会随对象显示大小的变化而改变。
  if (ev_weight > content_weight) {
    // 实际宽度 > 可视宽度  文字溢出
    return true
  } else {
    // 否则为不溢出
    return false
  }
}

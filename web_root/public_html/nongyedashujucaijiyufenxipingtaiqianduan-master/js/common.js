/**
 * Created by xx640 on 2017/4/27.
 */
function show_list() {
    var showUp = $(".upload-type ul ul");
    showUp.toggle();
}
function uploadFile() {
    var uploadFile = $(".upload-file");
    var connectDB = $(".connect-db");
    uploadFile.show();
    connectDB.hide();
}
function connectDatabase() {
    var uploadFile = $(".upload-file");
    var connectDB = $(".connect-db");
    uploadFile.hide();
    connectDB.show();
}
function show_list1() {
    var showUp = $(".digit-type ul ul");
    showUp.toggle();
}
function showBar(index) {
    //console.log(index);

    var hide_bar = $(".bar");
    var show_bar = $(".bar").eq(index);

    hide_bar.hide();
    show_bar.show();
    //show_bar.style.display = "block";
    // 基于准备好的dom，初始化echarts实例
    var myChart = echarts.init(document.getElementsByClassName('bar')[index]);

    // 指定图表的配置项和数据
    var option = {
        title: {
            text: 'ECharts 入门示例'
        },
        tooltip: {},
        legend: {
            data:['销量']
        },
        xAxis: {
            data: []
        },
        yAxis: {},
        series: [{
            name: '销量',
            type: 'bar',
            data: []
        }]
    };

    // 异步加载数据
    $.get('data.json').done(function (data) {
        // 填入数据
        myChart.setOption({
            xAxis: {
                data: data.categories
            },
            series: [{
                // 根据名字对应到相应的系列
                name: '销量',
                data: data.series[index].data
            }]
        });
    });
    // 使用刚指定的配置项和数据显示图表。
    myChart.setOption(option);
}
import QtQuick 2.0
import QtWebEngine 1.4

WebEngineView {
    property bool isFirstLoad: true
    anchors.centerIn: parent
    width: parent.width
    height: parent.height
    url: moduleManagerParam.flowChartURL
    onLoadProgressChanged: {
        if (loadProgress === 0) {
            console.log("Flowchart start to laod")
        } else if (loadProgress === 100 && isFirstLoad) {
            isFirstLoad = false
            console.log("Flowchart load complete")
            if (baseModuleManager.FlowchartFilename.length > 0) {
                console.log("Trying to load : " + baseModuleManager.FlowchartFilename)
                file.setSource(baseModuleManager.FlowchartFilename)
                var result = file.read()
                var command = "document.getElementById('flowchart_data').value ='" + result + "'";
                flowChartPage.webView.runJavaScript(command, function(result) {
                    console.log("Load flowchart success")
                    command = "document.getElementById('set_data').click()"
                    flowChartPage.webView.runJavaScript(command)
                    baseModuleManager.loadFlowchart(result, loadfileDialog.fileUrl)
                })
            }
        }
    }
}

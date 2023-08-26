#!/usr/bin/python3

import xml.etree.ElementTree as ET

HTML_DATA = """
<!DOCTYPE html>
<html>
<head>
<title>page title</title>
<style>
table, th, td {
  border: 1px solid black;
  border-collapse: collapse;
  padding-top: 10px;
  padding-bottom: 10px;
  padding-left: 10px;
  padding-right: 10px;
}
</style>
</head>
<body>
 <table>
  <tr>
    <th>Start Time</th>
    <th>Export ID</th>
    <th>Duration (hh:mm:ss)</th>
  </tr>
  <tr>
      <td>
        Time 1
      </td>
      <td>
        <a target="_blank" href="status/abc?pretty=1">abc</a>
      </td>
      <td>
        10 seconds
      </td>
</tr>
</table>
</body>
</html>
"""

root = ET.fromstring(HTML_DATA)
print(root.tag, root[0].tag, root[1].tag)

body = root.find("body")
tables = body.findall("table")
table = tables[0]
print(table.findall("tr"))

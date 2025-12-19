#!/usr/bin/env python
# -*- coding: utf-8 -*-
from datetime import datetime
from email.header import Header
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import os


class EmailSender:
    def __init__(self, sender, username, passwd):
        self.sender = sender
        self.username = username
        self.passwd = passwd

    def send(self, subject, html_content, attachment_path, receivers, cc_list):
        message = MIMEMultipart()
        message['Subject'] = Header(subject, 'utf-8')
        message['From'] = self.sender
        message['To'] = ', '.join(receivers)
        message['Cc'] = ', '.join(cc_list)

        content = MIMEText(html_content, _subtype='html', _charset='utf-8')
        message.attach(content)

        attachment = MIMEText(open(attachment_path, 'rb').read(), 'base64', 'utf-8')
        attachment["Content-Type"] = 'application/octet-stream'
        attachment[
            "Content-Disposition"] = f'attachment; filename={os.path.split(os.path.realpath(attachment_path))[1]}'
        message.attach(attachment)

        smtp_obj = smtplib.SMTP('smtp.huawei.com')
        smtp_obj.login(self.username, self.passwd)
        smtp_obj.sendmail(self.sender, receivers + cc_list, message.as_string())
        smtp_obj.quit()

    @staticmethod
    def generate_email_content(test_results, start_time, end_time, branch):
        email = {}
        count_all = test_results['total']
        count_pass = test_results['passed']
        count_fail = count_all - count_pass
        pass_rate = 0 if count_all == 0 else round(count_pass * 100 / count_all, 2)
        test_time = datetime.now().strftime("%Y-%m-%d")
        subject = f'[MindStudio Insight] GUI冒烟用例测试报告_{test_time} ' \
                  f'(Fail: {count_fail} Pass: {count_pass} Total: {count_all})'
        email.update({'subject': subject})
        if not test_results:
            email.update({'html_content': '<div>Test failed, see the log for details.</div>'})
            return email

        html_content = (
            "<head>"
            f"<title>{subject}</title>"
            "<style>"
            ".fail { color:#CD0000; } "
            ".pass { color:#458B00; } "
            "font { size:'5'; face:'arial'; }"
            "td { text-align:center;vertical-align:middle; }"
            "</style>"
            "</head>"
            "<body>"
            f"<font size='2' color='blue'><b>Start time: </b>{start_time}<br/><font>"
            f"<font size='2' color='blue'><b>End time: </b>{end_time}<br/><font>"
            f"<font size='2' color='blue'><b>Branch: </b>{branch}<br/><font>"
            f"<p><font size='4' color='black'><b>GUI自动化通过率为：</span><font size='4' color='{'red' if pass_rate < 90 else 'green'}'>{pass_rate}%</span></p>"
            "<table width='800' border='2' bordercolor='black' cellspacing='2'>"
            "   <tr>"
            "       <td><strong>Case No</strong></td>"
            "       <td><strong>Model Name</strong></td>"
            "       <td><strong>Case Name</strong></td>"
            "       <td><strong><font>Test Case Execution Result</font></strong></td>"
            "   </tr>"
        )

        count = 0
        for model, model_result in test_results['result'].items():
            for case, result in model_result.items():
                count += 1
                html_content += (
                    "<tr>"
                    f"   <td>{str(count)}</td>"
                    f"   <td>{model}</td>"
                    f"   <td>{case}</td>"
                    f"   <td class = {result}><strong>{result}<strong></td>"
                    "</tr>"
                )
        email.update({'html_content': html_content})

        return email

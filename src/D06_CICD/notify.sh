#!bin/bash
TOKEN="" # bot token removed for privacy
USER="" # user id removed for privacy
TIME=10

URL="https://api.telegram.org/bot$TOKEN/sendMessage"
TEXT="Project:+$CI_PROJECT_NAME%0A%0AJob name: $CI_JOB_NAME%0AStatus: $CI_JOB_STATUS%0A%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG"

curl -s --max-time $TIME -d "chat_id=$USER&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null

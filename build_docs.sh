#!/bin/bash

function checkErrors
{
	if [ $? != 0 ]
	then
		echo 
		echo "An error occured while deploying artifacts."
		echo
		exit 1
	fi
}

function dispatchDocBuildsEvent
{
	TAG_NAME=$APPVEYOR_REPO_TAG_NAME
	CI_NAME="AppVeyor"
	CONFIG_NAME=$APPVEYOR_BUILD_WORKER_IMAGE + " " + $PLATFORM + " " + $CONFIGURATION
	JOB_ID=$APPVEYOR_JOB_ID

	echo "Dispatch GitHub Action to build documentation..."	
	echo "TAG=$TAG_NAME"
	echo "CI=$CI_NAME"
	echo "CONFIGURATION=$CONFIG_NAME"
	echo "JOB=$JOB_ID"
	echo ""
	
	BODY_CONTENT="{\"event_type\": \"doc-build\", \"client_payload\": {\"tag\": \"$TAG_NAME\", \"configuration\": \"$CONFIG_NAME\", \"ci\": \"$CI_NAME\", \"job\": \"$JOB_ID\"}}"
	echo "BODY_CONTENT=$BODY_CONTENT"
	echo ""

	curl -H "Authorization: token $GITHUB_ACTION_DISPATCH_TOKEN" \
		 -H 'Accept: application/vnd.github.everest-preview+json' \
		 "https://api.github.com/repos/systelab/cpp-trace-api/dispatches" \
		 -d "$BODY_CONTENT"
	checkErrors
	echo "Done.\n"
}


# MAIN
if [[ -z $APPVEYOR_REPO_TAG_NAME ]]
then
	echo
	echo "Not dispatching build docs event because this is not a tag build"
	exit 0
fi

dispatchDocBuildsEvent

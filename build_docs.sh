#!/bin/bash

#usage
function usage
{
	echo "Usage: $0 -t TAG -c CI -n CONFIG_NAME -j JOB_ID"
	echo ""
	echo
	echo "  -t ARG   Tag name associated to current build"
	echo "  -c ARG   Continuous Integration system (AppVeyor or Travis)"
	echo "  -n ARG   Configuration name of the build"
	echo "  -j ARG   Identifier of the build job"
}

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

function parseParameters
{
	TAG_NAME=
	CI_NAME=
	CONFIG_NAME=
	JOB_ID=

	while getopts "ht:c:n:j:" OPTION 
	do
		case $OPTION in
		h)
			usage
			exit
			;;
		t)
			TAG_NAME=$OPTARG
			;;
		c)
			CI_NAME=$OPTARG
			;;
		n)
			CONFIG_NAME=$OPTARG
			;;
		j)
			JOB_ID=$OPTARG
			;;
		?)		
			echo "Invalid parameter" 
			usage
			exit
			;;
		esac
	done
}


function dispatchDocBuildsEvent
{
	if [[ -z $TAG_NAME ]]
	then
		echo
		echo "Not dispatching build docs event because this is not a tag build"
		exit 0
	fi

	echo "Dispatch GitHub Action to build documentation..."	
	echo "TAG=$TAG_NAME"
	echo "CI=$CI_NAME"
	echo "CONFIG_NAME=$CONFIG_NAME"
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
parseParameters "${@}"
dispatchDocBuildsEvent
